//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS glyph preparation.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "IR/Linkage.hpp"
#include "IR/Program.hpp"

#include "Target/CallType.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   //
   // Info::preFunc
   //
   void Info::preFunc()
   {
      if(func->defin) switch(func->ctype)
      {
      case IR::CallType::StdCall:
      case IR::CallType::StkCall:
         prog->getGlyphData(func->glyph).type = IR::Type_Funct(func->ctype);
         break;

      default: break;
      }

      if(func->defin && func->allocAut)
         preStmntCall("___GDCC__Plsa", 1, 1);

      InfoBase::preFunc();
   }

   //
   // Info::preObj
   //
   void Info::preObj()
   {
      // TODO: Normalize usage of unused space name to be "", not null.
      if(!obj->space.name)
         obj->space.name = Core::STR_;
   }

   //
   // Info::preStmnt
   //
   void Info::preStmnt()
   {
      switch(stmnt->code.base)
      {
      case IR::CodeBase::Add:   preStmnt_Add(); break;
      case IR::CodeBase::AddX:  preStmnt_AddX(); break;
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
      case IR::CodeBase::Retn:  preStmnt_Retn(); break;
      case IR::CodeBase::Rjnk:  preStmnt_Rjnk(); break;
      case IR::CodeBase::ShL:   preStmnt_ShL(); break;
      case IR::CodeBase::ShR:   preStmnt_ShR(); break;
      case IR::CodeBase::Sub:   preStmnt_Sub(); break;
      case IR::CodeBase::SubX:  preStmnt_SubX(); break;

      default:
         break;
      }
   }

   //
   // Info::preStmntCall
   //
   void Info::preStmntCall(Core::String name, Core::FastU retrn, Core::FastU param)
   {
      if(!prog->findFunction(name))
      {
         auto &newFn = prog->getFunction(name);

         newFn.ctype = IR::CallType::StkCall;
         newFn.linka = IR::Linkage::ExtC;
         newFn.param = param;
         newFn.retrn = retrn;

         newFn.alloc = true;

         prog->getGlyphData(name).type = IR::Type_Funct(newFn.ctype);

         // Signal iterator to start over.
         throw ResetFunc();
      }
   }

   //
   // Info::preStmntStkBin
   //
   void Info::preStmntStkBin(Core::FastU min, AddFunc add)
   {
      auto n = getStmntSize();

      if(n < min)
         return;

      (this->*add)(n);
   }

   //
   // Info::preStrEnt
   //
   void Info::preStrEnt()
   {
      prog->getGlyphData(strent->glyph).type = IR::Type_StrEn();
   }
}

// EOF

