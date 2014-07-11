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

#include "IR/CallType.hpp"
#include "IR/Exp.hpp"
#include "IR/Linkage.hpp"
#include "IR/Program.hpp"
#include "IR/ScriptType.hpp"

#include <climits>
#include <cstdio>


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
         ctype   {IR::CallType::None},
         glyph   {glyph_},
         label   {Core::STRNULL},
         labelTmp{Core::STRNULL},
         linka   {IR::Linkage::None},
         localArs{0},
         localReg{0},
         name    {name_},
         param   {0},
         retrn   {nullptr},
         stmnt   {nullptr},
         stype   {IR::ScriptType::None},
         valueInt{nullptr},
         valueStr{Core::STRNULL},

         defin   {false},
         sflagNet{false},
         sflagClS{false},

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
         // Operate on a temporary function to be merged later.
         IR::Function fn{glyph};

         fn.ctype    = ctype;
         fn.linka    = linka;
         fn.stype    = stype;

         fn.sflagNet = sflagNet;
         fn.sflagClS = sflagClS;

         // Configure glyph's type, even if the glyph won't be backed.
         prog.getGlyphData(glyph).type = IR::Type_Funct(ctype);

         // Merge into existing function (if any).
         prog.mergeFunction(prog.getFunction(glyph), std::move(fn));
      }

      //
      // Function::genFunctionDefn
      //
      void Function::genFunctionDefn(IR::Program &prog)
      {
         // Operate on a temporary function to be merged later.
         IR::Function fn{glyph};

         // Generate statements.
         if(stmnt) stmnt->genStmnt({fn.block, this, prog});

         fn.ctype    = ctype;
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

         // Check for explicit allocation.
         if(valueInt)
         {
            auto val = valueInt->getValue();
            if(val.v == IR::ValueBase::Fixed)
               fn.valueInt = number_cast<Core::FastU>(val.vFixed.value);
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

         // Configure glyph's type, even if the glyph won't be backed.
         prog.getGlyphData(glyph).type = IR::Type_Funct(ctype);

         // Merge into existing function (if any).
         prog.mergeFunction(prog.getFunction(glyph), std::move(fn));
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
      // Function::Create
      //
      Function::Ref Function::Create(Core::String name, Core::String glyph)
      {
         return static_cast<Ref>(new Function(name, glyph));
      }
   }
}

// EOF

