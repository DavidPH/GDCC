//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Doominati statement preparation.
//
//-----------------------------------------------------------------------------

#include "BC/DGE/Info.hpp"

#include "Core/Exception.hpp"

#include "IR/Statement.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::DGE
{
   //
   // Info::preStmnt
   //
   void Info::preStmnt()
   {
      switch(stmnt->code.base)
      {
      case IR::CodeBase::Add:   preStmnt_Add(); break;
      case IR::CodeBase::Bclo:  preStmnt_Bclo(); break;
      case IR::CodeBase::Bclz:  preStmnt_Bclz(); break;
      case IR::CodeBase::CmpEQ: preStmnt_CmpEQ(); break;
      case IR::CodeBase::CmpGE: preStmnt_CmpGE(); break;
      case IR::CodeBase::CmpGT: preStmnt_CmpGT(); break;
      case IR::CodeBase::CmpLE: preStmnt_CmpLE(); break;
      case IR::CodeBase::CmpLT: preStmnt_CmpLT(); break;
      case IR::CodeBase::CmpNE: preStmnt_CmpNE(); break;
      case IR::CodeBase::Div:   preStmnt_Div(); break;
      case IR::CodeBase::DivX:  preStmnt_DivX(); break;
      case IR::CodeBase::Mod:   preStmnt_Mod(); break;
      case IR::CodeBase::Mul:   preStmnt_Mul(); break;
      case IR::CodeBase::MulX:  preStmnt_MulX(); break;
      case IR::CodeBase::Neg:   preStmnt_Neg(); break;
      case IR::CodeBase::ShL:   preStmnt_ShL(); break;
      case IR::CodeBase::ShR:   preStmnt_ShR(); break;
      case IR::CodeBase::Sub:   preStmnt_Sub(); break;
      case IR::CodeBase::Tr:    preStmnt_Tr(); break;

      default:
         break;
      }
   }

   //
   // Info::preStmntStkBin
   //
   void Info::preStmntStkBin(Core::FastU min, AddFunc add)
   {
      auto b = getStmntSize();

      if(b <= 2)
         return;

      auto n = getStmntSizeW(b);

      if(n < min)
         return;

      (this->*add)(n);
   }
}

// EOF

