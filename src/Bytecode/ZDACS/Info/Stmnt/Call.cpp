//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Call statement handling.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

#include "Bytecode/ZDACS/Code.hpp"

#include "Core/Exception.hpp"
#include "Core/Range.hpp"

#include "IR/Function.hpp"
#include "IR/CallType.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Bytecode
   {
      namespace ZDACS
      {
         //
         // Info::genStmnt_Call
         //
         void Info::genStmnt_Call()
         {
            auto ret = stmnt->op.size;

            switch(stmnt->args[0].a)
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
               throw Core::ExceptStr(stmnt->pos, "bad gen Call");
            }
         }

         //
         // Info::genStmnt_Cscr_IA
         //
         void Info::genStmnt_Cscr_IA()
         {
            auto argc = stmnt->args.size() - 1;
            auto ret  = stmnt->op.size;

            if(argc > 4)
               numChunkCODE += 8 + (argc - 4) * 20;
            else if(ret)
               numChunkCODE += 8 + (4 - argc) * 8;
            else
               numChunkCODE += 8;

            if(ret)
               numChunkCODE += (ret - 1) * 16;
         }

         //
         // Info::genStmnt_Cscr_IS
         //
         void Info::genStmnt_Cscr_IS()
         {
            auto argc = stmnt->args.size() - 2;
            auto ret  = stmnt->op.size;

            if(argc > 4)
               numChunkCODE += (argc - 4) * 20;

            numChunkCODE += 40
               + lenDropArg(stmnt->args[1], 0)
               + lenPushArg(stmnt->args[1], 0);

            if(ret)
               numChunkCODE += lenPushArg(stmnt->args[1], 1, 1 + ret);
         }

         //
         // Info::genStmnt_Cscr_SA
         //
         void Info::genStmnt_Cscr_SA()
         {
            auto argc = stmnt->args.size() - 1;
            auto ret  = stmnt->op.size;

            if(argc > 4)
               numChunkCODE += (argc - 4) * 20;

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
            auto argc = stmnt->args.size() - 2;
            auto ret  = stmnt->op.size;

            if(argc > 4)
               numChunkCODE += (argc - 4) * 20;

            numChunkCODE += 48
               + lenDropArg(stmnt->args[1], 0)
               + lenPushArg(stmnt->args[1], 0);

            if(ret)
               numChunkCODE += lenPushArg(stmnt->args[1], 1, 1 + ret);
         }

         //
         // Info::genStmnt_Cspe
         //
         void Info::genStmnt_Cspe()
         {
            auto argc = stmnt->args.size() - 1;
            auto ret  = stmnt->op.size;

            // No call args.
            if(argc == 0)
            {
               numChunkCODE += ret ? 48 : 12;
               return;
            }

            switch(stmnt->args[1].a)
            {
            case IR::ArgBase::Lit:
               numChunkCODE += 8 + argc * (ret ? 8 : 4);
               break;

            case IR::ArgBase::Stk:
               numChunkCODE += 8;

               // Dummy args.
               if(ret) numChunkCODE += (5 - argc) * 8;

               break;

            default:
               throw Core::ExceptStr(stmnt->pos, "bad gen Cspr");
            }
         }

         //
         // Info::genStmnt_Retn
         //
         void Info::genStmnt_Retn()
         {
            auto ret = stmnt->op.size;

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
               throw Core::ExceptStr(stmnt->pos, "bad gen Retn");
            }
         }

         //
         // Info::putStmnt_Call
         //
         void Info::putStmnt_Call()
         {
            auto ret = stmnt->op.size;

            switch(stmnt->args[0].a)
            {
            case IR::ArgBase::Lit:
               if(ret == 0)
                  putCode(Code::Call_Nul);
               else
                  putCode(Code::Call_Lit);

               putWord(GetWord(stmnt->args[0].aLit));

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
               throw Core::ExceptStr(stmnt->pos, "bad put Call");
            }
         }

         //
         // Info::putStmnt_Casm
         //
         void Info::putStmnt_Casm()
         {
            for(auto const &arg : stmnt->args)
               putWord(GetWord(arg.aLit));
         }

         //
         // Info::putStmnt_Cnat
         //
         void Info::putStmnt_Cnat()
         {
            putCode(Code::Cnat);
            putWord(stmnt->args.size() - 1);
            putWord(GetWord(stmnt->args[0].aLit));
         }

         //
         // Info::putStmnt_Cscr_IA
         //
         void Info::putStmnt_Cscr_IA()
         {
            auto argc = stmnt->args.size() - 1;
            auto ret  = stmnt->op.size;

            if(argc > 4)
               putStmntDropRetn(argc - 4);

            if(ret) switch(argc)
            {
            case  0: putCode(Code::Push_Lit, 0);
            case  1: putCode(Code::Push_Lit, 0);
            case  2: putCode(Code::Push_Lit, 0);
            case  3: putCode(Code::Push_Lit, 0);
            default: putCode(Code::Cspe_5R1); break;
            }
            else switch(argc)
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
            auto argc = stmnt->args.size() - 2;
            auto ret  = stmnt->op.size;

            if(argc > 4)
               putStmntDropRetn(argc - 4);

            // Clear returned flag.
            putCode(Code::Push_Lit, 0);
            putStmntDropArg(stmnt->args[1], 0);

            switch(argc)
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
            putStmntPushArg(stmnt->args[1], 0);
            putCode(Code::Jcnd_Nil, putPos - 8 - lenPushArg(stmnt->args[1], 0));

            // Push any return words.
            if(ret)
               putStmntPushArg(stmnt->args[1], 1, 1 + ret);
         }

         //
         // Info::putStmnt_Cscr_SA
         //
         void Info::putStmnt_Cscr_SA()
         {
            auto argc = stmnt->args.size() - 1;
            auto ret  = stmnt->op.size;

            if(argc > 4)
               putStmntDropRetn(argc - 4);

            putCode(Code::Cnat);
            putWord(argc < 4 ? argc + 1 : 5);
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
            auto argc = stmnt->args.size() - 2;
            auto ret  = stmnt->op.size;

            if(argc > 4)
               putStmntDropRetn(argc - 4);

            // Clear returned flag.
            putCode(Code::Push_Lit, 0);
            putStmntDropArg(stmnt->args[1], 0);

            putCode(Code::Cnat);
            putWord(argc < 4 ? argc + 1 : 5);
            putWord(44); // ACS_NamedExecuteWithResult
            putCode(Code::Drop_Nul);

            // Check returned flag.
            putCode(Code::Jump_Lit, putPos + 16);
            putCode(Code::Wait_Lit, 1);
            putStmntPushArg(stmnt->args[1], 0);
            putCode(Code::Jcnd_Nil, putPos - 8 - lenPushArg(stmnt->args[1], 0));

            // Push any return words.
            if(ret)
               putStmntPushArg(stmnt->args[1], 1, 1 + ret);
         }

         //
         // Info::putStmnt_Cspe
         //
         void Info::putStmnt_Cspe()
         {
            auto argc = stmnt->args.size() - 1;
            auto ret  = stmnt->op.size;

            IR::ArgBase a;
            if(argc == 0)
               a = ret ? IR::ArgBase::Stk : IR::ArgBase::Lit;
            else
               a = stmnt->args[1].a;

            switch(a)
            {
            case IR::ArgBase::Lit:
               if(ret)
               {
                  for(auto const &arg : Core::MakeRange(stmnt->args.begin() + 1, stmnt->args.end()))
                  {
                     putCode(Code::Push_Lit);
                     putWord(GetWord(arg.aLit));
                  }

                  putCode(Code::Cspe_5R1, GetWord(stmnt->args[0].aLit));

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

               putWord(GetWord(stmnt->args[0].aLit));

               // Dummy arg.
               if(argc == 0)
                  putWord(0);

               for(auto const &arg : Core::MakeRange(stmnt->args.begin() + 1, stmnt->args.end()))
                  putWord(GetWord(arg.aLit));

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

               putWord(GetWord(stmnt->args[0].aLit));

               break;

            default:
               throw Core::ExceptStr(stmnt->pos, "bad put Cspe");
            }
         }

         //
         // Info::putStmnt_Retn
         //
         void Info::putStmnt_Retn()
         {
            auto ret = stmnt->op.size;

            switch(func->ctype)
            {
            case IR::CallType::SScriptI:
            case IR::CallType::SScriptS:
               // Set return flag.
               putCode(Code::Push_LocReg, getStkPtrIdx());
               putCode(Code::Push_Lit,    1);
               putCode(Code::SubU);
               putCode(Code::Push_Lit,    1);
               putCode(Code::Drop_GblArr, LocArsArray);

               // Set return data.
               for(Core::FastU i = ret; i--;)
               {
                  putCode(Code::Push_LocReg, getStkPtrIdx());
                  putCode(Code::Push_Lit,    i);
                  putCode(Code::AddU);
                  putCode(Code::Swap);
                  putCode(Code::Drop_GblArr, LocArsArray);
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
               throw Core::ExceptStr(stmnt->pos, "bad put Retn");
            }
         }

         //
         // Info::trStmnt_Call
         //
         void Info::trStmnt_Call()
         {
            CheckArgC(stmnt, 1);
            for(auto n = stmnt->args.size(); --n != 0;)
               CheckArgB(stmnt, n, IR::ArgBase::Stk);

            switch(stmnt->args[0].a)
            {
            case IR::ArgBase::Lit:
            case IR::ArgBase::Stk:
               break;

            default:
               throw Core::ExceptStr(stmnt->pos, "bad tr Call");
            }
         }

         //
         // Info::trStmnt_Casm
         //
         void Info::trStmnt_Casm()
         {
            CheckArgC(stmnt, 1);
            CheckArgB(stmnt, 0, IR::ArgBase::Lit);

            for(auto n = stmnt->args.size(); --n != 0;)
               CheckArgB(stmnt, n, IR::ArgBase::Lit);
         }

         //
         // Info::trStmnt_Cnat
         //
         void Info::trStmnt_Cnat()
         {
            CheckArgC(stmnt, 1);
            CheckArgB(stmnt, 0, IR::ArgBase::Lit);

            for(auto n = stmnt->args.size(); --n != 0;)
               CheckArgB(stmnt, n, IR::ArgBase::Stk);
         }

         //
         // Info::trStmnt_Cscr_IA
         //
         void Info::trStmnt_Cscr_IA()
         {
            CheckArgC(stmnt, 1);
            CheckArgB(stmnt, 0, IR::ArgBase::Stk);
            for(auto n = stmnt->args.size(); --n != 0;)
               CheckArgB(stmnt, n, IR::ArgBase::Stk);
         }

         //
         // Info::trStmnt_Cscr_IS
         //
         void Info::trStmnt_Cscr_IS()
         {
            CheckArgC(stmnt, 2);
            CheckArgB(stmnt, 0, IR::ArgBase::Stk);
            for(auto n = stmnt->args.size(); --n != 1;)
               CheckArgB(stmnt, n, IR::ArgBase::Stk);
         }

         //
         // Info::trStmnt_Cscr_SA
         //
         void Info::trStmnt_Cscr_SA()
         {
            CheckArgC(stmnt, 1);
            CheckArgB(stmnt, 0, IR::ArgBase::Stk);
            for(auto n = stmnt->args.size(); --n != 0;)
               CheckArgB(stmnt, n, IR::ArgBase::Stk);
         }

         //
         // Info::trStmnt_Cscr_SS
         //
         void Info::trStmnt_Cscr_SS()
         {
            CheckArgC(stmnt, 2);
            CheckArgB(stmnt, 0, IR::ArgBase::Stk);
            for(auto n = stmnt->args.size(); --n != 1;)
               CheckArgB(stmnt, n, IR::ArgBase::Stk);
         }

         //
         // Info::trStmnt_Cspe
         //
         void Info::trStmnt_Cspe()
         {
            CheckArgC(stmnt, 1);
            CheckArgB(stmnt, 0, IR::ArgBase::Lit);

            auto ret = stmnt->op.size;

            if(ret > 1)
               throw Core::ExceptStr(stmnt->pos, "bad tr Cspe ret");

            // Too many call args.
            if(stmnt->args.size() > 6)
               throw Core::ExceptStr(stmnt->pos, "bad tr Cspe argc");

            // No call args.
            if(stmnt->args.size() == 1)
               return;

            switch(stmnt->args[1].a)
            {
            case IR::ArgBase::Lit:
               for(auto n = stmnt->args.size(); --n != 1;)
                  CheckArgB(stmnt, n, IR::ArgBase::Lit);
               break;

            case IR::ArgBase::Stk:
               for(auto n = stmnt->args.size(); --n != 1;)
                  CheckArgB(stmnt, n, IR::ArgBase::Stk);
               break;

            default:
               throw Core::ExceptStr(stmnt->pos, "bad tr Cspe");
            }
         }

         //
         // Info::trStmnt_Retn
         //
         void Info::trStmnt_Retn()
         {
            if(stmnt->op.size)
            {
               CheckArgC(stmnt, 1);
               moveArgStk_src(stmnt->args[0], stmnt->op.size);
            }
         }
      }
   }
}

// EOF

