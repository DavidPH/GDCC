//-----------------------------------------------------------------------------
//
// Copyright (C) 2018-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Generic check functions.
//
//-----------------------------------------------------------------------------

#include "BC/Info.hpp"

#include "IR/Exception.hpp"
#include "IR/Statement.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC
{
   //
   // Info::chkStmnt
   //
   void Info::chkStmnt()
   {
      for(auto &arg : stmnt->args)
         chkStmntArg(arg);

      for(auto t : stmnt->code.type) switch(t)
      {
      case 'A': case 'E': case 'F': case 'I':
      case 'K': case 'R': case 'U': case 'X':
         break;

      default:
         IR::ErrorCode(stmnt, "bad code type");
      }

      switch(stmnt->code.base)
      {
      case IR::CodeBase::Nop:
      case IR::CodeBase::Xcod_SID:
         chkStmntArgC(0);
         break;

      case IR::CodeBase::Retn:
      case IR::CodeBase::Rjnk:
         chkStmntArgC(0, 1);
         break;

      case IR::CodeBase::Jdyn:
      case IR::CodeBase::Jump:
         chkStmntArgC(1);
         break;

      case IR::CodeBase::BNot:
      case IR::CodeBase::Bclo:
      case IR::CodeBase::Bclz:
      case IR::CodeBase::Copy:
      case IR::CodeBase::Jcnd_Nil:
      case IR::CodeBase::Jcnd_Tru:
      case IR::CodeBase::Jfar_Pro:
      case IR::CodeBase::Jfar_Set:
      case IR::CodeBase::LNot:
      case IR::CodeBase::Move:
      case IR::CodeBase::Neg:
      case IR::CodeBase::Pltn:
      case IR::CodeBase::Swap:
      case IR::CodeBase::Tr:
         chkStmntArgC(2);
         break;

      case IR::CodeBase::Call:
      case IR::CodeBase::Casm:
      case IR::CodeBase::Cnat:
      case IR::CodeBase::Cscr_IA:
      case IR::CodeBase::Cscr_SA:
      case IR::CodeBase::Cspe:
         chkStmntArgC(2, -1);
         break;

      case IR::CodeBase::Add:
      case IR::CodeBase::BAnd:
      case IR::CodeBase::BOrI:
      case IR::CodeBase::BOrX:
      case IR::CodeBase::CmpEQ:
      case IR::CodeBase::CmpGE:
      case IR::CodeBase::CmpGT:
      case IR::CodeBase::CmpLE:
      case IR::CodeBase::CmpLT:
      case IR::CodeBase::CmpNE:
      case IR::CodeBase::Div:
      case IR::CodeBase::DivX:
      case IR::CodeBase::Jfar_Sta:
      case IR::CodeBase::LAnd:
      case IR::CodeBase::LOrI:
      case IR::CodeBase::Mod:
      case IR::CodeBase::Mul:
      case IR::CodeBase::MulX:
      case IR::CodeBase::ShL:
      case IR::CodeBase::ShR:
      case IR::CodeBase::Sub:
         chkStmntArgC(3);
         break;

      case IR::CodeBase::Cscr_IS:
      case IR::CodeBase::Cscr_SS:
         chkStmntArgC(3, -1);
         break;

      case IR::CodeBase::AddX:
      case IR::CodeBase::SubX:
         chkStmntArgC(3, 4);
         break;

      case IR::CodeBase::Bges:
      case IR::CodeBase::Bget:
      case IR::CodeBase::Bset:
         chkStmntArgC(4);
         break;

      case IR::CodeBase::Jcnd_Tab:
         if(stmnt->args.size() % 2 == 0)
            IR::ErrorCode(stmnt, "bad arg count");
         break;

      case IR::CodeBase::None:
         IR::ErrorCode(stmnt, "bad code base");
      }

      switch(auto size = stmnt->code.type.size(); stmnt->code.base)
      {
      case IR::CodeBase::Nop:
      case IR::CodeBase::BAnd:
      case IR::CodeBase::BNot:
      case IR::CodeBase::BOrI:
      case IR::CodeBase::BOrX:
      case IR::CodeBase::Bclo:
      case IR::CodeBase::Bclz:
      case IR::CodeBase::Bges:
      case IR::CodeBase::Bget:
      case IR::CodeBase::Bset:
      case IR::CodeBase::Call:
      case IR::CodeBase::Casm:
      case IR::CodeBase::Cnat:
      case IR::CodeBase::Copy:
      case IR::CodeBase::Cscr_IA:
      case IR::CodeBase::Cscr_IS:
      case IR::CodeBase::Cscr_SA:
      case IR::CodeBase::Cscr_SS:
      case IR::CodeBase::Cspe:
      case IR::CodeBase::Jcnd_Nil:
      case IR::CodeBase::Jcnd_Tab:
      case IR::CodeBase::Jcnd_Tru:
      case IR::CodeBase::Jdyn:
      case IR::CodeBase::Jfar_Pro:
      case IR::CodeBase::Jfar_Set:
      case IR::CodeBase::Jfar_Sta:
      case IR::CodeBase::Jump:
      case IR::CodeBase::LAnd:
      case IR::CodeBase::LNot:
      case IR::CodeBase::LOrI:
      case IR::CodeBase::Move:
      case IR::CodeBase::Pltn:
      case IR::CodeBase::Retn:
      case IR::CodeBase::Rjnk:
      case IR::CodeBase::Swap:
      case IR::CodeBase::Xcod_SID:
         if(size != 0)
            IR::ErrorCode(stmnt, "bad code type");
         break;

      case IR::CodeBase::Add:
      case IR::CodeBase::AddX:
      case IR::CodeBase::CmpEQ:
      case IR::CodeBase::CmpGE:
      case IR::CodeBase::CmpGT:
      case IR::CodeBase::CmpLE:
      case IR::CodeBase::CmpLT:
      case IR::CodeBase::CmpNE:
      case IR::CodeBase::Div:
      case IR::CodeBase::DivX:
      case IR::CodeBase::Mod:
      case IR::CodeBase::Mul:
      case IR::CodeBase::MulX:
      case IR::CodeBase::Neg:
      case IR::CodeBase::ShL:
      case IR::CodeBase::ShR:
      case IR::CodeBase::Sub:
      case IR::CodeBase::SubX:
         if(size != 0 && size != 1)
            IR::ErrorCode(stmnt, "bad code type");
         break;

      case IR::CodeBase::Tr:
         if(size != 0 && size != 1 && size != 2)
            IR::ErrorCode(stmnt, "bad code type");
         break;

      case IR::CodeBase::None:
         IR::ErrorCode(stmnt, "bad code type");
      }
   }

   //
   // Info::chkStmntArg
   //
   void Info::chkStmntArg(IR::Arg const &arg)
   {
      if(arg.a == IR::ArgBase::Gen || arg.a == IR::ArgBase::Cpy)
         IR::ErrorCode(stmnt, "bad arg type");
   }

   //
   // Info::chkStmntArgA
   //
   void Info::chkStmntArgA(IR::ArgPtr2 const &arg, IR::ArgBase a)
   {
      if(arg.arr->a != a)
         IR::ErrorCode(stmnt, ".arr must be ", a);
   }

   //
   // Info::chkStmntArgB
   //
   void Info::chkStmntArgB(Core::FastU n, IR::ArgBase a)
   {
      if(stmnt->args[n].a != a)
         IR::ErrorCode(stmnt, "arg[", n, "] must be ", a);
   }

   //
   // Info::chkStmntArgC
   //
   void Info::chkStmntArgC(Core::FastU min, Core::FastU max)
   {
      if(stmnt->args.size() < min || stmnt->args.size() > max)
         IR::ErrorCode(stmnt, "bad arg count");
   }

   //
   // Info::chkStmntArgI
   //
   void Info::chkStmntArgI(IR::ArgPtr1 const &arg, IR::ArgBase a)
   {
      if(arg.idx->a != a)
         IR::ErrorCode(stmnt, ".idx must be ", a);
   }

   //
   // Info::chkStmntArgI
   //
   void Info::chkStmntArgI(IR::ArgPtr2 const &arg, IR::ArgBase a)
   {
      if(arg.idx->a != a)
         IR::ErrorCode(stmnt, ".idx must be ", a);
   }
}

// EOF

