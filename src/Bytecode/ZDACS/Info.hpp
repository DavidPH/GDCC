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

         virtual void gen();

         virtual void put(std::ostream &out);


         static GDCC::IR::Type_Fixed const TypeWord;

         static bool UseChunkSTRE;
         static bool UseFakeACS0;

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

         virtual void genFunc();

         virtual void genSpace();
         void genSpaceIniti();
         void genSpaceIniti(GDCC::IR::Space &space);
         void genSpaceInitiValue(InitVal *&data, InitVal const *end, GDCC::IR::Value const &val);

         virtual void genStmnt();
         void genStmnt_Call();
         void genStmnt_Cspe();
         void genStmnt_Move_W();
         void genStmnt_Move_W__Arr_Stk(GDCC::IR::ArgPtr2 const &arr);
         void genStmnt_Move_W__Stk_Arr(GDCC::IR::ArgPtr2 const &arr);
         void genStmnt_Move_W__Stk_Lit(GDCC::IR::Exp const *exp);
         void genStmnt_Retn();

         virtual void genStr();

         GDCC::FastU lenChunk();
         GDCC::FastU lenChunk(char const *name, GDCC::Array<GDCC::String> const &strs, bool junk);
         GDCC::FastU lenChunkAIMP();
         GDCC::FastU lenChunkAINI();
         GDCC::FastU lenChunkARAY();
         GDCC::FastU lenChunkASTR();
         GDCC::FastU lenChunkATAG();
         GDCC::FastU lenChunkCODE();
         GDCC::FastU lenChunkFNAM();
         GDCC::FastU lenChunkFUNC();
         GDCC::FastU lenChunkLOAD();
         GDCC::FastU lenChunkMEXP();
         GDCC::FastU lenChunkMIMP();
         GDCC::FastU lenChunkMINI();
         GDCC::FastU lenChunkMSTR();
         GDCC::FastU lenChunkSFLG();
         GDCC::FastU lenChunkSNAM();
         GDCC::FastU lenChunkSPTR();
         GDCC::FastU lenChunkSTRL();
         GDCC::FastU lenChunkSVCT();

         std::size_t lenString(GDCC::String str);

         virtual void preFunc();
         virtual void preStr();

         void putBlock(GDCC::IR::Block &block);

         void putByte(GDCC::FastU i) {out->put(i & 0xFF);}

         void putChunk();
         void putChunk(char const *name, GDCC::Array<GDCC::String> const &strs, bool junk);
         void putChunkAIMP();
         void putChunkAINI();
         void putChunkARAY();
         void putChunkASTR();
         void putChunkATAG();
         void putChunkCODE();
         void putChunkFNAM();
         void putChunkFUNC();
         void putChunkLOAD();
         void putChunkMEXP();
         void putChunkMIMP();
         void putChunkMINI();
         void putChunkMSTR();
         void putChunkSFLG();
         void putChunkSNAM();
         void putChunkSPTR();
         void putChunkSTRL();
         void putChunkSVCT();

         void putData(char const *str, std::size_t len) {out->write(str, len);}

         void putExpWord(GDCC::IR::Exp const *exp);

         void putHWord(GDCC::FastU i);

         void putStmnt();
         void putStmnt_Call();
         void putStmnt_Cspe();
         void putStmnt_Move_W();
         void putStmnt_Move_W__Arr_Stk(GDCC::IR::ArgPtr2 const &arr, GDCC::FastU i);
         void putStmnt_Move_W__Stk_Arr(GDCC::IR::ArgPtr2 const &arr, GDCC::FastU i);
         void putStmnt_Move_W__Stk_Lit(GDCC::IR::Exp const *exp);
         void putStmnt_Retn();

         void putString(GDCC::String str);
         void putString(GDCC::String str, GDCC::FastU key);

         void putWord(GDCC::FastU i);

         virtual void trFunc();

         virtual void trStmnt();
         void trStmnt_Call();
         void trStmnt_Cspe();
         void trStmnt_Move_W();
         void trStmnt_Move_W__Arr_Stk(GDCC::IR::ArgPtr2 const &arr);
         void trStmnt_Move_W__Stk_Arr(GDCC::IR::ArgPtr2 const &arr);
         void trStmnt_Retn();

         std::unordered_map<GDCC::IR::Space const *, InitData> init;

         GDCC::FastU numChunkAIMP;
         GDCC::FastU numChunkAINI;
         GDCC::FastU numChunkARAY;
         GDCC::FastU numChunkASTR;
         GDCC::FastU numChunkATAG;
         GDCC::FastU numChunkCODE;
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


         static void BackGlyphFunc(GDCC::String glyph, GDCC::FastU val, GDCC::IR::CallType ctype);
         static void BackGlyphGlyph(GDCC::String glyph, GDCC::String val);
         static void BackGlyphStr(GDCC::String glyph, GDCC::FastU val);
         static void BackGlyphWord(GDCC::String glyph, GDCC::FastU val);

         static void CheckArg(GDCC::IR::Arg const &arg, GDCC::Origin pos);

         static GDCC::FastU CodeBase();

         static GDCC::CounterRef<GDCC::IR::Exp> ResolveGlyph(GDCC::String glyph);
         static GDCC::FastU ResolveValue(GDCC::IR::Value const &val);
      };
   }
}

#endif//Bytecode__MgC__Info_H__

