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

#include "AST/Exp.hpp"

#include "AST/Arg.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/Block.hpp"
#include "IR/Glyph.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GenGenArgPtr2
//
// Generates a GenArg specialization for an ArgPtr2-based Arg.
//
#define GenGenArgPtr2(ArgT) \
template<> ArgT GenArg<ArgT>(GDCC::AST::Exp const *exp, \
   GDCC::AST::GenStmntCtx const &ctx, GDCC::AST::Arg const &arg, \
   GDCC::IR::Arg &&idx, GDCC::IR::Exp const *off) \
{ \
   GDCC::IR::Glyph glyph{&ctx.prog, arg.type->getQualAddr().name}; \
   auto arr = GDCC::IR::ExpCreate_ValueGlyph(glyph, exp->pos); \
   \
   return ArgT(GDCC::IR::Arg_Lit(arr), std::move(idx), off); \
}


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// GenArg
//
template<typename ArgT>
static ArgT GenArg(GDCC::AST::Exp const *, GDCC::AST::GenStmntCtx const &,
   GDCC::AST::Arg const &, GDCC::IR::Arg &&idx, GDCC::IR::Exp const *off)
{
   return ArgT(std::move(idx), off);
}
GenGenArgPtr2(GDCC::IR::Arg_GblArr)
GenGenArgPtr2(GDCC::IR::Arg_MapArr)
GenGenArgPtr2(GDCC::IR::Arg_StrArr)
GenGenArgPtr2(GDCC::IR::Arg_WldArr)

//
// GenStmnt_MoveWordGet
//
template<typename ArgT>
static void GenStmnt_MoveWordGet(GDCC::AST::Exp const *exp,
   GDCC::AST::GenStmntCtx const &ctx, GDCC::AST::Arg const &arg,
   GDCC::IR::Arg &&idx, GDCC::Core::FastU off)
{
   using namespace GDCC;

   // Convert offset to an IR expression.
   auto offExp = IR::ExpCreate_ValueRoot(IR::Value_Fixed(off,
      IR::Type_Fixed(32, 0, false, false)), exp->pos);

   ctx.block.addStatementArgs(IR::Code::Move_W,
      IR::Arg_Stk(), GenArg<ArgT>(exp, ctx, arg, std::move(idx), offExp));
}

//
// GenStmnt_MoveWordSet
//
template<typename ArgT>
static void GenStmnt_MoveWordSet(GDCC::AST::Exp const *exp,
   GDCC::AST::GenStmntCtx const &ctx, GDCC::AST::Arg const &arg,
   GDCC::IR::Arg &&idx, GDCC::Core::FastU off)
{
   using namespace GDCC;

   // Convert offset to an IR expression.
   auto offExp = IR::ExpCreate_ValueRoot(IR::Value_Fixed(off,
      IR::Type_Fixed(32, 0, false, false)), exp->pos);

   ctx.block.addStatementArgs(IR::Code::Move_W,
      GenArg<ArgT>(exp, ctx, arg, std::move(idx), offExp), IR::Arg_Stk());
}

//
// GenStmnt_MovePartT
//
template<typename ArgT>
static void GenStmnt_MovePartT(GDCC::AST::Exp const *exp,
   GDCC::AST::GenStmntCtx const &ctx, GDCC::AST::Arg const &arg,
   bool get, bool set)
{
   using namespace GDCC;

   // If arg address is a constant, then use Arg_Lit address.
   if(arg.data->isIRExp())
   {
      // Evaluate arg's data for side effects.
      arg.data->genStmnt(ctx);

      IR::Arg_Lit idx{arg.data->getIRExp()};

      if(set)
      {
         for(Core::FastU n = arg.type->getSizeWords(); n--;)
            GenStmnt_MoveWordSet<ArgT>(exp, ctx, arg, idx, n);
      }

      if(get)
      {
         for(Core::FastU n = 0, e = arg.type->getSizeWords(); n != e; ++n)
            GenStmnt_MoveWordGet<ArgT>(exp, ctx, arg, idx, n);
      }

      return;
   }

   throw Core::ExceptStr(exp->pos, "non-constant address stub");
}

