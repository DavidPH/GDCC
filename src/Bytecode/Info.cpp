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

#include "Bytecode/Info.hpp"

#include "IR/Program.hpp"

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

namespace GDCC
{
   namespace Bytecode
   {
      //
      // Info::gen
      //
      void Info::gen()
      {
         for(auto &itr : prog->rangeSpaceGblArs()) genSpace(itr);
         for(auto &itr : prog->rangeSpaceMapArs()) genSpace(itr);
         for(auto &itr : prog->rangeSpaceWldArs()) genSpace(itr);

         genSpace(prog->getSpaceGblReg());
         genSpace(prog->getSpaceLocArs());
         genSpace(prog->getSpaceMapReg());
         genSpace(prog->getSpaceWldReg());

         for(auto &itr : prog->rangeFunction())
            genFunc(itr);

         for(auto &itr : prog->rangeStrEnt())
            genStr(itr);
      }

      //
      // Info::gen
      //
      void Info::gen(IR::Program &prog_)
      {
         TryPointer(gen, prog);
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
      void Info::genBlock(IR::Block &block_)
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
      void Info::genFunc(IR::Function &func_)
      {
         TryPointer(genFunc, func);
      }

      //
      // Info::genSpace
      //
      void Info::genSpace(IR::Space &space_)
      {
         TryPointer(genSpace, space);
      }

      //
      // Info::genStmnt
      //
      void Info::genStmnt(IR::Statement &stmnt_)
      {
         TryPointer(genStmnt, stmnt);
      }

      //
      // Info::genStr
      //
      void Info::genStr(IR::StrEnt &str_)
      {
         TryPointer(genStr, str);
      }

      //
      // Info::pre
      //
      void Info::pre()
      {
         for(auto &itr : prog->rangeSpaceGblArs()) preSpace(itr);
         for(auto &itr : prog->rangeSpaceMapArs()) preSpace(itr);
         for(auto &itr : prog->rangeSpaceWldArs()) preSpace(itr);

         preSpace(prog->getSpaceGblReg());
         preSpace(prog->getSpaceLocArs());
         preSpace(prog->getSpaceMapReg());
         preSpace(prog->getSpaceWldReg());

         for(auto &itr : prog->rangeFunction())
            preFunc(itr);

         for(auto &itr : prog->rangeStrEnt())
            preStr(itr);
      }

      //
      // Info::pre
      //
      void Info::pre(IR::Program &prog_)
      {
         TryPointer(pre, prog);
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
      void Info::preBlock(IR::Block &block_)
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
      void Info::preFunc(IR::Function &func_)
      {
         TryPointer(preFunc, func);
      }

      //
      // Info::preSpace
      //
      void Info::preSpace(IR::Space &space_)
      {
         TryPointer(preSpace, space);
      }

      //
      // Info::preStmnt
      //
      void Info::preStmnt(IR::Statement &stmnt_)
      {
         TryPointer(preStmnt, stmnt);
      }

      //
      // Info::preStr
      //
      void Info::preStr(IR::StrEnt &str_)
      {
         TryPointer(preStr, str);
      }

      //
      // Info::put
      //
      void Info::put(IR::Program &prog_, std::ostream &out_)
      {
         try
         {
            out  = &out_;
            prog = &prog_;

            put();

            out  = nullptr;
            prog = nullptr;
         }
         catch(...)
         {
            out  = nullptr;
            prog = nullptr;
            throw;
         }
      }

      //
      // Info::tr
      //
      void Info::tr()
      {
         for(auto &itr : prog->rangeSpaceGblArs()) trSpace(itr);
         for(auto &itr : prog->rangeSpaceMapArs()) trSpace(itr);
         for(auto &itr : prog->rangeSpaceWldArs()) trSpace(itr);

         trSpace(prog->getSpaceGblReg());
         trSpace(prog->getSpaceLocArs());
         trSpace(prog->getSpaceMapReg());
         trSpace(prog->getSpaceWldReg());

         for(auto &itr : prog->rangeFunction())
            trFunc(itr);

         for(auto &itr : prog->rangeStrEnt())
            trStr(itr);
      }

      //
      // Info::tr
      //
      void Info::tr(IR::Program &prog_)
      {
         TryPointer(tr, prog);
      }

      //
      // Info::trBlock
      //
      void Info::trBlock()
      {
         try
         {
            auto end   = static_cast<IR::Statement *>(block->end());
               stmnt = static_cast<IR::Statement *>(block->begin());
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
      void Info::trBlock(IR::Block &block_)
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
      void Info::trFunc(IR::Function &func_)
      {
         TryPointer(trFunc, func);
      }

      //
      // Info::trSpace
      //
      void Info::trSpace(IR::Space &space_)
      {
         TryPointer(trSpace, space);
      }

      //
      // Info::trStmnt
      //
      void Info::trStmnt(IR::Statement &stmnt_)
      {
         TryPointer(trStmnt, stmnt);
      }

      //
      // Info::trStr
      //
      void Info::trStr(IR::StrEnt &str_)
      {
         TryPointer(trStr, str);
      }

      //
      // Info::CheckArgB
      //
      void Info::CheckArgB(IR::Arg const &arg, IR::AddrBase b, Core::Origin pos)
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
      void Info::CheckArgB(IR::Statement *stmnt, std::size_t a, IR::ArgBase b)
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
      void Info::CheckArgC(IR::Statement *stmnt, std::size_t c)
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
      bool Info::IsExp0(IR::Exp const *exp)
      {
         if(!exp) return true;

         auto val = exp->getValue();

         switch(val.v)
         {
         case IR::ValueBase::Fixed:
            return !val.vFixed.value;

         default:
            return false;
         }
      }
   }
}

// EOF

