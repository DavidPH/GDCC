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

         labelNum{0},
         labelStr{nullptr},
         labelSuf{nullptr}
      {
         auto const &dat = glyph.getData();

         // Allocate label buffer.
         // glyph "$L$" labelNum "\0"
         labelLen = dat.len + 3;
         labelStr.reset(new char[labelLen + ((sizeof(labelNum) * CHAR_BIT + 2) / 3) + 1]);
         std::memcpy(labelStr.get(), dat.str, dat.len);

         // Add mangle suffix.
         labelSuf = &labelStr[dat.len];
         labelSuf[0] = '$';
         labelSuf[1] = 'L';
         labelSuf[2] = '$';

         // Precompute hash base for label prefix.
         labelHash = Core::HashString(labelSuf, 3, dat.hash);

         labelSuf += 3;
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
         std::size_t len = std::sprintf(labelSuf, "%zu", ++labelNum);

         auto hash = Core::HashString(labelSuf, len, labelHash);

         return Core::AddString(labelStr.get(), len + labelLen, hash);
      }

      //
      // Function::getIRFunction
      //
      IR::Function &Function::getIRFunction(IR::Program &prog)
      {
         auto &fn = prog.getFunction(glyph);

         fn.ctype    = ctype;
         fn.label    = label;
         fn.linka    = linka;
         fn.localArs = localArs;
         fn.localReg = localReg;
         fn.param    = param;
         fn.retrn    = retrn && !retrn->isTypeVoid() ? retrn->getSizeWords() : 0;
         fn.stype    = stype;

         fn.alloc    = true;
         fn.defin    = defin;
         fn.sflagNet = sflagNet;
         fn.sflagClS = sflagClS;

         if(stmnt)
            stmnt->genStmnt(fn.block, this);

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

         return fn;
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

