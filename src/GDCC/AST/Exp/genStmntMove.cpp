//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// AST::Exp::genStmntMove and related functions implementation.
//
//-----------------------------------------------------------------------------

#include "../Exp.hpp"

#include "../Arg.hpp"
#include "../Type.hpp"

#include "GDCC/IR/Block.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp::genStmntMove
      //
      void Exp::genStmntMove(IR::Block &block, Function *fn, Arg const &dst,
         Arg const &src) const
      {
         genStmntMovePart(block, fn, src, true, false);
         genStmntMovePart(block, fn, dst, false, true);
      }

      //
      // Exp::genStmntMovePart
      //
      void Exp::genStmntMovePart(IR::Block &block, Function *fn, Arg const &arg,
         bool get, bool set) const
      {
         //
         // getOffset
         //
         auto getOffset = [&](FastU off)
         {
            return IR::ExpCreate_ValueRoot(IR::Value_Fixed(off,
               IR::Type_Fixed(32, 0, false, false)), pos);
         };

         IR::Exp::CPtr argIRExp;

         switch(arg.type->getQualAddr().base)
         {
            //
            // Lit
            //
         case IR::AddrBase::Lit:
            if(set)
            {
               std::cerr << "ERROR: " << pos << ": AddrBase::Lit set\n";
               throw EXIT_FAILURE;
            }

            if(get)
            {
               if(arg.data->isIRExp())
               {
                  if(arg.type->getSizeWords() == 1)
                  {
                     block.addStatementArgs(IR::Code::Move_W, IR::Arg_Stk(),
                        IR::Arg_Lit(arg.data->getIRExp()));
                  }
                  else
                  {
                     std::cerr << "STUB: " << pos << ": " << __FILE__ << ':'
                        << __LINE__ << ": AddrBase::Lit get multi-word\n";
                     throw EXIT_FAILURE;
                  }
               }
               else
               {
                  arg.data->genStmnt(block, fn, Arg(arg.type, IR::AddrBase::Stk));
               }
            }

            break;

            //
            // Nul
            //
         case IR::AddrBase::Nul:
            if(set)
            {
               for(FastU n = arg.type->getSizeWords(); n--;)
                  block.addStatementArgs(IR::Code::Move_W, IR::Arg_Nul(), IR::Arg_Stk());
            }

            if(get)
            {
               std::cerr << "ERROR: " << pos << ": AddrBase::Nul get\n";
               throw EXIT_FAILURE;
            }

            break;

            //
            // Stk
            //
         case IR::AddrBase::Stk:
            if(get && set)
            {
               std::cerr << "STUB: " << pos << ": " << __FILE__ << ':' << __LINE__
                  << ": AddrBase::Stk get && set\n";
               throw EXIT_FAILURE;
            }

            break;

            //
            // LocReg
            //
         case IR::AddrBase::LocReg:
            argIRExp = arg.data->getIRExp();

            if(set)
            {
               for(FastU n = arg.type->getSizeWords(); n--;)
               {
                  block.addStatementArgs(IR::Code::Move_W,
                     IR::Arg_LocReg(IR::Arg_Lit(argIRExp), getOffset(n)),
                     IR::Arg_Stk());
               }
            }

            if(get)
            {
               for(FastU n = 0, e = arg.type->getSizeWords(); n != e; ++n)
               {
                  block.addStatementArgs(IR::Code::Move_W,
                     IR::Arg_Stk(),
                     IR::Arg_LocReg(IR::Arg_Lit(argIRExp), getOffset(n)));
               }
            }

            break;

         default:
            std::cerr << "STUB: " << pos << ": " << __FILE__ << ':' << __LINE__ << '\n';
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

