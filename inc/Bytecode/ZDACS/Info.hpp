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
            void genStmnt_AddU_W();
            void genStmnt_Call();
            void genStmnt_CmpU_EQ_W2();
            void genStmnt_CmpU_EQ_W3();
            void genStmnt_Cscr_IA();
            void genStmnt_Cscr_IS();
            void genStmnt_Cscr_SA();
            void genStmnt_Cscr_SS();
            void genStmnt_Cspe();
            void genStmnt_DiXI_W();
            void genStmnt_InvU_W2();
            void genStmnt_InvU_W3();
            void genStmnt_Move_W();
            void genStmnt_Move_W__Arr_Stk(IR::ArgPtr2 const &arr);
            void genStmnt_Move_W__Stk_Arr(IR::ArgPtr2 const &arr);
            void genStmnt_Move_Wx(Core::FastU x);
            void genStmnt_NegF_Wx();
            void genStmnt_NegI_W2();
            void genStmnt_NegI_W3();
            void genStmnt_Retn();
            void genStmnt_ShRU_W();
            void genStmnt_ShLU_W2();
            void genStmnt_ShRI_W2();
            void genStmnt_ShRU_W2();
            void genStmnt_ShLU_W3();
            void genStmnt_ShRI_W3();
            void genStmnt_ShRU_W3();

            void genStmntBitwise2();
            void genStmntBitwise3();

            void genStmntCall(Core::FastU ret);

            void genStmntDropArg(IR::Arg const &arg, Core::FastU w);
            void genStmntDropArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

            void genStmntPushArg(IR::Arg const &arg, Core::FastU w);
            void genStmntPushArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

            virtual void genStr();

            Core::FastU getInitGblArray();
            Core::FastU getInitGblIndex();
            Core::FastU getInitWldArray();
            Core::FastU getInitWldIndex();

            Core::FastU getStkPtrIdx();

            bool isCopyArg(IR::Arg const &arg);

            bool isDropArg(IR::Arg const &arg);

            bool isFastArg(IR::Arg const &arg);

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

            Core::FastU lenDropArg(IR::Arg const &arg, Core::FastU w);
            Core::FastU lenDropArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

            Core::FastU lenIncUArg(IR::Arg const &arg, Core::FastU w);
            Core::FastU lenIncUArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

            Core::FastU lenPushArg(IR::Arg const &arg, Core::FastU w);
            Core::FastU lenPushArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

            std::size_t lenString(Core::String str);

            virtual void preFunc();

            virtual void preStmnt();
            void preStmntCall(Core::String name, Core::FastU retrn, Core::FastU param);

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
            void putCode(Code code, Core::FastU arg0);

            void putData(char const *s, std::size_t len);

            using InfoBase::putFunc;
            virtual void putFunc();

            void putHWord(Core::FastU i);

            void putIniti();
            void putInitiSpace(IR::Space &space, Code code);

            virtual void putStmnt();
            void putStmnt_AddU_W();
            void putStmnt_Call();
            void putStmnt_CmpU_EQ_W2();
            void putStmnt_CmpU_EQ_W3();
            void putStmnt_CmpU_NE_W2();
            void putStmnt_CmpU_NE_W3();
            void putStmnt_Cscr_IA();
            void putStmnt_Cscr_IS();
            void putStmnt_Cscr_SA();
            void putStmnt_Cscr_SS();
            void putStmnt_Cspe();
            void putStmnt_DiXI_W();
            void putStmnt_InvU_W2();
            void putStmnt_InvU_W3();
            void putStmnt_Move_W();
            void putStmnt_Move_W__Arr_Stk(IR::ArgPtr2 const &arr, Code code);
            void putStmnt_Move_W__Stk_Arr(IR::ArgPtr2 const &arr, Code code);
            void putStmnt_Move_W__Stk_Lit(IR::Exp const *exp);
            void putStmnt_Move_Wx(Core::FastU x);
            void putStmnt_NegF_Wx();
            void putStmnt_NegI_W2();
            void putStmnt_NegI_W3();
            void putStmnt_Retn();
            void putStmnt_ShRU_W();
            void putStmnt_ShLU_W2();
            void putStmnt_ShRI_W2();
            void putStmnt_ShRU_W2();
            void putStmnt_ShLU_W3();
            void putStmnt_ShRI_W3();
            void putStmnt_ShRU_W3();
            void putStmnt_SubU_W();
            void putStmnt_Swap_Wx(Core::FastU x);
            void putStmnt_Xcod_SID();

            void putStmntBitwise2(Code code);
            void putStmntBitwise3(Code code);

            void putStmntCall(Core::String name, Core::FastU ret);

            void putStmntDropArg(IR::Arg const &arg, Core::FastU w);
            void putStmntDropArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

            void putStmntDropRetn(Core::FastU ret);

            void putStmntIncUArg(IR::Arg const &arg, Core::FastU w);
            void putStmntIncUArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

            void putStmntPushArg(IR::Arg const &arg, Core::FastU w);
            void putStmntPushArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

            void putStmntPushFunct(Core::FastU value);

            void putStmntPushRetn(Core::FastU ret);

            void putStmntPushStrEn(Core::FastU value);

            void putStmntShiftRU(Core::FastU shift);

            void putString(Core::String str);
            void putString(Core::String str, Core::FastU key);

            void putWord(Core::FastU i);

            Core::CounterRef<IR::Exp const> resolveGlyph(Core::String glyph);

            virtual void trFunc();

            virtual void trStmnt();
            void trStmnt_AddU_W();
            void trStmnt_Call();
            void trStmnt_CmpU_EQ(IR::Code codeMove);
            void trStmnt_Cscr_IA();
            void trStmnt_Cscr_IS();
            void trStmnt_Cscr_SA();
            void trStmnt_Cscr_SS();
            void trStmnt_Cspe();
            void trStmnt_DiXI_W();
            void trStmnt_InvU_W2();
            void trStmnt_InvU_W3();
            void trStmnt_Move_W();
            void trStmnt_Move_Wx();
            void trStmnt_NegI_W2();
            void trStmnt_NegI_W3();
            void trStmnt_Retn();
            void trStmnt_ShLU_W2();
            void trStmnt_ShLU_W3();
            void trStmnt_ShRI_W2();
            void trStmnt_ShRI_W3();
            void trStmnt_ShRU_W();
            void trStmnt_ShRU_W2();
            void trStmnt_ShRU_W3();
            void trStmnt_SubU_W();
            void trStmnt_Swap_Wx(Core::FastU x);

            void trStmntBitwise2();
            void trStmntBitwise3();
            bool trStmntShift(IR::Code codeMove);
            void trStmntStk2(IR::Code moveDst, IR::Code moveSrc);
            void trStmntStk3(IR::Code moveDst, IR::Code moveSrc, bool ordered);

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

            static Core::FastU GetWord(IR::Arg_Lit const &arg, Core::FastU w = 0);
            static Core::FastU GetWord(IR::Exp const *exp, Core::FastU w = 0);

            static Core::FastU GetScriptValue(IR::Function const &script);

            static bool IsScript(IR::CallType ctype);
            static bool IsScriptS(IR::CallType ctype);
         };
      }
   }
}

#endif//Bytecode__MgC__Info_H__

