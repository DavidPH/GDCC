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

#include "AST/GenStmnt/Move.hpp"

#include "AST/Temporary.hpp"

#include "Core/Exception.hpp"

#include "IR/ExpCode.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// GenStmnt_MovePartIdx
//
template<typename ArgT, typename IdxT>
static void GenStmnt_MovePartIdx(GDCC::AST::Exp const *exp,
   GDCC::AST::GenStmntCtx const &ctx, GDCC::AST::Arg const &arg,
   IdxT const &idx, bool get, bool set)
{
   using namespace GDCC;

   if(set)
   {
      for(Core::FastU n = arg.type->getSizeWords(); n--;)
         AST::GenStmnt_MoveWordSetT<ArgT>(exp, ctx, arg, idx, n);
   }

   if(get)
   {
      for(Core::FastU n = 0, e = arg.type->getSizeWords(); n != e; ++n)
         AST::GenStmnt_MoveWordGetT<ArgT>(exp, ctx, arg, idx, n);
   }
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

      // Use literal as index.
      GenStmnt_MovePartIdx<ArgT>(exp, ctx, arg,
         IR::Arg_Lit(arg.data->getIRExp()), get, set);

      return;
   }

   // If fetching or setting a single word, and not both, use address on stack.
   if(arg.type->getSizeWords() == 1 && get ^ set)
   {
      // Evaluate arg's data.
      arg.data->genStmntStk(ctx);

      // Use Stk as index.
      GenStmnt_MovePartIdx<ArgT>(exp, ctx, arg, IR::Arg_Stk(), get, set);

      return;
   }

   // As a last resort, just evaluate the pointer and store in a temporary.
   {
      // Evaluate arg's data.
      arg.data->genStmntStk(ctx);

      // Move to temporary.
      AST::Temporary tmp{ctx, exp->pos, arg.data->getType()->getSizeWords()};
      for(Core::FastU n = tmp.size(); n--;)
         ctx.block.addStatementArgs(IR::Code::Move_W,
            tmp.getArg(n), IR::Arg_Stk());

      // Use temporary as index.
      GenStmnt_MovePartIdx<ArgT>(exp, ctx, arg, tmp.getArg(), get, set);

      return;
   }
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
         arg.data->genStmnt(ctx);

         auto code = IR::ExpCode_Move(arg.type->getSizeWords());

         if(code == IR::Code::None)
         {
            std::cerr << "STUB: " << exp->pos << ": " << __FILE__ << ':'
               << __LINE__ << ": AddrBase::Lit get multi-word > 3\n";
            throw EXIT_FAILURE;
         }

         ctx.block.addStatementArgs(code, IR::Arg_Stk(), arg.data->getIRExp());
      }
      else
         arg.data->genStmnt(ctx, AST::Arg(arg.type, IR::AddrBase::Stk));
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
         Arg const &arg_, bool get, bool set)
      {
         auto arg = arg_;

         // Special handling of void arg.
         if(arg.type->isTypeVoid())
         {
            // A void src is an error.
            if(get) throw Core::ExceptStr(exp->pos, "void src");

            // A void dst is a no-op.
            if(set) return;
         }

         // Map from generic address space for codegen.
         if(arg.type->getQualAddr().base == IR::AddrBase::Gen)
            arg.type = arg.type->getTypeQual(IR::GetAddrGen());

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

