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

#ifndef Bytecode__MgC__Info_H__
#define Bytecode__MgC__Info_H__

#include "../Info.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

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

         void putBlock(GDCC::IR::Block &block);

         void putExp(GDCC::IR::Exp const *exp);

         void putGlyph(GDCC::IR::Glyph glyph);

         void putFunc(GDCC::IR::Function &func);

         void putObj(GDCC::IR::Object const &obj);
         void putObjValue(GDCC::IR::Value const &val);
         void putObjValue_Multi(GDCC::IR::Value_Multi const &val);

         void putValue(GDCC::IR::Value const &val);

         void putStmnt();
         void putStmnt_Jump();
         void putStmnt_Move_W();

         virtual void trStmnt();
         void trStmntMath();
         void trStmnt_Call();
         void trStmnt_Jump();
         void trStmnt_Move_W();


         static void CheckArg(GDCC::IR::Arg const &arg, GDCC::Origin const &pos);
      };
   }
}

#endif//Bytecode__MgC__Info_H__