//
// GenStmnt_MovePartT<IR::Arg_Cpy>
//
template<> void GenStmnt_MovePartT<GDCC::IR::Arg_Cpy>(
   GDCC::AST::Exp const *exp, GDCC::AST::GenStmntCtx const &,
   GDCC::AST::Arg const &, bool get, bool set)
{
   if(set) throw GDCC::Core::ExceptStr(exp->pos, "AddrBase::Cpy set");
   if(get) throw GDCC::Core::ExceptStr(exp->pos, "AddrBase::Cpy get");
}

//
// GenStmnt_MovePartT<IR::Arg_Lit>
//
template<> void GenStmnt_MovePartT<GDCC::IR::Arg_Lit>(
   GDCC::AST::Exp const *exp, GDCC::AST::GenStmntCtx const &ctx,
   GDCC::AST::Arg const &arg, bool get, bool set)
{
   using namespace GDCC;

   if(set) throw Core::ExceptStr(exp->pos, "AddrBase::Lit set");

   if(get)
   {
      if(arg.data->isIRExp())
      {
         if(arg.type->getSizeWords() == 1)
         {
            ctx.block.addStatementArgs(IR::Code::Move_W, IR::Arg_Stk(),
               IR::Arg_Lit(arg.data->getIRExp()));
         }
         else
         {
            std::cerr << "STUB: " << exp->pos << ": " << __FILE__ << ':'
               << __LINE__ << ": AddrBase::Lit get multi-word\n";
            throw EXIT_FAILURE;
         }
      }
      else
      {
         arg.data->genStmnt(ctx, AST::Arg(arg.type, IR::AddrBase::Stk));
      }
   }
}

//
// GenStmnt_MovePartT<IR::Arg_Nul>
//
template<> void GenStmnt_MovePartT<GDCC::IR::Arg_Nul>(
   GDCC::AST::Exp const *exp, GDCC::AST::GenStmntCtx const &ctx,
   GDCC::AST::Arg const &arg, bool get, bool set)
{
   using namespace GDCC;

   if(set)
   {
      for(Core::FastU n = arg.type->getSizeWords(); n--;)
         ctx.block.addStatementArgs(IR::Code::Move_W,
            IR::Arg_Nul(), IR::Arg_Stk());
   }

   if(get) throw Core::ExceptStr(exp->pos, "AddrBase::Nul get");
}

//
// GenStmnt_MovePartT<IR::Arg_Stk>
//
template<> void GenStmnt_MovePartT<GDCC::IR::Arg_Stk>(
   GDCC::AST::Exp const *exp, GDCC::AST::GenStmntCtx const &,
   GDCC::AST::Arg const &, bool get, bool set)
{
   using namespace GDCC;

   if(get && set)
   {
      std::cerr << "STUB: " << exp->pos << ": " << __FILE__ << ':' << __LINE__
         << ": AddrBase::Stk get && set\n";
      throw EXIT_FAILURE;
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // GenStmnt_Move
      //
      void GenStmnt_Move(Exp const *exp, GenStmntCtx const &ctx,
         Arg const &dst, Arg const &src)
      {
         // Fall back to dumping full objects to stack.
         GenStmnt_MovePart(exp, ctx, src, true, false);
         GenStmnt_MovePart(exp, ctx, dst, false, true);
      }

      //
      // GenStmnt_Move
      //
      void GenStmnt_Move(Exp const *exp, GenStmntCtx const &ctx,
         Arg const &dst, Arg const &dup, Arg const &src)
      {
         // Fall back to dumping full objects to stack.
         GenStmnt_MovePart(exp, ctx, src, true, false);
         GenStmnt_MovePart(exp, ctx, dup, true, true);
         GenStmnt_MovePart(exp, ctx, dst, false, true);
      }

      //
      // GenStmnt_MovePart
      //
      void GenStmnt_MovePart(Exp const *exp, GenStmntCtx const &ctx,
         Arg const &arg, bool get, bool set)
      {
         // Special handling of void arg.
         if(arg.type->isTypeVoid())
         {
            // A void src is an error.
            if(get) throw Core::ExceptStr(exp->pos, "void src");

            // A void dst is a no-op.
            if(set) return;
         }

         switch(arg.type->getQualAddr().base)
         {
            #define GDCC_IR_AddrList(addr) \
            case IR::AddrBase::addr: \
               GenStmnt_MovePartT<IR::Arg_##addr>(exp, ctx, arg, get, set); \
               break;
            #include "IR/AddrList.hpp"
         }
      }
   }
}

// EOF

