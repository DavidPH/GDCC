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

#include "GDCC/Array.hpp"
#include "GDCC/Counter.hpp"
#include "GDCC/Number.hpp"

#include <unordered_map>


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

         virtual void tr();


         static GDCC::IR::Type_Fixed const TypeWord;

      protected:
         //
         // InitTag
         //
         enum class InitTag
         {
            Empty,
            Fixed,
            Funct,
            StrEn,
         };

         //
         // InitVal
         //
         struct InitVal
         {
            InitVal() : val{0}, tag{InitTag::Empty} {}

            GDCC::FastU val;
            InitTag     tag;
         };

         //
         // InitData
         //
         class InitData
         {
         public:
            InitData() : needTag{false}, onlyStr{true} {}

            GDCC::Array<InitVal> vals;

            bool needTag : 1;
            bool onlyStr : 1;
         };


         virtual void genFunc(GDCC::IR::Function &func);
         virtual void genStr(GDCC::IR::StrEnt &str);

         std::size_t lenString(GDCC::String str);

         void putByte(std::ostream &out, GDCC::FastU i);

         void putChunk(std::ostream &out);
         void putChunk(std::ostream &out, char const *name,
            GDCC::Array<GDCC::String> const &strs, bool junk);
         void putChunkAIMP(std::ostream &out);
         void putChunkAINI(std::ostream &out);
         void putChunkARAY(std::ostream &out);
         void putChunkASTR(std::ostream &out);
         void putChunkATAG(std::ostream &out);
         void putChunkFNAM(std::ostream &out);
         void putChunkFUNC(std::ostream &out);
         void putChunkLOAD(std::ostream &out);
         void putChunkMEXP(std::ostream &out);
         void putChunkMIMP(std::ostream &out);
         void putChunkMINI(std::ostream &out);
         void putChunkMSTR(std::ostream &out);
         void putChunkSFLG(std::ostream &out);
         void putChunkSNAM(std::ostream &out);
         void putChunkSPTR(std::ostream &out);
         void putChunkSTRL(std::ostream &out);
         void putChunkSVCT(std::ostream &out);

         void putExpWord(std::ostream &out, GDCC::IR::Exp const *exp);

         void putHWord(std::ostream &out, GDCC::FastU i);

         void putStmnt(std::ostream &out, GDCC::IR::Statement const &stmnt);
         void putStmnt_Call(std::ostream &out, GDCC::IR::Statement const &stmnt);
         void putStmnt_Cspe(std::ostream &out, GDCC::IR::Statement const &stmnt);
         void putStmnt_Move_W(std::ostream &out, GDCC::IR::Statement const &stmnt);
         void putStmnt_Move_W__Arr_Stk(std::ostream &out, GDCC::IR::ArgPtr2 const &arr, GDCC::FastU i);
         void putStmnt_Move_W__Stk_Arr(std::ostream &out, GDCC::IR::ArgPtr2 const &arr, GDCC::FastU i);
         void putStmnt_Move_W__Stk_Lit(std::ostream &out, GDCC::IR::Exp const *exp);
         void putStmnt_Retn(std::ostream &out, GDCC::IR::Statement const &stmnt);

         void putString(std::ostream &out, GDCC::String str);

         void putWord(std::ostream &out, GDCC::FastU i);

         virtual void trFunc(GDCC::IR::Function &func);

         virtual void trSpace(GDCC::IR::Space &space);
         void trSpaceIniti(GDCC::IR::Space &space);
         void trSpaceInitiValue(InitVal *&data, InitVal const *end, GDCC::IR::Value const &val);

         virtual void trStmnt(GDCC::IR::Statement &stmnt);
         void trStmnt_Call(GDCC::IR::Statement &stmnt);
         void trStmnt_Cspe(GDCC::IR::Statement &stmnt);
         void trStmnt_Move_W(GDCC::IR::Statement &stmnt);
         void trStmnt_Move_W__Arr_Stk(GDCC::IR::Statement &stmnt, GDCC::IR::ArgPtr2 const &arr);
         void trStmnt_Move_W__Stk_Arr(GDCC::IR::Statement &stmnt, GDCC::IR::ArgPtr2 const &arr);
         void trStmnt_Move_W__Stk_Lit(GDCC::IR::Statement &stmnt, GDCC::IR::Exp const *exp);
         void trStmnt_Retn(GDCC::IR::Statement &stmnt);

         virtual void trStr(GDCC::IR::StrEnt &str);


         static void BackGlyphFunc(GDCC::String glyph, GDCC::FastU val, GDCC::IR::CallType ctype);
         static void BackGlyphGlyph(GDCC::String glyph, GDCC::String val);
         static void BackGlyphStr(GDCC::String glyph, GDCC::FastU val);
         static void BackGlyphWord(GDCC::String glyph, GDCC::FastU val);

         static void CheckArg(GDCC::IR::Arg const &arg, GDCC::Origin pos);

         static GDCC::CounterRef<GDCC::IR::Exp> ResolveGlyph(GDCC::String glyph);
         static GDCC::FastU ResolveValue(GDCC::IR::Value const &val);


         std::unordered_map<GDCC::IR::Space const *, InitData> init;

         GDCC::FastU jumpPos;
         GDCC::FastU numChunkAIMP;
         GDCC::FastU numChunkAINI;
         GDCC::FastU numChunkARAY;
         GDCC::FastU numChunkASTR;
         GDCC::FastU numChunkATAG;
         GDCC::FastU numChunkFNAM;
         GDCC::FastU numChunkFUNC;
         GDCC::FastU numChunkLOAD;
         GDCC::FastU numChunkMEXP;
         GDCC::FastU numChunkMIMP;
         GDCC::FastU numChunkMINI;
         GDCC::FastU numChunkMSTR;
         GDCC::FastU numChunkSFLG;
         GDCC::FastU numChunkSNAM;
         GDCC::FastU numChunkSPTR;
         GDCC::FastU numChunkSTRL;
         GDCC::FastU numChunkSVCT;
      };
   }
}

#endif//Bytecode__MgC__Info_H__

