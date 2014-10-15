//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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

#include "../../Bytecode/Info.hpp"

#include "../../Core/Array.hpp"
#include "../../Core/Counter.hpp"
#include "../../Core/Number.hpp"

#include <unordered_map>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Bytecode
   {
      namespace ZDACS
      {
         enum class Code;

         //
         // Info
         //
         class Info : public InfoBase
         {
         public:
            Info();


            static IR::Type_Fixed const TypeWord;

            static Core::FastU InitScriptNumber;

            static Core::FastU LocArsArray;

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
            class InitVal
            {
            public:
               InitVal() : val{0}, tag{InitTag::Empty} {}

               Core::FastU val;
               InitTag     tag;
            };

            //
            // InitData
            //
            class InitData
            {
            public:
               InitData() : max{0}, needTag{false}, onlyStr{true} {}

               std::unordered_map<Core::FastU, InitVal> vals;

               Core::FastU max;

               bool needTag : 1;
               bool onlyStr : 1;
            };


            void backGlyphFunc(Core::String glyph, Core::FastU val, IR::CallType ctype);
            void backGlyphGlyph(Core::String glyph, Core::String val);
            void backGlyphObj(Core::String glyph, Core::FastU val);
            void backGlyphStr(Core::String glyph, Core::FastU val);
            void backGlyphWord(Core::String glyph, Core::FastU val);

            virtual void gen();

            virtual void genFunc();

            void genIniti();
            void genInitiSpace(IR::Space &space);

            virtual void genObj();

            virtual void genSpace();
            void genSpaceIniti();
            void genSpaceIniti(IR::Space &space);
            void genSpaceInitiValue(InitData &ini, Core::FastU &itr, IR::Value const &val);

            virtual void genStmnt();
            void genStmnt_Call();
            void genStmnt_Cspe();
            void genStmnt_Move_W();
            void genStmnt_Move_W__Arr_Stk(IR::ArgPtr2 const &arr);
            void genStmnt_Move_W__Stk_Arr(IR::ArgPtr2 const &arr);
            void genStmnt_Move_W__Stk_Lit(IR::Exp const *exp);
            void genStmnt_Retn();
            void genStmnt_ShRU_W();

            void genStmntBitwise2();
            void genStmntBitwise3();

            void genStmntPushArg(IR::Arg const &arg, Core::FastU w);
            void genStmntPushArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

            virtual void genStr();

            Core::FastU getInitGblArray();
            Core::FastU getInitGblIndex();
            Core::FastU getInitWldArray();
            Core::FastU getInitWldIndex();

            Core::FastU getStkPtrIdx();

            bool isInitiGblArr();
            bool isInitiWldArr();

            bool isPushArg(IR::Arg const &arg);

            Core::FastU lenChunk();
            Core::FastU lenChunk(char const *name, Core::Array<Core::String> const &strs, bool junk);
            Core::FastU lenChunkAIMP();
            Core::FastU lenChunkAINI();
            Core::FastU lenChunkARAY();
            Core::FastU lenChunkASTR();
            Core::FastU lenChunkATAG();
            Core::FastU lenChunkCODE();
            Core::FastU lenChunkFNAM();
            Core::FastU lenChunkFUNC();
            Core::FastU lenChunkLOAD();
            Core::FastU lenChunkMEXP();
            Core::FastU lenChunkMIMP();
            Core::FastU lenChunkMINI();
            Core::FastU lenChunkMSTR();
            Core::FastU lenChunkSFLG();
            Core::FastU lenChunkSNAM();
            Core::FastU lenChunkSPTR();
            Core::FastU lenChunkSTRL();
            Core::FastU lenChunkSVCT();

            std::size_t lenString(Core::String str);

            virtual void preFunc();
            virtual void preStmnt();
            virtual void preStr();

            virtual void put();

            void putByte(Core::FastU i);

            void putChunk();
            void putChunk(char const *name, Core::Array<Core::String> const &strs, bool junk);
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

            void putCode(Code code);

            void putData(char const *s, std::size_t len);

            void putHWord(Core::FastU i);

            void putIniti();
            void putInitiSpace(IR::Space &space, Code code);

            virtual void putStmnt();
            void putStmnt_Call();
            void putStmnt_Cspe();
            void putStmnt_Move_W();
            void putStmnt_Move_W__Arr_Stk(IR::ArgPtr2 const &arr, Code code);
            void putStmnt_Move_W__Reg_Stk(IR::ArgPtr1 const &reg, Code code);
            void putStmnt_Move_W__Stk_Arr(IR::ArgPtr2 const &arr, Code code);
            void putStmnt_Move_W__Stk_Lit(IR::Exp const *exp);
            void putStmnt_Move_W__Stk_Reg(IR::ArgPtr1 const &reg, Code code);
            void putStmnt_Retn();
            void putStmnt_ShRU_W();
            void putStmnt_Xcod_SID();

            void putStmntBitwise2(Code code);
            void putStmntBitwise3(Code code);

            void putStmntDropRetn(Core::FastU ret);

            void putStmntPushArg(IR::Arg const &arg, Core::FastU w);

            void putStmntPushFunct(Core::FastU value);

            void putStmntPushRetn(Core::FastU ret);

            void putStmntPushStrEn(Core::FastU value);

            void putString(Core::String str);
            void putString(Core::String str, Core::FastU key);

            void putWord(Core::FastU i);

            Core::CounterRef<IR::Exp const> resolveGlyph(Core::String glyph);

            virtual void trFunc();

            virtual void trStmnt();
            void trStmnt_Call();
            void trStmnt_Cspe();
            void trStmnt_Move_W();
            void trStmnt_Retn();
            void trStmnt_ShRU_W();

            void trStmntBitwise2();
            void trStmntBitwise3();
            void trStmntStk3(IR::Code codeMove, bool ordered);

            std::unordered_map<IR::Space const *, InitData> init;

            Core::FastU codeInit;
            Core::FastU codeInitEnd;

            Core::FastU numChunkAIMP;
            Core::FastU numChunkAINI;
            Core::FastU numChunkARAY;
            Core::FastU numChunkASTR;
            Core::FastU numChunkATAG;
            Core::FastU numChunkCODE;
            Core::FastU numChunkFNAM;
            Core::FastU numChunkFUNC;
            Core::FastU numChunkLOAD;
            Core::FastU numChunkMEXP;
            Core::FastU numChunkMIMP;
            Core::FastU numChunkMINI;
            Core::FastU numChunkMSTR;
            Core::FastU numChunkSFLG;
            Core::FastU numChunkSNAM;
            Core::FastU numChunkSPTR;
            Core::FastU numChunkSTRL;
            Core::FastU numChunkSVCT;


            static void CheckArg(IR::Arg const &arg, Core::Origin pos);

            static Core::FastU CodeBase();

            static Core::FastU GetWord(IR::Exp const *exp, Core::FastU w = 0);

            static Core::FastU GetScriptValue(IR::Function const &script);
         };
      }
   }
}

#endif//Bytecode__MgC__Info_H__

