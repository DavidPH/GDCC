//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS target information and handling class.
//
//-----------------------------------------------------------------------------

#ifndef Bytecode__ZDACS__Info_H__
#define Bytecode__ZDACS__Info_H__

#include "../Info.hpp"

#include "GDCC/Counter.hpp"
#include "GDCC/Number.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace Bytecode
{
   namespace ZDACS
   {
      //
      // Info
      //
      class Info : public InfoBase
      {
      public:
         Info();

         virtual void put(std::ostream &out);

         virtual void translateFunction(GDCC::IR::Function &func);
         virtual void translateStatement(GDCC::IR::Statement &stmnt);

      protected:
         void putByte(std::ostream &out, GDCC::FastU i);

         void putChunk(std::ostream &out);
         void putChunkSFLG(std::ostream &out);
         void putChunkSPTR(std::ostream &out);
         void putChunkSVCT(std::ostream &out);

         void putExpWord(std::ostream &out, GDCC::IR::Exp const *exp);

         void putHWord(std::ostream &out, GDCC::FastU i);

         void putStatement(std::ostream &out, GDCC::IR::Statement const &stmnt);
         void putStatement_Cspe(std::ostream &out, GDCC::IR::Statement const &stmnt);
         void putStatement_Move_W(std::ostream &out, GDCC::IR::Statement const &stmnt);
         void putStatement_Retn(std::ostream &out, GDCC::IR::Statement const &stmnt);

         void putWord(std::ostream &out, GDCC::FastU i);

         void translateStatement_Cspe(GDCC::IR::Statement &stmnt);
         void translateStatement_Move_W(GDCC::IR::Statement &stmnt);
         void translateStatement_Retn(GDCC::IR::Statement &stmnt);


         static void CheckArg(GDCC::IR::Arg const &arg, GDCC::Origin pos);

         static GDCC::CounterRef<GDCC::IR::Exp> ResolveGlyph(GDCC::String glyph);
         static GDCC::FastU ResolveValue(GDCC::IR::Value const &val);


         GDCC::FastU jumpPos;
         GDCC::FastU numChunkSFLG;
         GDCC::FastU numChunkSPTR;
         GDCC::FastU numChunkSVCT;
      };
   }
}

#endif//Bytecode__MgC__Info_H__

