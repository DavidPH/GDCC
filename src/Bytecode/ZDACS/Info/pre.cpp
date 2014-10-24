//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS glyph preparation.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

#include "IR/CallType.hpp"
#include "IR/Linkage.hpp"
#include "IR/Program.hpp"


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

            case IR::CallType::ScriptI:
               prog->getGlyphData(func->glyph).type = TypeWord;
               break;

            case IR::CallType::ScriptS:
               prog->getGlyphData(func->glyph).type = IR::Type_StrEn();
               break;

            default: break;
            }
         }

         //
         // Info::preStmnt
         //
         void Info::preStmnt()
         {
            switch(stmnt->code)
            {
            case IR::Code::AddI_W2:
            case IR::Code::AddU_W2: preStmntCall("___GDCC__AddU_W2", 2, 4); break;
            case IR::Code::AddI_W3:
            case IR::Code::AddU_W3: preStmntCall("___GDCC__AddU_W3", 3, 6); break;

            case IR::Code::CmpI_EQ_W2: preStmntCall("___GDCC__CmpU_EQ_W2", 1, 4); break;
            case IR::Code::CmpI_EQ_W3: preStmntCall("___GDCC__CmpU_EQ_W2", 1, 6); break;

            case IR::Code::CmpI_GE_W2: preStmntCall("___GDCC__CmpI_GE_W2", 1, 4); break;
            case IR::Code::CmpI_GE_W3: preStmntCall("___GDCC__CmpI_GE_W3", 1, 6); break;

            case IR::Code::CmpI_GT_W2: preStmntCall("___GDCC__CmpI_GT_W2", 1, 4); break;
            case IR::Code::CmpI_GT_W3: preStmntCall("___GDCC__CmpI_GT_W3", 1, 6); break;

            case IR::Code::CmpI_LE_W2: preStmntCall("___GDCC__CmpI_LE_W2", 1, 4); break;
            case IR::Code::CmpI_LE_W3: preStmntCall("___GDCC__CmpI_LE_W3", 1, 6); break;

            case IR::Code::CmpI_LT_W2: preStmntCall("___GDCC__CmpI_LT_W2", 1, 4); break;
            case IR::Code::CmpI_LT_W3: preStmntCall("___GDCC__CmpI_LT_W3", 1, 6); break;

            case IR::Code::CmpI_NE_W2: preStmntCall("___GDCC__CmpU_NE_W2", 1, 4); break;
            case IR::Code::CmpI_NE_W3: preStmntCall("___GDCC__CmpU_NE_W3", 1, 6); break;

            case IR::Code::CmpU_EQ_W2: preStmntCall("___GDCC__CmpU_EQ_W2", 1, 4); break;
            case IR::Code::CmpU_EQ_W3: preStmntCall("___GDCC__CmpU_EQ_W3", 1, 6); break;

            case IR::Code::CmpU_GE_W:  preStmntCall("___GDCC__CmpU_GE_W",  1, 2); break;
            case IR::Code::CmpU_GE_W2: preStmntCall("___GDCC__CmpU_GE_W2", 1, 4); break;
            case IR::Code::CmpU_GE_W3: preStmntCall("___GDCC__CmpU_GE_W3", 1, 6); break;

            case IR::Code::CmpU_GT_W:  preStmntCall("___GDCC__CmpU_GT_W",  1, 2); break;
            case IR::Code::CmpU_GT_W2: preStmntCall("___GDCC__CmpU_GT_W2", 1, 4); break;
            case IR::Code::CmpU_GT_W3: preStmntCall("___GDCC__CmpU_GT_W3", 1, 6); break;

            case IR::Code::CmpU_LE_W:  preStmntCall("___GDCC__CmpU_LE_W",  1, 2); break;
            case IR::Code::CmpU_LE_W2: preStmntCall("___GDCC__CmpU_LE_W2", 1, 4); break;
            case IR::Code::CmpU_LE_W3: preStmntCall("___GDCC__CmpU_LE_W3", 1, 6); break;

            case IR::Code::CmpU_LT_W:  preStmntCall("___GDCC__CmpU_LT_W",  1, 2); break;
            case IR::Code::CmpU_LT_W2: preStmntCall("___GDCC__CmpU_LT_W2", 1, 4); break;
            case IR::Code::CmpU_LT_W3: preStmntCall("___GDCC__CmpU_LT_W3", 1, 6); break;

            case IR::Code::CmpU_NE_W2: preStmntCall("___GDCC__CmpU_NE_W2", 1, 4); break;
            case IR::Code::CmpU_NE_W3: preStmntCall("___GDCC__CmpU_NE_W3", 1, 6); break;

            case IR::Code::DivI_W2: preStmntCall("___GDCC__DivI_W2", 2, 4); break;
            case IR::Code::DivI_W3: preStmntCall("___GDCC__DivI_W3", 3, 6); break;
            case IR::Code::DivU_W:  preStmntCall("___GDCC__DivU_W",  1, 2); break;
            case IR::Code::DivU_W2: preStmntCall("___GDCC__DivU_W2", 2, 4); break;
            case IR::Code::DivU_W3: preStmntCall("___GDCC__DivU_W3", 3, 6); break;
            case IR::Code::DivX_W2: preStmntCall("___GDCC__DivX_W2", 2, 4); break;

            case IR::Code::ModI_W2: preStmntCall("___GDCC__ModI_W2", 2, 4); break;
            case IR::Code::ModI_W3: preStmntCall("___GDCC__ModI_W3", 3, 6); break;
            case IR::Code::ModU_W:  preStmntCall("___GDCC__ModU_W",  1, 2); break;
            case IR::Code::ModU_W2: preStmntCall("___GDCC__ModU_W2", 2, 4); break;
            case IR::Code::ModU_W3: preStmntCall("___GDCC__ModU_W3", 3, 6); break;

            case IR::Code::MulI_W2:
            case IR::Code::MulU_W2: preStmntCall("___GDCC__MulU_W2", 2, 4); break;
            case IR::Code::MulI_W3:
            case IR::Code::MulU_W3: preStmntCall("___GDCC__MulU_W3", 3, 6); break;
            case IR::Code::MulX_W2: preStmntCall("___GDCC__MulX_W2", 2, 4); break;

            case IR::Code::Plsa:
            case IR::Code::Plsf: preStmntCall("___GDCC__alloc", 1, 2); break;

            case IR::Code::ShLU_W2: preStmntCall("___GDCC__ShLU_W2", 2, 4); break;
            case IR::Code::ShLU_W3: preStmntCall("___GDCC__ShLU_W2", 3, 6); break;

            case IR::Code::ShRI_W2: preStmntCall("___GDCC__ShRI_W2", 2, 4); break;
            case IR::Code::ShRI_W3: preStmntCall("___GDCC__ShRI_W2", 3, 6); break;

            case IR::Code::ShRU_W2: preStmntCall("___GDCC__ShRU_W2", 2, 4); break;
            case IR::Code::ShRU_W3: preStmntCall("___GDCC__ShRU_W2", 3, 6); break;

            case IR::Code::SubI_W2:
            case IR::Code::SubU_W2: preStmntCall("___GDCC__SubU_W2", 2, 4); break;
            case IR::Code::SubI_W3:
            case IR::Code::SubU_W3: preStmntCall("___GDCC__SubU_W3", 3, 6); break;

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
            }
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

