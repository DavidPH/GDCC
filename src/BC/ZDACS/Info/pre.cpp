//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS glyph preparation.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "IR/CallType.hpp"
#include "IR/Linkage.hpp"
#include "IR/Program.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace BC
   {
      namespace ZDACS
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
            switch(stmnt->op.code)
            {
            case IR::Code::AdXU_W: preStmnt_AdXU_W(); break;

            case IR::Code::AddF_W: preStmnt_AddF_W(); break;
            case IR::Code::AddI_W: preStmnt_AddU_W(); break;
            case IR::Code::AddU_W: preStmnt_AddU_W(); break;

            case IR::Code::Bclo_W: preStmnt_Bclz_W(true);  break;
            case IR::Code::Bclz_W: preStmnt_Bclz_W(false); break;

            case IR::Code::CmpF_EQ_W: preStmnt_CmpF_EQ_W(); break;
            case IR::Code::CmpF_GE_W: preStmnt_CmpF_GE_W(); break;
            case IR::Code::CmpF_GT_W: preStmnt_CmpF_GT_W(); break;
            case IR::Code::CmpF_LE_W: preStmnt_CmpF_LE_W(); break;
            case IR::Code::CmpF_LT_W: preStmnt_CmpF_LT_W(); break;
            case IR::Code::CmpF_NE_W: preStmnt_CmpF_NE_W(); break;

            case IR::Code::CmpI_EQ_W:
            case IR::Code::CmpU_EQ_W:
               preStmnt_CmpU_EQ_W(IR::Code::CmpU_EQ_W);
               break;

            case IR::Code::CmpI_GE_W: preStmnt_CmpI_GE_W(); break;
            case IR::Code::CmpI_GT_W: preStmnt_CmpI_GT_W(); break;
            case IR::Code::CmpI_LE_W: preStmnt_CmpI_LE_W(); break;
            case IR::Code::CmpI_LT_W: preStmnt_CmpI_LT_W(); break;

            case IR::Code::CmpI_NE_W:
            case IR::Code::CmpU_NE_W:
               preStmnt_CmpU_EQ_W(IR::Code::CmpU_NE_W);
               break;

            case IR::Code::CmpU_GE_W: preStmnt_CmpU_GE_W(); break;
            case IR::Code::CmpU_GT_W: preStmnt_CmpU_GT_W(); break;
            case IR::Code::CmpU_LE_W: preStmnt_CmpU_LE_W(); break;
            case IR::Code::CmpU_LT_W: preStmnt_CmpU_LT_W(); break;

            case IR::Code::DivF_W: preStmnt_DivF_W(); break;
            case IR::Code::DiXI_W: preStmnt_DiXI_W(); break;
            case IR::Code::DiXU_W: preStmnt_DiXU_W(); break;

            case IR::Code::DivI_W: preStmnt_DiXI_W(); break;
            case IR::Code::DivK_W: preStmnt_DivX_W(IR::Code::DiXU_W); break;
            case IR::Code::DivU_W: preStmnt_DiXU_W(); break;
            case IR::Code::DivX_W: preStmnt_DivX_W(IR::Code::DiXI_W); break;

            case IR::Code::LAnd:
            case IR::Code::LOrI:
               if(stmnt->op.size > 1)
                  preStmntCall(1, stmnt->op.size * 2);
               break;

            case IR::Code::ModI_W: preStmnt_DiXI_W(); break;
            case IR::Code::ModU_W: preStmnt_DiXU_W(); break;

            case IR::Code::MuXU_W: preStmnt_MuXU_W(); break;

            case IR::Code::MulF_W: preStmnt_MulF_W(); break;
            case IR::Code::MulI_W: preStmnt_MulU_W(); break;
            case IR::Code::MulK_W: preStmnt_MulK_W(); break;
            case IR::Code::MulU_W: preStmnt_MulU_W(); break;
            case IR::Code::MulX_W: preStmnt_MulX_W(); break;

            case IR::Code::Retn: preStmnt_Retn(); break;

            case IR::Code::ShLF_W: preStmnt_ShLF_W(); break;
            case IR::Code::ShLU_W: preStmnt_ShLU_W(); break;
            case IR::Code::ShRF_W: preStmnt_ShLF_W(); break;
            case IR::Code::ShRI_W: preStmnt_ShLU_W(); break;
            case IR::Code::ShRU_W: preStmnt_ShLU_W(); break;

            case IR::Code::SuXU_W: preStmnt_SuXU_W(); break;

            case IR::Code::SubF_W: preStmnt_SubF_W(); break;
            case IR::Code::SubI_W: preStmnt_SubU_W(); break;
            case IR::Code::SubU_W: preStmnt_SubU_W(); break;

            default:
               break;
            }
         }

         //
         // Info::preStmntCall
         //
         void Info::preStmntCall(Core::FastU retrn, Core::FastU param)
         {
            preStmntCall(getCallName(), retrn, param);
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
         // Info::preStmntCallDef
         //
         IR::Function *Info::preStmntCallDef(Core::String name,
            Core::FastU retrn, Core::FastU param, Core::FastU localReg,
            char const *file, std::size_t line)
         {
            try {preStmntCall(name, retrn, param);} catch(ResetFunc const &) {}

            IR::Function *newFunc = &prog->getFunction(name);

            if(newFunc->defin)
               return nullptr;

            newFunc->defin    = true;
            newFunc->label    = name + "$label";
            newFunc->localReg = localReg;

            newFunc->block.setOrigin({file, line});

            return newFunc;
         }

         //
         // Info::preStr
         //
         void Info::preStr()
         {
            prog->getGlyphData(str->glyph).type = IR::Type_StrEn();
         }
      }
   }
}

// EOF

