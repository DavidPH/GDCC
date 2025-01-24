//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2025 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Memory movement statement generation.
//
//-----------------------------------------------------------------------------

#include "SR/GenStmnt/Move.hpp"

#include "SR/Temporary.hpp"

#include "Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::SR
{

   //
   // GenStmnt_MoveDstIdx
   //
   template<typename ArgT, typename IdxT>
   static void GenStmnt_MoveDstIdx(Exp const *exp, GenStmntCtx const &ctx,
      Arg const &dst, IdxT const &idx)
   {
      IR::Arg_Stk stk{dst.type->getSizeBytes()};

      ctx.block.addStmnt(IR::CodeBase::Move,
         GenStmnt_Move_GenArg<ArgT>(exp, ctx, dst, idx, 0), stk);
   }

   //
   // GenStmnt_MoveDupIdx
   //
   template<typename ArgT, typename IdxT>
   static void GenStmnt_MoveDupIdx(Exp const *exp, GenStmntCtx const &ctx,
      Arg const &dup, IdxT const &idx)
   {
      IR::Arg_Stk stk{dup.type->getSizeBytes()};

      ctx.block.addStmnt(IR::CodeBase::Move,
         GenStmnt_Move_GenArg<ArgT>(exp, ctx, dup, idx, 0), stk);

      ctx.block.addStmnt(IR::CodeBase::Move,
         stk, GenStmnt_Move_GenArg<ArgT>(exp, ctx, dup, idx, 0));
   }

   //
   // GenStmnt_MoveSrcIdx
   //
   template<typename ArgT, typename IdxT>
   static void GenStmnt_MoveSrcIdx(Exp const *exp, GenStmntCtx const &ctx,
      Arg const &src, IdxT const &idx)
   {
      IR::Arg_Stk stk{src.type->getSizeBytes()};

      ctx.block.addStmnt(IR::CodeBase::Move,
         stk, GenStmnt_Move_GenArg<ArgT>(exp, ctx, src, idx, 0));
   }

   //
   // GenStmnt_MoveDstPreT
   //
   template<typename ArgT>
   static void GenStmnt_MoveDstPreT(Exp const *, GenStmntCtx const &ctx, Arg const &dst)
   {
      // If arg address is a constant, then use Arg_Lit address.
      if(dst.data->isIRExp())
         return;

      // If arg address is an IR arg, use it.
      // Note that isIRArg implies a lack of side effects.
      if(dst.data->getArgSrc().isIRArg())
         return;

      // If setting a single word, use address on stack.
      if(dst.type->getSizeWords() == 1)
      {
         // Evaluate arg's data.
         if(Target::IsAddrFirst(IR::GetArgBase<ArgT>()))
            dst.data->genStmntStk(ctx);

         return;
      }

      // As a last resort, evaluate the pointer and store in a temporary.
   }

   //
   // GenStmnt_MoveDstSufT
   //
   template<typename ArgT>
   static void GenStmnt_MoveDstSufT(Exp const *exp, GenStmntCtx const &ctx, Arg const &dst)
   {
      // If arg address is a constant, then use Arg_Lit address.
      if(dst.data->isIRExp())
      {
         // Evaluate arg's data for side effects.
         dst.data->genStmnt(ctx);

         // Use literal as index.
         GenStmnt_MoveDstIdx<ArgT>(exp, ctx, dst, dst.data->getIRArgLit());

         return;
      }

      // If arg address is an IR arg, use it.
      // Note that isIRArg implies a lack of side effects.
      if(dst.data->getArgSrc().isIRArg())
      {
         GenStmnt_MoveDstIdx<ArgT>(exp, ctx, dst,
            dst.data->getArgSrc().getIRArg(ctx.prog));

         return;
      }

      // If setting a single word, use address on stack.
      if(dst.type->getSizeWords() == 1)
      {
         // Evaluate arg's data.
         if(!Target::IsAddrFirst(IR::GetArgBase<ArgT>()))
            dst.data->genStmntStk(ctx);

         // Use Stk as index.
         GenStmnt_MoveDstIdx<ArgT>(exp, ctx, dst, dst.data->getIRArgStk());

         return;
      }

      // As a last resort, evaluate the pointer and store in a temporary.
      {
         // Evaluate arg's data.
         dst.data->genStmntStk(ctx);

         // Move to temporary.
         Temporary tmp{ctx, exp->pos, dst.data->getType()->getSizeWords()};
         ctx.block.addStmnt(IR::CodeBase::Move, tmp.getArg(), tmp.getArgStk());

         // Use temporary as index.
         GenStmnt_MoveDstIdx<ArgT>(exp, ctx, dst, tmp.getArg());
      }
   }

   //
   // GenStmnt_MoveDupT
   //
   template<typename ArgT>
   static void GenStmnt_MoveDupT(Exp const *exp, GenStmntCtx const &ctx, Arg const &dup)
   {
      // If arg address is a constant, then use Arg_Lit address.
      if(dup.data->isIRExp())
      {
         // Evaluate arg's data for side effects.
         dup.data->genStmnt(ctx);

         // Use literal as index.
         GenStmnt_MoveDupIdx<ArgT>(exp, ctx, dup, dup.data->getIRArgLit());

         return;
      }

      // If arg address is an IR arg, use it.
      // Note that isIRArg implies a lack of side effects.
      if(dup.data->getArgSrc().isIRArg())
      {
         GenStmnt_MoveDupIdx<ArgT>(exp, ctx, dup,
            dup.data->getArgSrc().getIRArg(ctx.prog));

         return;
      }

      // As a last resort, evaluate the pointer and store in a temporary.
      {
         // Evaluate arg's data.
         dup.data->genStmntStk(ctx);

         // Move to temporary.
         Temporary tmp{ctx, exp->pos, dup.data->getType()->getSizeWords()};
         ctx.block.addStmnt(IR::CodeBase::Move, tmp.getArg(), tmp.getArgStk());

         // Use temporary as index.
         GenStmnt_MoveDupIdx<ArgT>(exp, ctx, dup, tmp.getArg());
      }
   }

   //
   // GenStmnt_MoveSrcT
   //
   template<typename ArgT>
   static void GenStmnt_MoveSrcT(Exp const *exp, GenStmntCtx const &ctx, Arg const &src)
   {
      // If arg address is a constant, then use Arg_Lit address.
      if(src.data->isIRExp())
      {
         // Evaluate arg's data for side effects.
         src.data->genStmnt(ctx);

         // Use literal as index.
         GenStmnt_MoveSrcIdx<ArgT>(exp, ctx, src, src.data->getIRArgLit());

         return;
      }

      // If arg address is an IR arg, use it.
      // Note that isIRArg implies a lack of side effects.
      if(src.data->getArgSrc().isIRArg())
      {
         GenStmnt_MoveSrcIdx<ArgT>(exp, ctx, src,
            src.data->getArgSrc().getIRArg(ctx.prog));

         return;
      }

      // If fetching a single word, use address on stack.
      if(src.type->getSizeWords() == 1)
      {
         // Evaluate arg's data.
         src.data->genStmntStk(ctx);

         // Use Stk as index.
         GenStmnt_MoveSrcIdx<ArgT>(exp, ctx, src, src.data->getIRArgStk());

         return;
      }

      // As a last resort, evaluate the pointer and store in a temporary.
      {
         // Evaluate arg's data.
         src.data->genStmntStk(ctx);

         // Move to temporary.
         Temporary tmp{ctx, exp->pos, src.data->getType()->getSizeWords()};
         ctx.block.addStmnt(IR::CodeBase::Move, tmp.getArg(), tmp.getArgStk());

         // Use temporary as index.
         GenStmnt_MoveSrcIdx<ArgT>(exp, ctx, src, tmp.getArg());
      }
   }

   //
   // GenStmnt_MoveDstPreT<IR::Arg_Cpy>
   //
   template<> void GenStmnt_MoveDstPreT<IR::Arg_Cpy>(
      Exp const *exp, GenStmntCtx const &, Arg const &)
   {
      Core::Error(exp->pos, "AddrBase::Cpy dst");
   }

   //
   // GenStmnt_MoveDstSufT<IR::Arg_Cpy>
   //
   template<> void GenStmnt_MoveDstSufT<IR::Arg_Cpy>(
      Exp const *exp, GenStmntCtx const &, Arg const &)
   {
      Core::Error(exp->pos, "AddrBase::Cpy dst");
   }

   //
   // GenStmnt_MoveDupT<IR::Arg_Cpy>
   //
   template<> void GenStmnt_MoveDupT<IR::Arg_Cpy>(
      Exp const *exp, GenStmntCtx const &, Arg const &)
   {
      Core::Error(exp->pos, "AddrBase::Cpy dup");
   }

   //
   // GenStmnt_MoveSrcT<IR::Arg_Cpy>
   //
   template<> void GenStmnt_MoveSrcT<IR::Arg_Cpy>(
      Exp const *exp, GenStmntCtx const &, Arg const &)
   {
      Core::Error(exp->pos, "AddrBase::Cpy src");
   }

   //
   // GenStmnt_MoveDstPreT<IR::Arg_Lit>
   //
   template<> void GenStmnt_MoveDstPreT<IR::Arg_Lit>(
      Exp const *exp, GenStmntCtx const &, Arg const &)
   {
      Core::Error(exp->pos, "AddrBase::Lit dst");
   }

   //
   // GenStmnt_MoveDstSufT<IR::Arg_Lit>
   //
   template<> void GenStmnt_MoveDstSufT<IR::Arg_Lit>(
      Exp const *exp, GenStmntCtx const &, Arg const &)
   {
      Core::Error(exp->pos, "AddrBase::Lit dst");
   }

   //
   // GenStmnt_MoveDupT<IR::Arg_Lit>
   //
   template<> void GenStmnt_MoveDupT<IR::Arg_Lit>(
      Exp const *exp, GenStmntCtx const &, Arg const &)
   {
      Core::Error(exp->pos, "AddrBase::Lit dup");
   }

   //
   // GenStmnt_MoveSrcT<IR::Arg_Lit>
   //
   template<> void GenStmnt_MoveSrcT<IR::Arg_Lit>(
      Exp const *, GenStmntCtx const &ctx, Arg const &src)
   {
      if(src.data->isIRExp())
      {
         src.data->genStmnt(ctx);

         ctx.block.setArgSize(src.type->getSizeBytes()).addStmnt(
            IR::CodeBase::Move, IR::Block::Stk(), src.data->getIRExp());
      }
      else
         src.data->genStmnt(ctx, Arg{src.type, IR::AddrBase::Stk});
   }

   //
   // GenStmnt_MoveDstPreT<IR::Arg_Nul>
   //
   template<> void GenStmnt_MoveDstPreT<IR::Arg_Nul>(
      Exp const *, GenStmntCtx const &, Arg const &)
   {
   }

   //
   // GenStmnt_MoveDstSufT<IR::Arg_Nul>
   //
   template<> void GenStmnt_MoveDstSufT<IR::Arg_Nul>(
      Exp const *, GenStmntCtx const &ctx, Arg const &dst)
   {
      ctx.block.setArgSize(dst.type->getSizeBytes()).addStmnt(
         IR::CodeBase::Move, IR::Block::Nul(), IR::Block::Stk());
   }

   //
   // GenStmnt_MoveDupT<IR::Arg_Nul>
   //
   template<> void GenStmnt_MoveDupT<IR::Arg_Nul>(
      Exp const *exp, GenStmntCtx const &, Arg const &)
   {
      Core::Error(exp->pos, "AddrBase::Nul dup");
   }

   //
   // GenStmnt_MoveSrcT<IR::Arg_Nul>
   //
   template<> void GenStmnt_MoveSrcT<IR::Arg_Nul>(
      Exp const *exp, GenStmntCtx const &, Arg const &)
   {
      Core::Error(exp->pos, "AddrBase::Nul src");
   }

   //
   // GenStmnt_MoveDstPreT<IR::Arg_Stk>
   //
   template<> void GenStmnt_MoveDstPreT<IR::Arg_Stk>(
      Exp const *, GenStmntCtx const &, Arg const &)
   {
   }

   //
   // GenStmnt_MoveDstSufT<IR::Arg_Stk>
   //
   template<> void GenStmnt_MoveDstSufT<IR::Arg_Stk>(
      Exp const *, GenStmntCtx const &, Arg const &)
   {
   }

   //
   // GenStmnt_MoveDupT<IR::Arg_Stk>
   //
   template<> void GenStmnt_MoveDupT<IR::Arg_Stk>(
      Exp const *exp, GenStmntCtx const &, Arg const &)
   {
      Core::Error(exp->pos, "AddrBase::Stk dup");
   }

   //
   // GenStmnt_MoveSrcT<IR::Arg_Stk>
   //
   template<> void GenStmnt_MoveSrcT<IR::Arg_Stk>(
      Exp const *, GenStmntCtx const &, Arg const &)
   {
   }

   //
   // GenStmnt_MoveDstPreT<IR::Arg_Gen>
   //
   template<> void GenStmnt_MoveDstPreT<IR::Arg_Gen>(
      Exp const *exp, GenStmntCtx const &ctx, Arg const &dst)
   {
      // Map from generic address space for codegen.
      Arg arg{dst.type->getTypeQual(Target::GetAddrGen()), dst.data};

      switch(arg.type->getQualAddr().base)
      {
         #define GDCC_Target_AddrList(addr) \
         case IR::AddrBase::addr: \
            GenStmnt_MoveDstPreT<IR::Arg_##addr>(exp, ctx, arg); \
            break;
         #include "Target/AddrList.hpp"
      }
   }

   //
   // GenStmnt_MoveDstSufT<IR::Arg_Gen>
   //
   template<> void GenStmnt_MoveDstSufT<IR::Arg_Gen>(
      Exp const *exp, GenStmntCtx const &ctx, Arg const &dst)
   {
      // Map from generic address space for codegen.
      Arg arg{dst.type->getTypeQual(Target::GetAddrGen()), dst.data};

      switch(arg.type->getQualAddr().base)
      {
         #define GDCC_Target_AddrList(addr) \
         case IR::AddrBase::addr: \
            GenStmnt_MoveDstSufT<IR::Arg_##addr>(exp, ctx, arg); \
            break;
         #include "Target/AddrList.hpp"
      }
   }

   //
   // GenStmnt_MoveDupT<IR::Arg_Gen>
   //
   template<> void GenStmnt_MoveDupT<IR::Arg_Gen>(
      Exp const *exp, GenStmntCtx const &ctx, Arg const &dup)
   {
      // Map from generic address space for codegen.
      Arg arg{dup.type->getTypeQual(Target::GetAddrGen()), dup.data};

      switch(arg.type->getQualAddr().base)
      {
         #define GDCC_Target_AddrList(addr) \
         case IR::AddrBase::addr: \
            GenStmnt_MoveDupT<IR::Arg_##addr>(exp, ctx, arg); \
            break;
         #include "Target/AddrList.hpp"
      }
   }

   //
   // GenStmnt_MoveSrcT<IR::Arg_Gen>
   //
   template<> void GenStmnt_MoveSrcT<IR::Arg_Gen>(
      Exp const *exp, GenStmntCtx const &ctx, Arg const &src)
   {
      // Map from generic address space for codegen.
      Arg arg{src.type->getTypeQual(Target::GetAddrGen()), src.data};

      switch(arg.type->getQualAddr().base)
      {
         #define GDCC_Target_AddrList(addr) \
         case IR::AddrBase::addr: \
            GenStmnt_MoveSrcT<IR::Arg_##addr>(exp, ctx, arg); \
            break;
         #include "Target/AddrList.hpp"
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::SR
{
   //
   // GenStmnt_Move
   //
   void GenStmnt_Move(Exp const *exp, GenStmntCtx const &ctx,
      Arg const &dst, Arg const &src)
   {
      // Try to use IR args.
      if(dst.isIRArg() && src.isIRArg())
      {
         ctx.block.addStmnt(IR::CodeBase::Move,
            dst.getIRArg(ctx.prog), src.getIRArg(ctx.prog));

         return;
      }

      // Fall back to dumping full objects to stack.
      GenStmnt_MoveDstPre(exp, ctx, dst);
      GenStmnt_MoveSrc   (exp, ctx, src);
      GenStmnt_MoveDstSuf(exp, ctx, dst);
   }

   //
   // GenStmnt_Move
   //
   void GenStmnt_Move(Exp const *exp, GenStmntCtx const &ctx,
      Arg const &dst, Arg const &dup, Arg const &src)
   {
      // Try to use IR args.
      if(dst.isIRArg() && dup.isIRArg() && src.isIRArg())
      {
         auto dupIR = dup.getIRArg(ctx.prog);

         ctx.block.addStmnt(IR::CodeBase::Move,
            dupIR, src.getIRArg(ctx.prog));

         ctx.block.addStmnt(IR::CodeBase::Move,
            dst.getIRArg(ctx.prog), dupIR);

         return;
      }

      // Fall back to dumping full objects to stack.
      GenStmnt_MoveDstPre(exp, ctx, dst);
      GenStmnt_MoveSrc   (exp, ctx, src);
      GenStmnt_MoveDup   (exp, ctx, dup);
      GenStmnt_MoveDstSuf(exp, ctx, dst);
   }

   //
   // GenStmnt_MoveDstPre
   //
   void GenStmnt_MoveDstPre(Exp const *exp, GenStmntCtx const &ctx, Arg const &dst)
   {
      // A void dst is a no-op.
      if(dst.type->isTypeVoid())
         return;

      switch(dst.type->getQualAddr().base)
      {
         #define GDCC_Target_AddrList(addr) \
         case IR::AddrBase::addr: \
            GenStmnt_MoveDstPreT<IR::Arg_##addr>(exp, ctx, dst); \
            break;
         #include "Target/AddrList.hpp"
      }
   }

   //
   // GenStmnt_MoveDstSuf
   //
   void GenStmnt_MoveDstSuf(Exp const *exp, GenStmntCtx const &ctx, Arg const &dst)
   {
      // A void dst is a no-op.
      if(dst.type->isTypeVoid())
         return;

      switch(dst.type->getQualAddr().base)
      {
         #define GDCC_Target_AddrList(addr) \
         case IR::AddrBase::addr: \
            GenStmnt_MoveDstSufT<IR::Arg_##addr>(exp, ctx, dst); \
            break;
         #include "Target/AddrList.hpp"
      }
   }

   //
   // GenStmnt_MoveDup
   //
   void GenStmnt_MoveDup(Exp const *exp, GenStmntCtx const &ctx, Arg const &dup)
   {
      // A void src is an error.
      if(dup.type->isTypeVoid())
         Core::Error(exp->pos, "void dup");

      switch(dup.type->getQualAddr().base)
      {
         #define GDCC_Target_AddrList(addr) \
         case IR::AddrBase::addr: \
            GenStmnt_MoveDupT<IR::Arg_##addr>(exp, ctx, dup); \
            break;
         #include "Target/AddrList.hpp"
      }
   }

   //
   // GenStmnt_MoveSrc
   //
   void GenStmnt_MoveSrc(Exp const *exp, GenStmntCtx const &ctx, Arg const &src)
   {
      // A void src is an error.
      if(src.type->isTypeVoid())
         Core::Error(exp->pos, "void src");

      switch(src.type->getQualAddr().base)
      {
         #define GDCC_Target_AddrList(addr) \
         case IR::AddrBase::addr: \
            GenStmnt_MoveSrcT<IR::Arg_##addr>(exp, ctx, src); \
            break;
         #include "Target/AddrList.hpp"
      }
   }
}

// EOF

