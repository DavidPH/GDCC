//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Memory movement statement generation.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__GenStmnt__Move_H__
#define GDCC__SR__GenStmnt__Move_H__

#include "../../SR/Arg.hpp"
#include "../../SR/Exp.hpp"
#include "../../SR/Type.hpp"

#include "../../IR/Block.hpp"
#include "../../IR/Glyph.hpp"

#include "../../Platform/Platform.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_SR_GenGenStmnt_Move_GenArgPtr2
//
// Generates a GenStmnt_Move_GenArg specialization for an ArgPtr2-based Arg.
//
#define GDCC_SR_GenGenStmnt_Move_GenArgPtr2(ArgT) \
   template<> inline ArgT GenStmnt_Move_GenArg<ArgT>(Exp const *exp, \
      GenStmntCtx const &ctx, Arg const &arg, IR::Arg const &idx, Core::FastU off) \
   { \
      IR::Glyph glyph{&ctx.prog, arg.type->getQualAddr().name}; \
      auto arr = IR::ExpCreate_Glyph(glyph, exp->pos); \
      auto arrN = Platform::GetWordBytes(); \
      \
      return ArgT(arg.type->getSizeBytes(), IR::Arg_Lit(arrN, arr), idx, off); \
   } \
   \
   template<> inline ArgT GenStmnt_Move_GenArg<ArgT>(Exp const *exp, \
      GenStmntCtx const &ctx, Arg const &arg, IR::Arg &&idx, Core::FastU off) \
   { \
      IR::Glyph glyph{&ctx.prog, arg.type->getQualAddr().name}; \
      auto arr = IR::ExpCreate_Glyph(glyph, exp->pos); \
      auto arrN = Platform::GetWordBytes(); \
      \
      return ArgT(arg.type->getSizeBytes(), IR::Arg_Lit(arrN, arr), std::move(idx), off); \
   }


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace SR
   {
      //
      // GenStmnt_Move_GenArg
      //
      template<typename ArgT>
      ArgT GenStmnt_Move_GenArg(Exp const *, GenStmntCtx const &, Arg const &arg,
         IR::Arg const &idx, Core::FastU off)
      {
         return ArgT(arg.type->getSizeBytes(), idx, off);
      }
      template<typename ArgT>
      ArgT GenStmnt_Move_GenArg(Exp const *, GenStmntCtx const &, Arg const &arg,
         IR::Arg &&idx, Core::FastU off)
      {
         return ArgT(arg.type->getSizeBytes(), std::move(idx), off);
      }
      GDCC_SR_GenGenStmnt_Move_GenArgPtr2(IR::Arg_GblArr)
      GDCC_SR_GenGenStmnt_Move_GenArgPtr2(IR::Arg_HubArr)
      GDCC_SR_GenGenStmnt_Move_GenArgPtr2(IR::Arg_LocArr)
      GDCC_SR_GenGenStmnt_Move_GenArgPtr2(IR::Arg_ModArr)
      GDCC_SR_GenGenStmnt_Move_GenArgPtr2(IR::Arg_StrArr)
   }
}

#endif//GDCC__SR__GenStmnt__Move_H__

