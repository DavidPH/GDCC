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

         virtual void translateBlock(GDCC::IR::Block &block);

      protected:
         void putExp(std::ostream &out, GDCC::IR::Exp const *exp);

         void putGlyph(std::ostream &out, GDCC::IR::Glyph glyph);

         void putFunction(std::ostream &out, GDCC::IR::Function const &func);

         void putValue(std::ostream &out, GDCC::IR::Value const &val);

         void putStatement(std::ostream &out, GDCC::IR::Statement const &stmnt);
         void putStatement_Jump(std::ostream &out, GDCC::IR::Statement const &stmnt);
         void putStatement_Move_W(std::ostream &out, GDCC::IR::Statement const &stmnt);

         void translateStatement(GDCC::IR::Statement &stmnt);
         void translateStatement_Call(GDCC::IR::Statement &stmnt);
         void translateStatement_Jump(GDCC::IR::Statement &stmnt);
         void translateStatement_Move_W(GDCC::IR::Statement &stmnt);
      };
   }
}

#endif//Bytecode__MgC__Info_H__

