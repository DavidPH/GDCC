//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
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
   // Info::genStmnt_Call
   //
   void Info::genStmnt_Call()
   {
      auto argc = getStmntSize();
      auto argm = GetParamMax(IR::CallType::StdCall);
      auto ret  = stmnt->args[0].getSize();

      if(argc > argm)
         numChunkCODE += (argc - argm) * 20;

      switch(stmnt->args[1].a)
      {
      case IR::ArgBase::Lit:
         if(ret == 0)
            numChunkCODE += 8;
         else
            numChunkCODE += 8 + (ret - 1) * 16;

         break;

      case IR::ArgBase::Stk:
         if(ret == 0)
            numChunkCODE += 8;
         else
            numChunkCODE += 4 + (ret - 1) * 16;

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
      numChunkCODE += 4;
      for(auto const &arg : Core::MakeRange(stmnt->args.begin() + 2, stmnt->args.end()))
      {
         if(arg.a == IR::ArgBase::Lit)
            numChunkCODE += arg.aLit.size * 4;
      }
   }

   //
   // Info::genStmnt_Cnat
   //
   void Info::genStmnt_Cnat()
   {
      numChunkCODE += 12;
   }

   //
   // Info::genStmnt_Cscr_IA
   //
   void Info::genStmnt_Cscr_IA()
   {
      auto argc = getStmntSize();
      auto argm = GetParamMax(IR::CallType::ScriptI);
      auto argn = argc < argm ? argc : argm;
      auto ret  = stmnt->args[0].getSize();

      if(argc > argm)
         numChunkCODE += (argc - argm) * 20;

      if(ret && argn < 4)
         numChunkCODE += (4 - argn) * 8;

      numChunkCODE += 8;

      if(ret)
         numChunkCODE += (ret - 1) * 16;
   }

   //
   // Info::genStmnt_Cscr_IS
   //
   void Info::genStmnt_Cscr_IS()
   {
      auto argc = getStmntSize();
      auto argm = GetParamMax(IR::CallType::SScriptI);
      auto ret  = stmnt->args[0].getSize();

      if(argc > argm)
         numChunkCODE += (argc - argm) * 20;

      numChunkCODE += 8 + 8 + 24
         + lenDropArg(stmnt->args[2], 0)
         + lenPushArg(stmnt->args[2], 0);

      if(ret)
         numChunkCODE += lenPushArg(stmnt->args[2], 1, 1 + ret);
   }

   //
   // Info::genStmnt_Cscr_SA
   //
   void Info::genStmnt_Cscr_SA()
   {
      auto argc = getStmntSize();
      auto argm = GetParamMax(IR::CallType::ScriptS);
      auto ret  = stmnt->args[0].getSize();

      if(argc > argm)
         numChunkCODE += (argc - argm) * 20;

      numChunkCODE += 12;

      if(ret)
         numChunkCODE += (ret - 1) * 16;
      else
         numChunkCODE += 4;
   }

   //
   // Info::genStmnt_Cscr_SS
   //
   void Info::genStmnt_Cscr_SS()
   {
      auto argc = getStmntSize();
      auto argm = GetParamMax(IR::CallType::SScriptS);
      auto ret  = stmnt->args[0].getSize();

      if(argc > argm)
         numChunkCODE += (argc - argm) * 20;

      numChunkCODE += 48
         + lenDropArg(stmnt->args[2], 0)
         + lenPushArg(stmnt->args[2], 0);

      if(ret)
         numChunkCODE += lenPushArg(stmnt->args[2], 1, 1 + ret);
   }

   //
   // Info::genStmnt_Cspe
   //
   void Info::genStmnt_Cspe()
   {
      auto argc = getStmntSize();
      auto ret  = stmnt->args[0].a == IR::ArgBase::Nul ? 0 : stmnt->args[0].getSize();

      // No call args.
      if(argc == 0)
      {
         numChunkCODE += ret ? 48 : 12;
         return;
      }

      switch(stmnt->args[2].a)
      {
      case IR::ArgBase::Lit:
         if(ret)
            numChunkCODE += 48;
         else
            numChunkCODE += 8 + argc * 4;
         break;

      case IR::ArgBase::Stk:
         numChunkCODE += 8;

         // Dummy args.
         if(ret) numChunkCODE += (5 - argc) * 8;

         break;

      default:
         Core::Error(stmnt->pos, "bad gen Cspr");
      }
   }

   //
   // Info::genStmnt_Retn
   //
   void Info::genStmnt_Retn()
   {
      auto ret = getStmntSize();

      if(func->allocAut)
         numChunkCODE += 16;

      switch(func->ctype)
      {
      case IR::CallType::SScriptI:
      case IR::CallType::SScriptS:
         numChunkCODE += 40 + ret * 32;
         break;

      case IR::CallType::StdCall:
      case IR::CallType::StkCall:
         if(ret == 0)
            numChunkCODE += 4;
         else
            numChunkCODE += 4 + (ret - 1) * 20;
         break;

      case IR::CallType::ScriptI:
      case IR::CallType::ScriptS:
         if(ret == 0)
            numChunkCODE += 4;
         else
            numChunkCODE += 8 + (ret - 1) * 20;
         break;

      default:
         Core::Error(stmnt->pos, "bad gen Retn");
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
   // Info::putStmnt_Call
   //
   void Info::putStmnt_Call()
   {
      auto argc = getStmntSize();
      auto argm = GetParamMax(IR::CallType::StdCall);
      auto ret  = stmnt->args[0].getSize();

      if(argc > argm)
         putStmntDropRetn(argc - argm);

      switch(stmnt->args[1].a)
      {
      case IR::ArgBase::Lit:
         if(ret == 0)
            putCode(Code::Call_Nul);
         else
            putCode(Code::Call_Lit);

         putWord(getWord(stmnt->args[1].aLit));

         if(ret)
            putStmntPushRetn(ret - 1);

         break;

      case IR::ArgBase::Stk:
         putCode(Code::Call_Stk);

         if(ret == 0)
            putCode(Code::Drop_Nul);
         else
            putStmntPushRetn(ret - 1);

         break;

      default:
         Core::Error(stmnt->pos, "bad put Call");
      }
   }

   //
   // Info::putStmnt_Casm
   //
   void Info::putStmnt_Casm()
   {
      putWord(getWord(stmnt->args[1].aLit));
      for(auto const &arg : Core::MakeRange(stmnt->args.begin() + 2, stmnt->args.end()))
      {
         if(arg.a == IR::ArgBase::Lit) for(Core::FastU i = 0; i != arg.aLit.size; ++i)
            putWord(getWord(arg.aLit, i));
      }
   }

   //
   // Info::putStmnt_Cnat
   //
   void Info::putStmnt_Cnat()
   {
      putCode(Code::Cnat);
      putWord(getStmntSize());
      putWord(getWord(stmnt->args[1].aLit));
   }

   //
   // Info::putStmnt_Cscr_IA
   //
   void Info::putStmnt_Cscr_IA()
   {
      auto argc = getStmntSize();
      auto argm = GetParamMax(IR::CallType::ScriptI);
      auto argn = argc < argm ? argc : argm;
      auto ret  = stmnt->args[0].getSize();

      if(argc > argm)
         putStmntDropRetn(argc - argm);

      if(ret) switch(argn)
      {
      case  0: putCode(Code::Push_Lit, 0);
      case  1: putCode(Code::Push_Lit, 0);
      case  2: putCode(Code::Push_Lit, 0);
      case  3: putCode(Code::Push_Lit, 0);
      default: putCode(Code::Cspe_5R1); break;
      }
      else switch(argn)
      {
      case  0: putCode(Code::Cspe_1); break;
      case  1: putCode(Code::Cspe_2); break;
      case  2: putCode(Code::Cspe_3); break;
      case  3: putCode(Code::Cspe_4); break;
      default: putCode(Code::Cspe_5); break;
      }

      putWord(84); // ACS_ExecuteWithResult

      if(ret)
         putStmntPushRetn(ret - 1);
   }

   //
   // Info::putStmnt_Cscr_IS
   //
   void Info::putStmnt_Cscr_IS()
   {
      auto argc = getStmntSize();
      auto argm = GetParamMax(IR::CallType::SScriptI);
      auto argn = argc < argm ? argc : argm;
      auto ret  = stmnt->args[0].getSize();

      if(argc > argm)
         putStmntDropRetn(argc - argm);

      // Clear returned flag.
      putCode(Code::Push_Lit, 0);
      putStmntDropArg(stmnt->args[2], 0);

      switch(argn)
      {
      case  0: putCode(Code::Cspe_1); break;
      case  1: putCode(Code::Cspe_2); break;
      case  2: putCode(Code::Cspe_3); break;
      case  3: putCode(Code::Cspe_4); break;
      default: putCode(Code::Cspe_5); break;
      }

      putWord(84); // ACS_ExecuteWithResult

      // Check returned flag.
      putCode(Code::Jump_Lit, putPos + 16);
      putCode(Code::Wait_Lit, 1);
      putStmntPushArg(stmnt->args[2], 0);
      putCode(Code::Jcnd_Nil, putPos - 8 - lenPushArg(stmnt->args[2], 0));

      // Push any return words.
      if(ret)
         putStmntPushArg(stmnt->args[2], 1, 1 + ret);
   }

   //
   // Info::putStmnt_Cscr_SA
   //
   void Info::putStmnt_Cscr_SA()
   {
      auto argc = getStmntSize();
      auto argm = GetParamMax(IR::CallType::ScriptS);
      auto ret  = stmnt->args[0].getSize();

      if(argc > argm)
         putStmntDropRetn(argc - argm);

      putCode(Code::Cnat);
      putWord(argc < argm ? argc + 1 : argm + 1);
      putWord(44); // ACS_NamedExecuteWithResult

      if(ret)
         putStmntPushRetn(ret - 1);
      else
         putCode(Code::Drop_Nul);
   }

   //
   // Info::putStmnt_Cscr_SS
   //
   void Info::putStmnt_Cscr_SS()
   {
      auto argc = getStmntSize();
      auto argm = GetParamMax(IR::CallType::SScriptS);
      auto ret  = stmnt->args[0].getSize();

      if(argc > argm)
         putStmntDropRetn(argc - argm);

      // Clear returned flag.
      putCode(Code::Push_Lit, 0);
      putStmntDropArg(stmnt->args[2], 0);

      putCode(Code::Cnat);
      putWord(argc < argm ? argc + 1 : argm + 1);
      putWord(44); // ACS_NamedExecuteWithResult
      putCode(Code::Drop_Nul);

      // Check returned flag.
      putCode(Code::Jump_Lit, putPos + 16);
      putCode(Code::Wait_Lit, 1);
      putStmntPushArg(stmnt->args[2], 0);
      putCode(Code::Jcnd_Nil, putPos - 8 - lenPushArg(stmnt->args[2], 0));

      // Push any return words.
      if(ret)
         putStmntPushArg(stmnt->args[2], 1, 1 + ret);
   }

   //
   // Info::putStmnt_Cspe
   //
   void Info::putStmnt_Cspe()
   {
      auto argc = getStmntSize();
      auto ret  = stmnt->args[0].a == IR::ArgBase::Nul ? 0 : stmnt->args[0].getSize();

      switch(stmnt->args.size() > 2 ? stmnt->args[2].a : IR::ArgBase::Lit)
      {
      case IR::ArgBase::Lit:
         if(ret)
         {
            for(auto const &arg : Core::MakeRange(stmnt->args.begin() + 2, stmnt->args.end()))
            {
               for(Core::FastU i = 0; i != arg.aLit.size; ++i)
                  putCode(Code::Push_Lit, getWord(arg.aLit, i));
            }

            for(Core::FastU i = argc; i != 5; ++i)
               putCode(Code::Push_Lit, 0);

            putCode(Code::Cspe_5R1, getWord(stmnt->args[1].aLit));

            break;
         }

         switch(argc)
         {
         case 0: putCode(Code::Cspe_1L); break;
         case 1: putCode(Code::Cspe_1L); break;
         case 2: putCode(Code::Cspe_2L); break;
         case 3: putCode(Code::Cspe_3L); break;
         case 4: putCode(Code::Cspe_4L); break;
         case 5: putCode(Code::Cspe_5L); break;
         }

         putWord(getWord(stmnt->args[1].aLit));

         // Dummy arg.
         if(argc == 0)
            putWord(0);

         for(auto const &arg : Core::MakeRange(stmnt->args.begin() + 2, stmnt->args.end()))
         {
            for(Core::FastU i = 0; i != arg.aLit.size; ++i)
               putWord(getWord(arg.aLit, i));
         }

         break;

      case IR::ArgBase::Stk:
         if(ret)
         {
            switch(argc)
            {
            case 0: putCode(Code::Push_Lit, 0);
            case 1: putCode(Code::Push_Lit, 0);
            case 2: putCode(Code::Push_Lit, 0);
            case 3: putCode(Code::Push_Lit, 0);
            case 4: putCode(Code::Push_Lit, 0);
            case 5: putCode(Code::Cspe_5R1); break;
            }
         }
         else
         {
            switch(argc)
            {
            case 0: putCode(Code::Push_Lit, 0);
            case 1: putCode(Code::Cspe_1); break;
            case 2: putCode(Code::Cspe_2); break;
            case 3: putCode(Code::Cspe_3); break;
            case 4: putCode(Code::Cspe_4); break;
            case 5: putCode(Code::Cspe_5); break;
            }
         }

         putWord(getWord(stmnt->args[1].aLit));

         break;

      default:
         Core::Error(stmnt->pos, "bad put Cspe");
      }
   }

   //
   // Info::putStmnt_Retn
   //
   void Info::putStmnt_Retn()
   {
      auto ret = getStmntSize();

      if(func->allocAut)
      {
         putCode(Code::Push_LocReg, getStkPtrIdx());
         putCode(Code::Call_Nul,    getWord(resolveGlyph("___GDCC__Plsf")));
      }

      switch(func->ctype)
      {
      case IR::CallType::SScriptI:
      case IR::CallType::SScriptS:
         // Set return flag.
         putCode(Code::Push_LocReg, getStkPtrIdx());
         putCode(Code::Push_Lit,    1);
         putCode(Code::SubU);
         putCode(Code::Push_Lit,    1);
         putCode(Code::Drop_GblArr, StaArray);

         // Set return data.
         for(Core::FastU i = ret; i--;)
         {
            putCode(Code::Push_LocReg, getStkPtrIdx());
            putCode(Code::Push_Lit,    i);
            putCode(Code::AddU);
            putCode(Code::Swap);
            putCode(Code::Drop_GblArr, StaArray);
         }

         putCode(Code::Rscr);

         break;

      case IR::CallType::StdCall:
      case IR::CallType::StkCall:
         if(ret == 0)
            putCode(Code::Retn_Nul);
         else
         {
            putStmntDropRetn(ret - 1);
            putCode(Code::Retn_Stk);
         }
         break;

      case IR::CallType::ScriptI:
      case IR::CallType::ScriptS:
         if(ret == 0)
         {
            putCode(Code::Rscr);
         }
         else
         {
            putStmntDropRetn(ret - 1);
            putCode(Code::Drop_ScrRet);
            putCode(Code::Rscr);
         }
         break;

      default:
         Core::Error(stmnt->pos, "bad put Retn");
      }
   }

   //
   // Info::trStmnt_Call
   //
   void Info::trStmnt_Call()
   {
      moveArgStk_dst(stmnt->args[0]);
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
      if(stmnt->args.size() > 0)
         moveArgStk_src(stmnt->args[0]);
   }
}

// EOF

