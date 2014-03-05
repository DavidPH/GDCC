//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C binary "operator *" integer expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Mul.hpp"

#include "AST/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/Block.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_MulInteg::v_genStmnt
      //
      void Exp_MulInteg::v_genStmnt(IR::Block &block, AST::Function *fn,
         AST::Arg const &dst) const
      {
         if(tryGenStmntNul(block, fn, dst)) return;

         // Evaluate both sub-expressions to stack.
         expL->genStmntStk(block, fn);
         expR->genStmntStk(block, fn);

         // Multiply on stack.
         auto sign = type->getSizeBitsS();
         IR::Code code;
         switch(type->getSizeWords())
         {
         case 1: code = sign ? IR::Code::MulI_W : IR::Code::MulU_W; break;
         default:
            throw Core::ExceptStr(pos, "unsupported word count");
         }
         block.addStatementArgs(code,
            IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());

         // Move to destination.
         genStmntMovePart(block, fn, dst, false, true);
      }
   }
}

// EOF

