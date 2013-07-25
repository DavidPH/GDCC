//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// MageCraft target information and handling class.
//
//-----------------------------------------------------------------------------

#include "Info.hpp"

#include "GDCC/IR/Function.hpp"

#include "GDCC/IR/Exp/ValueGlyph.hpp"
#include "GDCC/IR/Exp/ValueRoot.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Bytecode
{
   namespace MgC
   {
      //
      // Info::CheckArg
      //
      void Info::CheckArg(GDCC::IR::Arg const &arg, GDCC::Origin const &pos)
      {
         switch(arg.a)
         {
         case GDCC::IR::ArgBase::Lit: break;
         case GDCC::IR::ArgBase::Nul: break;
         case GDCC::IR::ArgBase::Stk: break;

         case GDCC::IR::ArgBase::LocArs:
            CheckArg(*arg.aLocArs.idx, pos);
            break;

         case GDCC::IR::ArgBase::LocReg:
            CheckArgB(arg, GDCC::IR::ArgBase::Lit, pos);
            break;

         default:
            std::cerr << "ERROR: " << pos << ": MgC cannot use Arg: " << arg.a << '\n';
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

