//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation function handling.
//
//-----------------------------------------------------------------------------

#include "IR/Function.hpp"

#include "IR/CallType.hpp"
#include "IR/Linkage.hpp"
#include "IR/IArchive.hpp"
#include "IR/OArchive.hpp"
#include "IR/Program.hpp"
#include "IR/ScriptType.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Function constructor
      //
      Function::Function(Core::String glyph_) :
         block   {},
         ctype   {CallType::None},
         glyph   {glyph_},
         label   {Core::STRNULL},
         linka   {Linkage::None},
         localArs{0},
         localReg{0},
         param   {0},
         retrn   {0},
         stype   {ScriptType::None},
         valueInt{0},
         valueStr{Core::STRNULL},

         alloc   {false},
         defin   {false},
         sflagNet{false},
         sflagClS{false}
      {
      }

      //
      // Function::allocValue
      //
      void Function::allocValue(Program &prog, CallType const *ctypeVec,
         std::size_t ctypeLen)
      {
         for(;; ++valueInt)
         {
            for(auto const &func : prog.rangeFunction())
            {
               if(&func != this && !func.alloc && func.valueInt == valueInt)
               {
                  for(auto ctypeItr : Core::MakeRange(ctypeVec, ctypeVec + ctypeLen))
                     if(func.ctype == ctypeItr) goto nextValue;
               }
            }

            break;

         nextValue:;
         }

         alloc = false;
      }

      //
      // operator OArchive << Function
      //
      OArchive &operator << (OArchive &out, Function const &in)
      {
         return out << in.block << in.ctype << in.label << in.linka << in.linka
            << in.localArs << in.localReg << in.param << in.retrn << in.stype
            << in.valueInt << in.valueStr << in.alloc << in.defin << in.sflagNet
            << in.sflagClS;
      }

      //
      // operator IArchive >> Function
      //
      IArchive &operator >> (IArchive &in, Function &out)
      {
         in >> out.block >> out.ctype >> out.label >> out.linka >> out.linka
            >> out.localArs >> out.localReg >> out.param >> out.retrn >> out.stype
            >> out.valueInt >> out.valueStr;

         out.alloc    = GetIR<bool>(in);
         out.defin    = GetIR<bool>(in);
         out.sflagNet = GetIR<bool>(in);
         out.sflagClS = GetIR<bool>(in);

         return in;
      }
   }
}

// EOF

