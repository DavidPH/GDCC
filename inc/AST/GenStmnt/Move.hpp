//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Memory movement statement generation.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__GenStmnt__Move_H__
#define GDCC__AST__GenStmnt__Move_H__

#include "../../AST/Arg.hpp"
#include "../../AST/Exp.hpp"
#include "../../AST/Type.hpp"

#include "../../IR/Block.hpp"
#include "../../IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_AST_GenGenStmnt_Move_GenArgPtr2
//
// Generates a GenStmnt_Move_GenArg specialization for an ArgPtr2-based Arg.
//
#define GDCC_AST_GenGenStmnt_Move_GenArgPtr2(ArgT) \
template<> inline ArgT GenStmnt_Move_GenArg<ArgT>(Exp const *exp, \
   GenStmntCtx const &ctx, Arg const &arg, IR::Arg &&idx, IR::Exp const *off) \
{ \
   IR::Glyph glyph{&ctx.prog, arg.type->getQualAddr().name}; \
   auto arr = IR::ExpCreate_ValueGlyph(glyph, exp->pos); \
   \
   return ArgT(IR::Arg_Lit(arr), std::move(idx), off); \
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // GenStmnt_Move_GenArg
      //
      template<typename ArgT>
      ArgT GenStmnt_Move_GenArg(Exp const *, GenStmntCtx const &, Arg const &,
         IR::Arg &&idx, IR::Exp const *off)
      {
         return ArgT(std::move(idx), off);
      }
      GDCC_AST_GenGenStmnt_Move_GenArgPtr2(IR::Arg_GblArr)
      GDCC_AST_GenGenStmnt_Move_GenArgPtr2(IR::Arg_MapArr)
      GDCC_AST_GenGenStmnt_Move_GenArgPtr2(IR::Arg_StrArr)
      GDCC_AST_GenGenStmnt_Move_GenArgPtr2(IR::Arg_WldArr)

      //
      // GenStmnt_MoveWordGetT
      //
      template<typename ArgT>
      void GenStmnt_MoveWordGetT(Exp const *exp, GenStmntCtx const &ctx,
         Arg const &arg, IR::Arg &&idx, Core::FastU off)
      {
         // Convert offset to an IR expression.
         auto offExp = IR::ExpCreate_ValueRoot(IR::Value_Fixed(off,
            IR::Type_Fixed(32, 0, false, false)), exp->pos);

         ctx.block.addStatementArgs(IR::Code::Move_W,
            IR::Arg_Stk(),
            GenStmnt_Move_GenArg<ArgT>(exp, ctx, arg, std::move(idx), offExp));
      }

      //
      // GenStmnt_MoveWordSetT
      //
      template<typename ArgT>
      void GenStmnt_MoveWordSetT(Exp const *exp, GenStmntCtx const &ctx,
         Arg const &arg, IR::Arg &&idx, Core::FastU off)
      {
         // Convert offset to an IR expression.
         auto offExp = IR::ExpCreate_ValueRoot(IR::Value_Fixed(off,
            IR::Type_Fixed(32, 0, false, false)), exp->pos);

         ctx.block.addStatementArgs(IR::Code::Move_W,
            GenStmnt_Move_GenArg<ArgT>(exp, ctx, arg, std::move(idx), offExp),
            IR::Arg_Stk());
      }
   }
}

#endif//GDCC__AST__GenStmnt__Move_H__

