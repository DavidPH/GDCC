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
      public:
         virtual void put(std::ostream &out);

      protected:
         void putExp(std::ostream &out, GDCC::IR::Exp const *exp);

         void putGlyph(std::ostream &out, GDCC::IR::Glyph glyph);

         void putFunc(std::ostream &out, GDCC::IR::Function const &func);

         void putObj(std::ostream &out, GDCC::IR::Object const &obj);
         void putObjValue(std::ostream &out, GDCC::IR::Value const &val);
         void putObjValue_Multi(std::ostream &out, GDCC::IR::Value_Multi const &val);

         void putValue(std::ostream &out, GDCC::IR::Value const &val);

         void putStmnt(std::ostream &out, GDCC::IR::Statement const &stmnt);
         void putStmnt_Jump(std::ostream &out, GDCC::IR::Statement const &stmnt);
         void putStmnt_Move_W(std::ostream &out, GDCC::IR::Statement const &stmnt);

         virtual void trStmnt(GDCC::IR::Statement &stmnt);
         void trStmnt_Call(GDCC::IR::Statement &stmnt);
         void trStmnt_Jump(GDCC::IR::Statement &stmnt);
         void trStmnt_Move_W(GDCC::IR::Statement &stmnt);


         static void CheckArg(GDCC::IR::Arg const &arg, GDCC::Origin const &pos);
      };
   }
}

#endif//Bytecode__MgC__Info_H__

