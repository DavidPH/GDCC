//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Target information and handling base class.
//
//-----------------------------------------------------------------------------

#include "BC/Info.hpp"

#include "Core/Exception.hpp"

#include "IR/Exp/Multi.hpp"
#include "IR/Program.hpp"

#include "Platform/Platform.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC
{
   //
   // Info::getWord
   //
   Core::FastU Info::getWord(IR::Arg_Lit const &arg, Core::FastU w)
   {
      return getWord(arg.value, arg.off + w);
   }

   //
   // Info::getWord
   //
   Core::FastU Info::getWord(IR::Exp const *exp, Core::FastU w)
   {
      return getWord(exp->pos, exp->getValue(), w);
   }

   //
   // Info::getWord
   //
   Core::FastU Info::getWord(Core::Origin pos, IR::Value const &val, Core::FastU w)
   {
      switch(val.v)
      {
      case IR::ValueBase::DJump: return w ? 0 : val.vDJump.value;
      case IR::ValueBase::Fixed: return getWord_Fixed(val.vFixed, w);
      case IR::ValueBase::Float: return getWord_Float(val.vFloat, w);
      case IR::ValueBase::Funct: return w ? 0 : val.vFunct.value;
      case IR::ValueBase::Point: return w ? 0 : val.vPoint.value;
      case IR::ValueBase::StrEn: return w ? 0 : val.vStrEn.value;

      case IR::ValueBase::Empty:
         throw Core::ExceptStr(pos, "bad getWord Value: Empty");
      case IR::ValueBase::Array:
         throw Core::ExceptStr(pos, "bad getWord Value: Array");
      case IR::ValueBase::Assoc:
         throw Core::ExceptStr(pos, "bad getWord Value: Assoc");
      case IR::ValueBase::Tuple:
         throw Core::ExceptStr(pos, "bad getWord Value: Tuple");
      case IR::ValueBase::Union:
         throw Core::ExceptStr(pos, "bad getWord Value: Union");
      }

      throw Core::ExceptStr(pos, "bad getWord Value");
   }

   //
   // Info::getWord_Fixed
   //
   Core::FastU Info::getWord_Fixed(IR::Value_Fixed const &val, Core::FastU w)
   {
      Core::FastU valI;

      if(w)
      {
         if(val.vtype.bitsS)
            valI = Core::NumberCast<Core::FastI>(val.value >> (w * 32));
         else
            valI = Core::NumberCast<Core::FastU>(val.value >> (w * 32));
      }
      else
      {
         if(val.vtype.bitsS)
            valI = Core::NumberCast<Core::FastI>(val.value);
         else
            valI = Core::NumberCast<Core::FastU>(val.value);
      }

      return valI & 0xFFFFFFFF;
   }

   //
   // Info::getWord_Float
   //
   Core::FastU Info::getWord_Float(IR::Value_Float const &val, Core::FastU w)
   {
      // Special handling for 0.
      // TODO: Negative zero. May require replacing mpf_class.
      if(!val.value) return 0;

      // Convert float to binary string.
      // The sign and mantissa bits are stored in the string, while the
      // exponent is stored in exp. The string includes the implicit
      // leading 1 which will be skipped.
      std::unique_ptr<char[]> buf{new char[val.vtype.bitsI + 3]};
      mp_exp_t                exp;
      mpf_get_str(buf.get(), &exp, 2, val.vtype.bitsI + 1, val.value.get_mpf_t());

      Core::Integ valI = 0;

      // Sign bit.
      auto start = buf.get();
      if(*start == '-') valI |= 1, ++start;

      // Exponent bits.
      exp += (1 << (val.vtype.bitsF - 1)) - 2;
      exp &= (1 << (val.vtype.bitsF    )) - 1;

      valI <<= val.vtype.bitsF;
      valI |= exp;

      // Mantissa bits. Skip first bit because it is an implicit 1.
      auto itr = ++start;
      for(; *itr; ++itr) {valI <<= 1; if(*itr == '1') ++valI;}
      valI <<= val.vtype.bitsI - (itr - start);

      // Return requested word.
      if(w) valI >>= w * 32;
      return Core::NumberCast<Core::FastU>(valI) & 0xFFFFFFFF;
   }

   //
   // Info::getWordCount
   //
   Core::FastU Info::getWordCount(IR::Type const &type)
   {
      switch(type.t)
      {
      case IR::TypeBase::Array:
         return getWordCount(*type.tArray.elemT) * type.tArray.elemC;
      case IR::TypeBase::Assoc: return getWordCount_Assoc(type.tAssoc);
      case IR::TypeBase::DJump: return 1;
      case IR::TypeBase::Empty: return 0;
      case IR::TypeBase::Fixed: return (type.tFixed.getBits() + 31) / 32;
      case IR::TypeBase::Float: return (type.tFloat.getBits() + 31) / 32;
      case IR::TypeBase::Funct: return 1;
      case IR::TypeBase::Point: return getWordCount_Point(type.tPoint);
      case IR::TypeBase::StrEn: return 1;
      case IR::TypeBase::Tuple: return getWordCount_Tuple(type.tTuple);
      case IR::TypeBase::Union: return getWordCount_Union(type.tUnion);
      }

      throw Core::ExceptStr({}, "bad getWordCount Type");
   }

   //
   // Info::getWordCount_Assoc
   //
   Core::FastU Info::getWordCount_Assoc(IR::Type_Assoc const &type)
   {
      Core::FastU size = 0;

      for(auto const &a : type.assoc)
         size = std::max(size, a.addr / Platform::GetWordBytes() + getWordCount(a.type));

      return size;
   }

   //
   // Info::getWordCount_Point
   //
   Core::FastU Info::getWordCount_Point(IR::Type_Point const &)
   {
      // TODO: Common values for certain address spaces.
      return 1;
   }

   //
   // Info::getWordCount_Tuple
   //
   Core::FastU Info::getWordCount_Tuple(IR::Type_Tuple const &type)
   {
      Core::FastU size = 0;

      for(auto const &t : type.types)
         size += getWordCount(t);

      return size;
   }

   //
   // Info::getWordCount_Union
   //
   Core::FastU Info::getWordCount_Union(IR::Type_Union const &type)
   {
      Core::FastU size = 0;

      for(auto const &t : type.types)
         size = std::max(size, getWordCount(t));

      return size;
   }

   //
   // Info::getWords
   //
   Info::WordArray Info::getWords(IR::Arg_Lit const &arg)
   {
      return getWords(arg.value);
   }

   //
   // Info::getWords
   //
   Info::WordArray Info::getWords(IR::Exp const *exp)
   {
      if(exp->isValue())
         return getWords(exp->pos, exp->getValue());

      WordArray   words;
      Core::FastU size;

      switch(exp->getName())
      {
      case Core::STR_Assoc:
         return getWords_Assoc(static_cast<IR::Exp_Assoc const *>(exp));

      default:
         size  = getWordCount(exp->getType());
         words = WordArray{size};

         for(Core::FastU w = 0; w != size; ++w)
            words[w] = {exp, w};

         return words;
      }
   }

   //
   // Info::getWords
   //
   Info::WordArray Info::getWords(Core::Origin pos, IR::Value const &val)
   {
      Core::FastU size = getWordCount(val.getType());
      WordArray   words{size};

      for(Core::FastU w = 0; w != size; ++w)
         words[w] = {nullptr, getWord(pos, val, w)};

      return words;
   }

   //
   // Info::getWords_Assoc
   //
   Info::WordArray Info::getWords_Assoc(IR::Exp_Assoc const *exp)
   {
      Core::FastU size = getWordCount(exp->getType());
      WordArray   words{size};

      for(std::size_t i = 0; i != exp->elemV.size(); ++i)
      {
         Core::FastU idx = exp->elemT.assoc[i].addr / Platform::GetWordBytes();

         auto w = getWords(exp->elemV[i]);

         std::move(w.begin(), w.end(), &words[idx]);
      }

      return words;
   }
}

// EOF

