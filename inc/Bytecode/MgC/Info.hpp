//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// MageCraft target information and handling class.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Bytecode__MgC__Info_H__
#define GDCC__Bytecode__MgC__Info_H__

#include "../../Bytecode/Info.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Bytecode
   {
      namespace MgC
      {
         //
         // Info
         //
         class Info : public InfoBase
         {
         protected:
            virtual void put();

            void putExp(IR::Exp const *exp);

            void putGlyph(IR::Glyph glyph);

            virtual void putFunc();
            using InfoBase::putFunc;

            virtual void putObj();
            using InfoBase::putObj;
            void putObjValue(IR::Value const &val);
            void putObjValue_Multi(Core::Array<IR::Value> const &val);

            void putValue(IR::Value const &val);

            virtual void putStmnt();
            void putStmnt_Jump();
            void putStmnt_Move_W();

            virtual void trStmnt();
            void trStmntMath();
            void trStmnt_Call();
            void trStmnt_Jump();
            void trStmnt_Move_W();


            static void CheckArg(IR::Arg const &arg, Core::Origin const &pos);
         };
      }
   }
}

#endif//GDCC__Bytecode__MgC__Info_H__

