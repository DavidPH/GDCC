//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// MageCraft target information and handling class.
//
//-----------------------------------------------------------------------------

#include "Bytecode/MgC/Info.hpp"

#include "IR/Arg.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Bytecode
   {
      namespace MgC
      {
         //
         // Info::CheckArg
         //
         void Info::CheckArg(IR::Arg const &arg, Core::Origin const &pos)
         {
            switch(arg.a)
            {
            case IR::ArgBase::Lit: break;
            case IR::ArgBase::Nul: break;
            case IR::ArgBase::Stk: break;

            case IR::ArgBase::LocReg:
               CheckArgB(*arg.aLocReg.idx, IR::ArgBase::Lit, pos);
               break;

            case IR::ArgBase::Sta:
               CheckArg(*arg.aSta.idx, pos);
               break;

            default:
               std::cerr << "ERROR: " << pos << ": MgC cannot use Arg: "
                  << arg.a << '\n';
               throw EXIT_FAILURE;
            }
         }
      }
   }
}

// EOF

