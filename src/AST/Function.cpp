//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree function handling.
//
//-----------------------------------------------------------------------------

#include "AST/Function.hpp"

#include "AST/Statement.hpp"
#include "AST/Type.hpp"

#include "Core/Option.hpp"

#include "IR/CallType.hpp"
#include "IR/Exp.hpp"
#include "IR/Linkage.hpp"
#include "IR/Program.hpp"
#include "IR/ScriptType.hpp"

#include "Option/Int.hpp"

#include "Platform/Platform.hpp"


//----------------------------------------------------------------------------|
// Options                                                                    |
//

//
// --alloc-Loc
//
static GDCC::Core::FastU AllocLoc = 4096;
static GDCC::Option::Int<GDCC::Core::FastU> AllocLocOpt
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("alloc-Loc")
      .setGroup("codegen")
      .setDescS("Sets the default Loc stack size.")
      .setDescL("Sets the default Loc stack size. Default is 4096."),

   &AllocLoc
};


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Function constructor
      //
      Function::Function(Core::String name_, Core::String glyph_) :
         allocLoc{0},
         ctype   {IR::CallType::None},
         glyph   {glyph_},
         label   {Core::STRNULL},
         labelEnd{nullptr},
         labelTmp{Core::STRNULL},
         linka   {IR::Linkage::None},
         localArs{0},
         localReg{0},
         name    {name_},
         param   {0},
         paramOpt{0},
         retrn   {nullptr},
         stmnt   {nullptr},
         stype   {IR::ScriptType::None},
         valueInt{nullptr},
         valueStr{Core::STRNULL},

         defin   {false},
         sflagNet{false},
         sflagClS{false},
         used    {false},

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

         // Operate on a temporary function to be merged later.
         IR::Function fn{glyph};

         fn.ctype    = IR::GetCallTypeIR(ctype);
         fn.linka    = linka;
         fn.stype    = stype;

         fn.sflagNet = sflagNet;
         fn.sflagClS = sflagClS;

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
         if(stmnt) stmnt->genStmnt({fn.block, this, prog});

         fn.ctype    = IR::GetCallTypeIR(ctype);
         fn.label    = label;
         fn.linka    = linka;
         fn.localArs = localArs;
         fn.localReg = localReg + localTmp.max();
         fn.param    = param;
         fn.retrn    = retrn && !retrn->isTypeVoid() ? retrn->getSizeWords() : 0;
         fn.stype    = stype;

         fn.defin    = defin;
         fn.sflagNet = sflagNet;
         fn.sflagClS = sflagClS;

         // Special rules for certain calling conventions.
         switch(fn.ctype)
         {
         case IR::CallType::StdCall:
            // Extra parameter for stack pointer. Extra register is handled
            // during local allocations.
            if(Platform::TargetCur == Platform::Target::ZDoom)
               ++fn.param;
            break;

         case IR::CallType::ScriptI:
         case IR::CallType::ScriptS:
            // Extra register for stack pointer.
            ++fn.localReg;
            break;

         default:
            break;
         }

         // Check for explicit allocation.
         if(valueInt)
         {
            auto val = valueInt->getValue();
            if(val.v == IR::ValueBase::Fixed)
               fn.valueInt = Core::NumberCast<Core::FastU>(val.vFixed.value);
            else
               fn.alloc = true;
         }
         else if(valueStr)
            fn.valueStr = valueStr;
         else
            fn.alloc = true;

         // If any temporaries generated, back the base glyph.
         if(labelTmp && localTmp.max())
         {
            auto &gdata = prog.getGlyphData(labelTmp);

            gdata.type  = Type::Size->getIRType();
            gdata.value = IR::ExpCreate_Value(
               IR::Value_Fixed(localReg, gdata.type.tFixed),
               Core::Origin(Core::STRNULL, 0));
         }

         // If this is a named script, generate a StrEnt, too.
         if(fn.ctype == IR::CallType::SScriptS ||
            fn.ctype == IR::CallType::ScriptS)
         {
            // Create StrEnt.
            auto &strent = prog.getStrEnt(genLabel());

            // Configure StrEnt.
            strent.valueStr = fn.valueStr ? fn.valueStr : fn.glyph;
            strent.alias    = true;
            strent.alloc    = true;
            strent.defin    = true;

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
      // Function::getLabelTmp
      //
      Core::String Function::getLabelTmp()
      {
         if(!labelTmp)
            labelTmp = genLabel();

         return labelTmp;
      }

      //
      // Function::setAllocLoc
      //
      void Function::setAllocLoc(IR::Exp const *exp)
      {
         if(exp)
            allocLoc = exp->getValue().getFastU();

         else switch(IR::GetCallTypeIR(ctype))
         {
         case IR::CallType::ScriptI:
         case IR::CallType::ScriptS:
            if(!stmnt || !stmnt->isNoAuto())
               allocLoc = AllocLoc;
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
}

// EOF

