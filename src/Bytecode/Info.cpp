//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Target information and handling base class.
//
//-----------------------------------------------------------------------------

#include "Info.hpp"

#include "GDCC/IR/Function.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Bytecode
{
   //
   // Info::translate
   //
   void Info::translate()
   {
      for(auto &itr : GDCC::IR::FunctionRange())
         translateFunction(itr.second);
   }

   //
   // Info::translateBlock
   //
   void Info::translateBlock(GDCC::IR::Block &block)
   {
      for(auto &stmnt : block)
         translateStatement(stmnt);
   }

   //
   // Info::translateFunction
   //
   void Info::translateFunction(GDCC::IR::Function &func)
   {
      translateBlock(func.block);
   }

   //
   // Info::CheckArgB
   //
   void Info::CheckArgB(GDCC::IR::Arg const &arg, GDCC::IR::AddrBase b, GDCC::Origin pos)
   {
      if(arg.a != b)
      {
         std::cerr << "ERROR: " << pos << ": " << arg.a << " must have "
            << b << " parameter\n";
         throw EXIT_FAILURE;
      }
   }

   //
   // Info::CheckArgB
   //
   void Info::CheckArgB(GDCC::IR::Statement &stmnt, std::size_t a, GDCC::IR::ArgBase b)
   {
      if(stmnt.args[a].a != b)
      {
         std::cerr << "ERROR: " << stmnt.pos << ": " << stmnt.code
            << " must have " << b << " args[" << a << "]\n";
         throw EXIT_FAILURE;
      }
   }

   //
   // Info::CheckArgC
   //
   void Info::CheckArgC(GDCC::IR::Statement &stmnt, std::size_t c)
   {
      if(stmnt.args.size() < c)
      {
         std::cerr << "ERROR: " << stmnt.pos << ": bad argc for " << stmnt.code
            << ": " << stmnt.args.size() << " < " << c << '\n';
         throw EXIT_FAILURE;
      }
   }

   //
   // Info::IsExp0
   //
   bool Info::IsExp0(GDCC::IR::Exp const *exp)
   {
      if(!exp) return true;

      auto val = exp->getValue();

      switch(val.v)
      {
      case GDCC::IR::ValueBase::Fixed:
         return !val.vFixed.value;

      default:
         return false;
      }
   }
}

// EOF

