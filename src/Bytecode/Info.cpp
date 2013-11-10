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
// Macros                                                                     |
//

//
// TryPointer
//
#define TryPointer(fun, ptr) \
   try \
   { \
      ptr = &ptr##_; \
      fun(); \
      ptr = nullptr; \
   } \
   catch(...) \
   { \
      ptr = nullptr; \
      throw; \
   }


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
   void Info::genBlock()
   {
      for(auto &stmnt_ : *block)
         genStmnt(stmnt_);
   }

   //
   // Info::genBlock
   //
   void Info::genBlock(GDCC::IR::Block &block_)
   {
      TryPointer(genBlock, block);
   }

   //
   // Info::genFunc
   //
   void Info::genFunc()
   {
      genBlock(func->block);
   }

   //
   // Info::genFunc
   //
   void Info::genFunc(GDCC::IR::Function &func_)
   {
      TryPointer(genFunc, func);
   }

   //
   // Info::genSpace
   //
   void Info::genSpace(GDCC::IR::Space &space_)
   {
      TryPointer(genSpace, space);
   }

   //
   // Info::genStmnt
   //
   void Info::genStmnt(GDCC::IR::Statement &stmnt_)
   {
      TryPointer(genStmnt, stmnt);
   }

   //
   // Info::genStr
   //
   void Info::genStr(GDCC::IR::StrEnt &str_)
   {
      TryPointer(genStr, str);
   }

   //
   // Info::pre
   //
   void Info::pre()
   {
      for(auto &itr : GDCC::IR::Space::GblArs) preSpace(itr.second);
      for(auto &itr : GDCC::IR::Space::MapArs) preSpace(itr.second);
      for(auto &itr : GDCC::IR::Space::WldArs) preSpace(itr.second);

      preSpace(GDCC::IR::Space::GblReg);
      preSpace(GDCC::IR::Space::LocArs);
      preSpace(GDCC::IR::Space::MapReg);
      preSpace(GDCC::IR::Space::WldReg);

      for(auto &itr : GDCC::IR::FunctionRange())
         preFunc(itr.second);

      for(auto &itr : GDCC::IR::StrEntRange())
         preStr(itr.second);
   }

   //
   // Info::preBlock
   //
   void Info::preBlock()
   {
      for(auto &stmnt_ : *block)
         preStmnt(stmnt_);
   }

   //
   // Info::preBlock
   //
   void Info::preBlock(GDCC::IR::Block &block_)
   {
      TryPointer(preBlock, block);
   }

   //
   // Info::preFunc
   //
   void Info::preFunc()
   {
      preBlock(func->block);
   }

   //
   // Info::preFunc
   //
   void Info::preFunc(GDCC::IR::Function &func_)
   {
      TryPointer(preFunc, func);
   }

   //
   // Info::preSpace
   //
   void Info::preSpace(GDCC::IR::Space &space_)
   {
      TryPointer(preSpace, space);
   }

   //
   // Info::preStmnt
   //
   void Info::preStmnt(GDCC::IR::Statement &stmnt_)
   {
      TryPointer(preStmnt, stmnt);
   }

   //
   // Info::preStr
   //
   void Info::preStr(GDCC::IR::StrEnt &str_)
   {
      TryPointer(preStr, str);
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
   void Info::trBlock()
   {
      try
      {
         auto end   = static_cast<GDCC::IR::Statement *>(block->end());
              stmnt = static_cast<GDCC::IR::Statement *>(block->begin());
         for(; stmnt != end; stmnt = stmnt->next)
            trStmnt();
         stmnt = nullptr;
      }
      catch(...)
      {
         stmnt = nullptr;
         throw;
      }
   }

   //
   // Info::trBlock
   //
   void Info::trBlock(GDCC::IR::Block &block_)
   {
      TryPointer(trBlock, block);
   }

   //
   // Info::trFunc
   //
   void Info::trFunc()
   {
      trBlock(func->block);
   }

   //
   // Info::trFunc
   //
   void Info::trFunc(GDCC::IR::Function &func_)
   {
      TryPointer(trFunc, func);
   }

   //
   // Info::trSpace
   //
   void Info::trSpace(GDCC::IR::Space &space_)
   {
      TryPointer(trSpace, space);
   }

   //
   // Info::trStmnt
   //
   void Info::trStmnt(GDCC::IR::Statement &stmnt_)
   {
      TryPointer(trStmnt, stmnt);
   }

   //
   // Info::trStr
   //
   void Info::trStr(GDCC::IR::StrEnt &str_)
   {
      TryPointer(trStr, str);
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
   void Info::CheckArgB(GDCC::IR::Statement *stmnt, std::size_t a, GDCC::IR::ArgBase b)
   {
      if(stmnt->args[a].a != b)
      {
         std::cerr << "ERROR: " << stmnt->pos << ": " << stmnt->code
            << " must have " << b << " args[" << a << "]\n";
         throw EXIT_FAILURE;
      }
   }

   //
   // Info::CheckArgC
   //
   void Info::CheckArgC(GDCC::IR::Statement *stmnt, std::size_t c)
   {
      if(stmnt->args.size() < c)
      {
         std::cerr << "ERROR: " << stmnt->pos << ": bad argc for " << stmnt->code
            << ": " << stmnt->args.size() << " < " << c << '\n';
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

