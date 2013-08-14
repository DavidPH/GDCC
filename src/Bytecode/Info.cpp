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
#include "GDCC/IR/Object.hpp"
#include "GDCC/IR/StrEnt.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Bytecode
{
   //
   // Info::gen
   //
   void Info::gen()
   {
      for(auto &itr : GDCC::IR::Space::GblArs) genSpace(itr.second);
      for(auto &itr : GDCC::IR::Space::MapArs) genSpace(itr.second);
      for(auto &itr : GDCC::IR::Space::WldArs) genSpace(itr.second);

      genSpace(GDCC::IR::Space::GblReg);
      genSpace(GDCC::IR::Space::LocArs);
      genSpace(GDCC::IR::Space::MapReg);
      genSpace(GDCC::IR::Space::WldReg);

      for(auto &itr : GDCC::IR::FunctionRange())
         genFunc(itr.second);

      for(auto &itr : GDCC::IR::StrEntRange())
         genStr(itr.second);
   }

   //
   // Info::genBlock
   //
   void Info::genBlock(GDCC::IR::Block &block)
   {
      for(auto &stmnt : block)
         genStmnt(stmnt);
   }

   //
   // Info::genFunc
   //
   void Info::genFunc(GDCC::IR::Function &func_)
   {
      try
      {
         func = &func_;
         genBlock(func->block);
         func = nullptr;
      }
      catch(...)
      {
         func = nullptr;
         throw;
      }
   }

   //
   // Info::genSpace
   //
   void Info::genSpace(GDCC::IR::Space &)
   {
   }

   //
   // Info::genStmnt
   //
   void Info::genStmnt(GDCC::IR::Statement &)
   {
   }

   //
   // Info::genStr
   //
   void Info::genStr(GDCC::IR::StrEnt &)
   {
   }

   //
   // Info::tr
   //
   void Info::tr()
   {
      for(auto &itr : GDCC::IR::Space::GblArs) trSpace(itr.second);
      for(auto &itr : GDCC::IR::Space::MapArs) trSpace(itr.second);
      for(auto &itr : GDCC::IR::Space::WldArs) trSpace(itr.second);

      trSpace(GDCC::IR::Space::GblReg);
      trSpace(GDCC::IR::Space::LocArs);
      trSpace(GDCC::IR::Space::MapReg);
      trSpace(GDCC::IR::Space::WldReg);

      for(auto &itr : GDCC::IR::FunctionRange())
         trFunc(itr.second);

      for(auto &itr : GDCC::IR::StrEntRange())
         trStr(itr.second);
   }

   //
   // Info::trBlock
   //
   void Info::trBlock(GDCC::IR::Block &block)
   {
      for(auto &stmnt : block)
         trStmnt(stmnt);
   }

   //
   // Info::trFunc
   //
   void Info::trFunc(GDCC::IR::Function &func_)
   {
      try
      {
         func = &func_;
         trBlock(func->block);
         func = nullptr;
      }
      catch(...)
      {
         func = nullptr;
         throw;
      }
   }

   //
   // Info::trSpace
   //
   void Info::trSpace(GDCC::IR::Space &)
   {
   }

   //
   // Info::trStr
   //
   void Info::trStr(GDCC::IR::StrEnt &)
   {
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

