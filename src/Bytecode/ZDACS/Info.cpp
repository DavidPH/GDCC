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

#include "Info.hpp"

#include "GDCC/IR/Program.hpp"

#include "Option/Option.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace Bytecode
{
   namespace ZDACS
   {
      GDCC::IR::Type_Fixed const Info::TypeWord{32, 0, false, false};

      bool Info::UseChunkSTRE = false;
      bool Info::UseFakeACS0  = false;
   }
}

namespace Option
{
   //
   // --bc-zdacs-chunk-STRE
   //
   OptionDPtr<bool> ChunkSTREOpt{'\0', "bc-zdacs-chunk-STRE", "output",
      "Generates an encrypted string table.", nullptr,
      &Bytecode::ZDACS::Info::UseChunkSTRE};

   //
   // --bc-zdacs-fake-ACS0
   //
   OptionDPtr<bool> FakeACS0Opt{'\0', "bc-zdacs-fake-ACS0", "output",
      "Generates a fake ACS0 header.", nullptr,
      &Bytecode::ZDACS::Info::UseFakeACS0};
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Bytecode
{
   namespace ZDACS
   {
      //
      // Info default constructor
      //
      Info::Info() :
         numChunkAIMP{0},
         numChunkAINI{0},
         numChunkARAY{0},
         numChunkASTR{0},
         numChunkATAG{0},
         numChunkCODE{0},
         numChunkFNAM{0},
         numChunkFUNC{0},
         numChunkLOAD{0},
         numChunkMEXP{0},
         numChunkMIMP{0},
         numChunkMINI{0},
         numChunkMSTR{0},
         numChunkSFLG{0},
         numChunkSNAM{0},
         numChunkSPTR{0},
         numChunkSTRL{0},
         numChunkSVCT{0}
      {
      }

      //
      // Info::backGlyphFunc
      //
      void Info::backGlyphFunc(GDCC::String glyph, GDCC::FastU val, GDCC::IR::CallType ctype)
      {
         auto &data = prog->getGlyphData(glyph);

         data.type  = GDCC::IR::Type_Funct(ctype);
         data.value = GDCC::IR::ExpCreate_ValueRoot(
            GDCC::IR::Value_Funct(val, GDCC::IR::Type_Funct(ctype)),
            GDCC::Origin(GDCC::STRNULL, 0));
      }

      //
      // Info::backGlyphGlyph
      //
      void Info::backGlyphGlyph(GDCC::String glyph, GDCC::String val)
      {
         auto &data = prog->getGlyphData(glyph);

         data.type  = prog->getGlyphData(val).type;
         data.value = GDCC::IR::ExpCreate_ValueGlyph(
            GDCC::IR::Glyph(prog, val),
            GDCC::Origin(GDCC::STRNULL, 0));
      }

      //
      // Info::backGlyphStr
      //
      void Info::backGlyphStr(GDCC::String glyph, GDCC::FastU val)
      {
         auto &data = prog->getGlyphData(glyph);

         data.type  = GDCC::IR::Type_StrEn();
         data.value = GDCC::IR::ExpCreate_ValueRoot(
            GDCC::IR::Value_StrEn(val, GDCC::IR::Type_StrEn()),
            GDCC::Origin(GDCC::STRNULL, 0));
      }

      //
      // Info::backGlyphWord
      //
      void Info::backGlyphWord(GDCC::String glyph, GDCC::FastU val)
      {
         auto &data = prog->getGlyphData(glyph);

         data.type  = TypeWord;
         data.value = GDCC::IR::ExpCreate_ValueRoot(
            GDCC::IR::Value_Fixed(val, TypeWord), GDCC::Origin(GDCC::STRNULL, 0));
      }

      //
      // Info::lenString
      //
      std::size_t Info::lenString(GDCC::String str)
      {
         auto const &data = str.getData();
         std::size_t len = 0;

         for(auto i = data.str, e = i + data.len; i != e; ++i) switch(*i)
         {
         case '\0':
            if('0' <= i[1] && i[1] <= '7')
               len += 4;
            else
               len += 2;
            break;

         case '\\':
            len += 2;
            break;

         default:
            len += 1;
            break;
         }

         return len + 1;
      };

      //
      // Info::resolveGlyph
      //
      GDCC::IR::Exp::CRef Info::resolveGlyph(GDCC::String glyph)
      {
         if(auto exp = prog->getGlyphData(glyph).value)
            return static_cast<GDCC::IR::Exp::CRef>(exp);

         std::cerr << "ERROR: unbacked glyph: '" << glyph << "'\n";
         throw EXIT_FAILURE;
      }

      //
      // Info::CheckArg
      //
      void Info::CheckArg(GDCC::IR::Arg const &arg, GDCC::Origin pos)
      {
         switch(arg.a)
         {
         case GDCC::IR::ArgBase::GblArr:
            CheckArgB(*arg.aGblArr.arr, GDCC::IR::ArgBase::Lit, pos);
            CheckArg (*arg.aGblArr.idx, pos);
            break;

         case GDCC::IR::ArgBase::GblReg:
            CheckArgB(*arg.aGblReg.idx, GDCC::IR::ArgBase::Lit, pos);
            break;

         case GDCC::IR::ArgBase::Lit: break;
         case GDCC::IR::ArgBase::Nul: break;
         case GDCC::IR::ArgBase::Stk: break;

         case GDCC::IR::ArgBase::LocReg:
            CheckArgB(*arg.aLocReg.idx, GDCC::IR::ArgBase::Lit, pos);
            break;

         case GDCC::IR::ArgBase::MapArr:
            CheckArgB(*arg.aMapArr.arr, GDCC::IR::ArgBase::Lit, pos);
            CheckArg (*arg.aMapArr.idx, pos);
            break;

         case GDCC::IR::ArgBase::MapReg:
            CheckArgB(*arg.aMapReg.idx, GDCC::IR::ArgBase::Lit, pos);
            break;

         case GDCC::IR::ArgBase::WldArr:
            CheckArgB(*arg.aWldArr.arr, GDCC::IR::ArgBase::Lit, pos);
            CheckArg (*arg.aWldArr.idx, pos);
            break;

         case GDCC::IR::ArgBase::WldReg:
            CheckArgB(*arg.aWldReg.idx, GDCC::IR::ArgBase::Lit, pos);
            break;

         default:
            std::cerr << "ERROR: " << pos << ": ZDACS cannot use Arg: " << arg.a << '\n';
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::CodeBase
      //
      GDCC::FastU Info::CodeBase()
      {
         return 24;
      }

      //
      // Info::ResolveValue
      //
      GDCC::FastU Info::ResolveValue(GDCC::IR::Value const &val)
      {
         switch(val.v)
         {
         case GDCC::IR::ValueBase::Fixed:
            if(val.vFixed.vtype.bitsS)
               return number_cast<GDCC::FastI>(val.vFixed.value);
            else
               return number_cast<GDCC::FastU>(val.vFixed.value);

         case GDCC::IR::ValueBase::Funct:
             return val.vFunct.value;

         case GDCC::IR::ValueBase::StrEn:
             return val.vFunct.value;

         default:
            std::cerr << "ERROR: bad Value\n";
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

