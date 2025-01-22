//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2025 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Call statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "BC/ZDACS/Code.hpp"
#include "BC/ZDACS/Module.hpp"

#include "Core/Range.hpp"

#include "IR/Exception.hpp"
#include "IR/Function.hpp"

#include "Target/CallType.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   //
   // Info::chkStmnt_Call
   //
   void Info::chkStmnt_Call()
   {
      for(auto n = stmnt->args.size(); --n != 1;)
         chkStmntArgB(n, IR::ArgBase::Stk);

      switch(stmnt->args[1].a)
      {
      case IR::ArgBase::Lit:
      case IR::ArgBase::Stk:
         break;

      default:
         IR::ErrorCode(stmnt, "unsupported arg type");
      }
   }

   //
   // Info::chkStmnt_Casm
   //
   void Info::chkStmnt_Casm()
   {
      chkStmntArgB(1, IR::ArgBase::Lit);

      if(stmnt->args.size() > 2) switch(stmnt->args[2].a)
      {
      case IR::ArgBase::Lit:
         for(auto n = stmnt->args.size(); --n != 2;)
            chkStmntArgB(n, IR::ArgBase::Lit);
         break;

      case IR::ArgBase::Stk:
         for(auto n = stmnt->args.size(); --n != 2;)
            chkStmntArgB(n, IR::ArgBase::Stk);
         break;

      default:
         IR::ErrorCode(stmnt, "unsupported arg type");
      }

   }

   //
   // Info::chkStmnt_Cnat
   //
   void Info::chkStmnt_Cnat()
   {
      chkStmntArgB(1, IR::ArgBase::Lit);
      for(auto n = stmnt->args.size(); --n != 1;)
         chkStmntArgB(n, IR::ArgBase::Stk);
   }

   //
   // Info::chkStmnt_Cscr_IA
   //
   void Info::chkStmnt_Cscr_IA()
   {
      chkStmntArgB(1, IR::ArgBase::Stk);
      for(auto n = stmnt->args.size(); --n != 1;)
         chkStmntArgB(n, IR::ArgBase::Stk);
   }

   //
   // Info::chkStmnt_Cscr_IS
   //
   void Info::chkStmnt_Cscr_IS()
   {
      chkStmntArgB(1, IR::ArgBase::Stk);
      for(auto n = stmnt->args.size(); --n != 2;)
         chkStmntArgB(n, IR::ArgBase::Stk);
   }

   //
   // Info::chkStmnt_Cspe
   //
   void Info::chkStmnt_Cspe()
   {
      chkStmntArgB(1, IR::ArgBase::Lit);

      auto ret = stmnt->args[0].a == IR::ArgBase::Nul ? 0 : stmnt->args[0].getSize();

      if(ret > 1)
         IR::ErrorCode(stmnt, "unsupported ret");

      // Too many call args.
      if(getStmntSize() > 5)
         IR::ErrorCode(stmnt, "unsupported argc");

      if(stmnt->args.size() > 2) switch(stmnt->args[2].a)
      {
      case IR::ArgBase::Lit:
         for(auto n = stmnt->args.size(); --n != 2;)
            chkStmntArgB(n, IR::ArgBase::Lit);
         break;

      case IR::ArgBase::Stk:
         for(auto n = stmnt->args.size(); --n != 2;)
            chkStmntArgB(n, IR::ArgBase::Stk);
         break;

      default:
         IR::ErrorCode(stmnt, "unsupported arg type");
      }
   }

   //
   // Info::chkStmnt_Rjnk
   //
   void Info::chkStmnt_Rjnk()
   {
      if(!stmnt->args.empty())
         chkStmntArgB(0, IR::ArgBase::Lit);
   }

   //
   // Info::genStmnt_Call
   //
   void Info::genStmnt_Call()
   {
      auto argc = getStmntSize();
      auto retn = stmnt->args[0].getSize();

      genStmntDropParam(argc, GetParamMax(IR::CallType::StdCall));

      switch(stmnt->args[1].a)
      {
      case IR::ArgBase::Lit:
         genCode(retn ? Code::Call_Lit : Code::Call_Nul, stmnt->args[1].aLit.value);
         genStmntPushRetn(stmnt->args[0], GetRetnMax(IR::CallType::StdCall));
         break;

      case IR::ArgBase::Stk:
         genCode(Code::Call_Stk);

         if(retn)
            genStmntPushRetn(stmnt->args[0], GetRetnMax(IR::CallType::StdCall));
         else
            genCode(Code::Drop_Nul);

         break;

      default:
         Core::Error(stmnt->pos, "bad gen Call");
      }
   }

   //
   // Info::genStmnt_Casm
   //
   void Info::genStmnt_Casm()
   {
      std::vector<ElemArg> args;
      for(auto const &arg : Core::MakeRange(stmnt->args.begin() + 2, stmnt->args.end()))
      {
         if(arg.a == IR::ArgBase::Lit) for(Core::FastU i = 0; i != arg.aLit.size; ++i)
            args.emplace_back(arg.aLit.value, i);
      }
      // TODO 2024-12-28: This should not call getWord.
      genCode(static_cast<Code>(getWord(stmnt->args[1].aLit)), ElemArgs{args.begin(), args.end()});
   }

   //
   // Info::genStmnt_Cnat
   //
   void Info::genStmnt_Cnat()
   {
      genCode(Code::Cnat, getStmntSize(), stmnt->args[1].aLit.value);
   }

   //
   // Info::genStmnt_Cscr_IA
   //
   void Info::genStmnt_Cscr_IA()
   {
      auto argc = getStmntSize();
      auto argm = GetParamMax(IR::CallType::ScriptI);
      auto argn = argc < argm ? argc : argm;
      auto retn = stmnt->args[0].getSize();
      auto spec = 84; // ACS_ExecuteWithResult

      genStmntDropParam(argc, argm);

      if(retn) switch(argn)
      {
         // TODO 2024-12-28: Use a multi-literal push code.
      case  0: genCode(Code::Push_Lit, 0);
      case  1: genCode(Code::Push_Lit, 0);
      case  2: genCode(Code::Push_Lit, 0);
      case  3: genCode(Code::Push_Lit, 0);
      default: genCode(Code::Cspe_5R1, spec); break;
      }
      else switch(argn)
      {
      case  0: genCode(Code::Cspe_1, spec); break;
      case  1: genCode(Code::Cspe_2, spec); break;
      case  2: genCode(Code::Cspe_3, spec); break;
      case  3: genCode(Code::Cspe_4, spec); break;
      default: genCode(Code::Cspe_5, spec); break;
      }

      genStmntPushRetn(stmnt->args[0], GetRetnMax(IR::CallType::ScriptI));
   }

   //
   // Info::genStmnt_Cscr_IS
   //
   void Info::genStmnt_Cscr_IS()
   {
      auto argc = getStmntSize();
      auto argm = GetParamMax(IR::CallType::SScriptI);
      auto argn = argc < argm ? argc : argm;
      auto spec = 84; // ACS_ExecuteWithResult

      genStmntDropParam(argc, argm);

      // Clear returned flag.
      genCode(Code::Push_Lit, 0);
      genStmntDropArg(stmnt->args[2], 0);

      switch(argn)
      {
      case  0: genCode(Code::Cspe_1, spec); break;
      case  1: genCode(Code::Cspe_2, spec); break;
      case  2: genCode(Code::Cspe_3, spec); break;
      case  3: genCode(Code::Cspe_4, spec); break;
      default: genCode(Code::Cspe_5, spec); break;
      }

      genStmnt_Cscr_SS_post();
   }

   //
   // Info::genStmnt_Cscr_SA
   //
   void Info::genStmnt_Cscr_SA()
   {
      auto argc = getStmntSize();
      auto argm = GetParamMax(IR::CallType::ScriptS);
      auto retn = stmnt->args[0].getSize();
      auto spec = 44; // ACS_NamedExecuteWithResult

      genStmntDropParam(argc, argm);

      genCode(Code::Cnat, argc < argm ? argc + 1 : argm + 1, spec);

      if(retn)
         genStmntPushRetn(stmnt->args[0], GetRetnMax(IR::CallType::ScriptS));
      else
         genCode(Code::Drop_Nul);
   }

   //
   // Info::genStmnt_Cscr_SS
   //
   void Info::genStmnt_Cscr_SS()
   {
      auto argc = getStmntSize();
      auto argm = GetParamMax(IR::CallType::SScriptS);
      auto spec = 44; // ACS_NamedExecuteWithResult

      genStmntDropParam(argc, argm);

      // Clear returned flag.
      genCode(Code::Push_Lit, 0);
      genStmntDropArg(stmnt->args[2], 0);

      genCode(Code::Cnat, argc < argm ? argc + 1 : argm + 1, spec);
      genCode(Code::Drop_Nul);

      genStmnt_Cscr_SS_post();
   }

   //
   // Info::genStmnt_Cscr_SS_post
   //
   void Info::genStmnt_Cscr_SS_post()
   {
      auto retn = stmnt->args[0].getSize();

      // Save far jump state.
      genCode(Code::Push_Lit,    FarJumpIndex);
      genCode(Code::Push_GblArr, StaArray);
      genStmntDropTmp(0);
      genCode(Code::Push_Lit,    FarJumpIndex);
      genCode(Code::Push_Lit,    0);
      genCode(Code::Drop_GblArr, StaArray);

      // Check returned flag.
      auto jumpOpen = module->chunkCODE.size();
      genCode(Code::Jump_Lit, 0);
      auto jumpWait = getCodePos();
      genCode(Code::Wait_Lit, 1);
      module->chunkCODE[jumpOpen].args[0] = getCodePos();
      genStmntPushArg(stmnt->args[2], 0);
      genCode(Code::Jcnd_Nil, jumpWait);

      // Restore far jump state;
      genCode(Code::Push_Lit,    FarJumpIndex);
      genStmntPushTmp(0);
      genCode(Code::Drop_GblArr, StaArray);

      // Push any return words.
      if(retn)
      {
         if(stmnt->args[0].a != IR::ArgBase::Stk)
         {
            for(Core::FastU i = 0; i != retn; ++i)
            {
               genStmntDropArgPre(stmnt->args[0], i);
               genStmntPushArg(stmnt->args[2], i + 1);
               genStmntDropArgSuf(stmnt->args[0], i);
            }
         }
         else
            genStmntPushArg(stmnt->args[2], 1, 1 + retn);
      }
   }

   //
   // Info::genStmnt_Cspe
   //
   void Info::genStmnt_Cspe()
   {
      std::vector<ElemArg> args;
      auto argc = getStmntSize();
      auto retn = stmnt->args[0].a == IR::ArgBase::Nul ? 0 : stmnt->args[0].getSize();

      switch(stmnt->args.size() > 2 ? stmnt->args[2].a : IR::ArgBase::Lit)
      {
      case IR::ArgBase::Lit:
         if(retn)
         {
            for(auto const &arg : Core::MakeRange(stmnt->args.begin() + 2, stmnt->args.end()))
            {
               for(Core::FastU i = 0; i != arg.aLit.size; ++i)
                  genCode(Code::Push_Lit, {arg.aLit.value, i});
            }

            for(Core::FastU i = argc; i != 5; ++i)
               genCode(Code::Push_Lit, 0);

            genCode(Code::Cspe_5R1, stmnt->args[1].aLit.value);

            break;
         }

         args.emplace_back(stmnt->args[1].aLit.value);

         // Dummy arg.
         if(argc == 0)
            args.emplace_back(0);

         for(auto const &arg : Core::MakeRange(stmnt->args.begin() + 2, stmnt->args.end()))
            for(Core::FastU i = 0; i != arg.aLit.size; ++i)
               args.emplace_back(arg.aLit.value, i);

         switch(argc)
         {
         case 0: genCode(Code::Cspe_1L, {args.begin(), args.end()}); break;
         case 1: genCode(Code::Cspe_1L, {args.begin(), args.end()}); break;
         case 2: genCode(Code::Cspe_2L, {args.begin(), args.end()}); break;
         case 3: genCode(Code::Cspe_3L, {args.begin(), args.end()}); break;
         case 4: genCode(Code::Cspe_4L, {args.begin(), args.end()}); break;
         case 5: genCode(Code::Cspe_5L, {args.begin(), args.end()}); break;
         }

         break;

      case IR::ArgBase::Stk:
         if(retn)
         {
            switch(argc)
            {
            case 0: genCode(Code::Push_Lit, 0);
            case 1: genCode(Code::Push_Lit, 0);
            case 2: genCode(Code::Push_Lit, 0);
            case 3: genCode(Code::Push_Lit, 0);
            case 4: genCode(Code::Push_Lit, 0);
            case 5: genCode(Code::Cspe_5R1, stmnt->args[1].aLit.value); break;
            }
         }
         else
         {
            switch(argc)
            {
            case 0: genCode(Code::Push_Lit, 0);
            case 1: genCode(Code::Cspe_1, stmnt->args[1].aLit.value); break;
            case 2: genCode(Code::Cspe_2, stmnt->args[1].aLit.value); break;
            case 3: genCode(Code::Cspe_3, stmnt->args[1].aLit.value); break;
            case 4: genCode(Code::Cspe_4, stmnt->args[1].aLit.value); break;
            case 5: genCode(Code::Cspe_5, stmnt->args[1].aLit.value); break;
            }
         }

         break;

      default:
         Core::Error(stmnt->pos, "bad gen Cspe");
      }
   }

   //
   // Info::genStmnt_Retn
   //
   void Info::genStmnt_Retn()
   {
      auto retn = getStmntSize();

      if(func->allocAut)
      {
         genCode(Code::Push_LocReg, getStkPtrIdx());
         genCode(Code::Call_Nul,    "___GDCC__Plsf");
      }

      switch(func->ctype)
      {
      case IR::CallType::SScriptI:
      case IR::CallType::SScriptS:
         // Set return flag.
         genCode(Code::Push_LocReg, getStkPtrIdx());
         genCode(Code::Push_Lit,    1);
         genCode(Code::SubU);
         genCode(Code::Push_Lit,    1);
         genCode(Code::Drop_GblArr, StaArray);

         // Set return data.
         if(retn) for(Core::FastU i = retn; i--;)
         {
            genCode(Code::Push_LocReg, getStkPtrIdx());
            genCode(Code::Push_Lit,    i);
            genCode(Code::AddU);
            if(stmnt->args[0].a != IR::ArgBase::Stk)
               genStmntPushArg(stmnt->args[0], i);
            else
               genCode(Code::Swap);
            genCode(Code::Drop_GblArr, StaArray);
         }

         genCode(Code::Rscr);

         break;

      case IR::CallType::StdCall:
      case IR::CallType::StkCall:
         if(retn)
            genStmntDropRetn(stmnt->args[0], GetRetnMax(func->ctype));
         genCode(retn ? Code::Retn_Stk : Code::Retn_Nul);
         break;

      case IR::CallType::ScriptI:
      case IR::CallType::ScriptS:
         if(retn)
         {
            genStmntDropRetn(stmnt->args[0], GetRetnMax(func->ctype));
            genCode(Code::Drop_ScrRet);
         }
         genCode(Code::Rscr);
         break;

      default:
         Core::Error(stmnt->pos, "bad gen Retn");
      }
   }

   //
   // Info::genStmnt_Rjnk
   //
   void Info::genStmnt_Rjnk()
   {
      auto retm = GetRetnMax(func->ctype);
      auto retn = getStmntSize();

      if(func->allocAut)
      {
         genCode(Code::Push_LocReg, getStkPtrIdx());
         genCode(Code::Call_Nul,    "___GDCC__Plsf");
      }

      switch(func->ctype)
      {
      case IR::CallType::SScriptI:
      case IR::CallType::SScriptS:
         // Set return flag.
         genCode(Code::Push_LocReg, getStkPtrIdx());
         genCode(Code::Push_Lit,    1);
         genCode(Code::SubU);
         genCode(Code::Push_Lit,    1);
         genCode(Code::Drop_GblArr, StaArray);

         genCode(Code::Rscr);

         break;

      case IR::CallType::StdCall:
      case IR::CallType::StkCall:
         if(retn && retm)
         {
            for(Core::FastU i = 0, e = std::min(retn, retm) - 1; i != e; ++i)
               genCode(Code::Push_Lit, 0);
         }
         genCode(Code::Retn_Nul);
         break;

      case IR::CallType::ScriptI:
      case IR::CallType::ScriptS:
         genCode(Code::Rscr);
         break;

      default:
         Core::Error(stmnt->pos, "bad gen Rjnk");
      }
   }

   //
   // Info::preStmnt_Retn
   //
   void Info::preStmnt_Retn()
   {
      if(func->allocAut)
         preStmntCall("___GDCC__Plsf", 0, 1);
   }

   //
   // Info::trStmnt_Cspe
   //
   void Info::trStmnt_Cspe()
   {
      if(stmnt->args[0].a != IR::ArgBase::Nul)
         moveArgStk_dst(stmnt->args[0]);
   }

   //
   // Info::trStmnt_Retn
   //
   void Info::trStmnt_Retn()
   {
      if(stmnt->args.size() > 0 && !isPushArg(stmnt->args[0]))
         moveArgStk_src(stmnt->args[0]);
   }
}

// EOF

