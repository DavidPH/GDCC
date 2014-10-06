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
            case IR::Code::Plsa:
            case IR::Code::Plsf:
               if(!prog->findFunction("___GDCC__alloc"))
               {
                  auto &newFn = prog->getFunction("___GDCC__alloc");

                  newFn.ctype = IR::CallType::LangC;
                  newFn.linka = IR::Linkage::ExtC;
                  newFn.param = 2;
                  newFn.retrn = 1;

                  newFn.alloc = true;

                  prog->getGlyphData(newFn.glyph).type = IR::Type_Funct(newFn.ctype);
               }
               break;

            default:
               break;
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

