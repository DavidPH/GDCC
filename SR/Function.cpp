//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2023 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation function handling.
//
//-----------------------------------------------------------------------------

#include "SR/Function.hpp"

#include "SR/Statement.hpp"
#include "SR/Type.hpp"

#include "Core/Exception.hpp"
#include "Core/Option.hpp"

#include "IR/Exp.hpp"
#include "IR/Linkage.hpp"
#include "IR/Program.hpp"

#include "Option/Int.hpp"

#include "Target/CallType.hpp"
#include "Target/Info.hpp"


//----------------------------------------------------------------------------|
// Options                                                                    |
//

namespace GDCC::SR
{
   //
   // --alloc-Aut
   //
   static Option::Int<Core::FastU> AllocAut
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("alloc-Aut")
         .setGroup("codegen")
         .setDescS("Sets the default Aut stack size.")
         .setDescL("Sets the default Aut stack size. Default is 4096."),

      4096
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::SR
{
   //
   // Function constructor
   //
   Function::Function(Core::String name_, Core::String glyph_) :
      allocAut{0},
      ctype   {IR::CallType::None},
      glyph   {glyph_},
      label   {nullptr},
      labelEnd{nullptr},
      labelRes{nullptr},
      labelTmp{nullptr},
      linka   {IR::Linkage::None},
      localAut{0},
      localReg{0},
      name    {name_},
      paramOpt{0},
      retrn   {nullptr},
      stmnt   {nullptr},
      stype   {},
      valueInt{nullptr},
      valueStr{nullptr},
      warnUse {nullptr},

      declAuto   {false},
      defin      {false},
      delay      {false},
      noInitDelay{false},
      used       {false},
      warnDone   {false},

      labeller{glyph, "$L$"}
   {
   }

   //
   // Function destructor
   //
   Function::~Function()
   {
   }

   //
   // Function::genLabel
   //
   Core::String Function::genLabel()
   {
      return labeller();
   }

   //
   // Function::genFunctionDecl
   //
   void Function::genFunctionDecl(IR::Program &prog)
   {
      if(!defin && !used) return;

      if(declAuto && !defin)
         Core::ErrorUndef("forward reference", glyph);

      // Operate on a temporary function to be merged later.
      IR::Function fn{glyph};

      fn.ctype = Target::GetCallTypeIR(ctype);
      fn.linka = linka;
      fn.stype = stype;

      // Configure glyph's type, even if the glyph won't be backed.
      prog.getGlyphData(glyph).type = IR::Type_Funct(fn.ctype);

      // Merge into existing function (if any).
      prog.mergeFunction(prog.getFunction(glyph), std::move(fn));
   }

   //
   // Function::genFunctionDefn
   //
   void Function::genFunctionDefn(IR::Program &prog)
   {
      if(!defin && !used) return;

      // Operate on a temporary function to be merged later.
      IR::Function fn{glyph};

      // Generate statements.
      if(stmnt)
      {
         stmnt->genStmnt({fn.block, this, prog});
         if(fn.block.hasLabelPending())
            fn.block.addStmnt(IR::CodeBase::Nop);
      }

      try
      {
         fn.param = type->getCallWords();
      }
      catch(SR::TypeError const &)
      {
         Core::Error(stmnt ? stmnt->pos : Core::Origin(), "incomplete parameter");
      }

      fn.allocAut = allocAut;
      fn.ctype    = Target::GetCallTypeIR(ctype);
      fn.label    = label;
      fn.linka    = linka;
      fn.localArr = localArr;
      fn.localAut = localAut;
      fn.localReg = localReg + localTmp.max();
      fn.retrn    = retrn && !retrn->isTypeVoid() ? retrn->getSizeWords() : 0;
      fn.stype    = stype;

      fn.defin    = defin;

      // Special rules for certain calling conventions.

      // Extra parameter for stack pointer.
      if(Target::IsCallAutoProp(fn.ctype))
         ++fn.param;

      // Extra register for stack pointer.
      if(fn.allocAut &&
         (fn.ctype == IR::CallType::ScriptI ||
            fn.ctype == IR::CallType::ScriptS ||
            fn.ctype == IR::CallType::StkCall))
      {
         // If there are fewer local registers than parameters, make sure not
         // to clobber any of the parameters with the stack pointer.
         fn.localReg = std::max(fn.localReg + 1, fn.param + 1);
      }

      // Check for explicit allocation.
      if(valueInt)
      {
         auto val = valueInt->getValue();
         if(val.v == IR::ValueBase::Fixed)
            fn.valueInt = Core::NumberCast<Core::FastU>(val.vFixed.value);
      }
      else
         fn.alloc = true;

      // Having a valueStr must not prevent automatic valueInt allocation.
      if(valueStr)
         fn.valueStr = valueStr;

      // If any temporaries generated, back the base glyph.
      if(labelTmp && localTmp.max())
      {
         auto &gdata = prog.getGlyphData(labelTmp);

         gdata.type  = Type::Size->getIRType();
         gdata.value = IR::ExpCreate_Value(
            IR::Value_Fixed(Core::NumberCast<Core::Integ>(localReg * Target::GetWordBytes()),
            gdata.type.tFixed), {nullptr, 0});
      }

      // If this is a named script, generate a StrEnt, too.
      if(fn.ctype == IR::CallType::SScriptS ||
         fn.ctype == IR::CallType::ScriptS)
      {
         // Create StrEnt.
         auto &strent = prog.getStrEnt(glyph + Core::String("$StrEnt"));

         // Configure StrEnt.
         strent.valueStr = fn.valueStr ? fn.valueStr : fn.glyph;
         strent.alias    = true;
         strent.alloc    = true;
         strent.defin    = true;
         strent.multiDef = true;

         // Prepare associated glyph.
         prog.getGlyphData(strent.glyph).type = IR::Type_StrEn();
      }

      // Configure glyph's type, even if the glyph won't be backed.
      prog.getGlyphData(glyph).type = IR::Type_Funct(fn.ctype);

      // Merge into existing function (if any).
      prog.mergeFunction(prog.getFunction(glyph), std::move(fn));
   }

   //
   // Function::getLabelEnd
   //
   Core::String Function::getLabelEnd()
   {
      if(!labelEnd)
         labelEnd = genLabel();

      return labelEnd;
   }

   //
   // Function::getLabelRes
   //
   Core::String Function::getLabelRes()
   {
      if(!labelRes)
         labelRes = genLabel();

      return labelRes;
   }

   //
   // Function::getLabelTmp
   //
   Core::String Function::getLabelTmp()
   {
      if(!labelTmp)
         labelTmp = genLabel();

      return labelTmp;
   }

   //
   // Function::setAllocAut
   //
   void Function::setAllocAut(IR::Exp const *exp)
   {
      if(exp)
         allocAut = exp->getValue().getFastU();

      else switch(Target::GetCallTypeIR(ctype))
      {
      case IR::CallType::ScriptI:
      case IR::CallType::ScriptS:
      case IR::CallType::StkCall:
         if(!stmnt || !stmnt->isNoAuto())
            allocAut = AllocAut;
         break;

      default:
         break;
      }
   }

   //
   // Function::Create
   //
   Function::Ref Function::Create(Core::String name, Core::String glyph)
   {
      return static_cast<Ref>(new Function(name, glyph));
   }
}

// EOF

