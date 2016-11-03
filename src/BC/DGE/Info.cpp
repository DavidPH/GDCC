//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Doominati target information and handling class.
//
//-----------------------------------------------------------------------------

#include "BC/DGE/Info.hpp"

#include "Core/Exception.hpp"

#include "IR/CallType.hpp"
#include "IR/Program.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::DGE
{
   //
   // Info::backGlyphObj
   //
   void Info::backGlyphObj(Core::String glyph, Core::FastU val)
   {
      auto &data = prog->getGlyphData(glyph);

      data.value = IR::ExpCreate_Value(IR::Value_Point(val,
         data.type.tPoint.reprB, data.type.tPoint.reprN, data.type.tPoint), {nullptr, 0});
   }

   //
   // Info::getFixedInfo
   //
   FixedInfo Info::getFixedInfo(Core::FastU n, bool s)
   {
      FixedInfo fi;

      fi.bitsS = s;

      switch(n)
      {
      case  1: fi.bitsF =  7; break;
      case  2: fi.bitsF = 15; break;
      default: fi.bitsF = 32; break;
      }

      fi.bitsI = n * 32 - fi.bitsF - fi.bitsS;

      fi.wordsF = (fi.bitsF + 31) / 32;
      fi.wordsI = (fi.bitsI + 31) / 32;

      return fi;
   }

   //
   // Info::getStkPtrIdx
   //
   Core::FastU Info::getStkPtrIdx()
   {
      switch(func->ctype)
      {
      case IR::CallType::StdCall:
         return 0;

      case IR::CallType::StkCall:
         if(func->allocAut)
            return (func->localReg - 1) * 4;

         goto case_def;

      default:
      case_def:
         std::cerr << "ERROR: " << stmnt->pos << ": bad getStkPtrIdx\n";
         throw EXIT_FAILURE;
      }
   }

   //
   // Info::isDropArg
   //
   bool Info::isDropArg(IR::Arg const &arg)
   {
      switch(arg.a)
      {
      case IR::ArgBase::Aut:    return isPushArg(*arg.aAut.idx);
      case IR::ArgBase::LocReg: return true;
      case IR::ArgBase::Nul:    return true;
      case IR::ArgBase::Sta:    return isPushArg(*arg.aSta.idx);
      default:                  return false;
      }
   }

   //
   // Info::isPushArg
   //
   bool Info::isPushArg(IR::Arg const &arg)
   {
      switch(arg.a)
      {
      case IR::ArgBase::Aut:    return isPushArg(*arg.aAut.idx);
      case IR::ArgBase::Lit:    return true;
      case IR::ArgBase::LocReg: return true;
      case IR::ArgBase::Sta:    return isPushArg(*arg.aSta.idx);
      default:                  return false;
      }
   }

   //
   // Info::CheckArg
   //
   void Info::CheckArg(IR::Arg const &arg, Core::Origin const &pos)
   {
      switch(arg.a)
      {
      case IR::ArgBase::Aut:
         CheckArg(*arg.aAut.idx, pos);
         break;

      case IR::ArgBase::Lit: break;
      case IR::ArgBase::Nul: break;
      case IR::ArgBase::Stk: break;

      case IR::ArgBase::LocReg:
         CheckArgB(*arg.aLocReg.idx, IR::ArgBase::Lit, pos);
         break;

      case IR::ArgBase::Sta:
         CheckArg(*arg.aSta.idx, pos);
         break;

      case IR::ArgBase::StrArs:
         CheckArg(*arg.aStrArs.idx, pos);
         break;

      default:
         std::cerr << "ERROR: " << pos << ": DGE cannot use Arg: "
            << arg.a << '\n';
         throw EXIT_FAILURE;
      }
   }

   //
   // Info::GetWord
   //
   Core::FastU Info::GetWord(IR::Arg_Lit const &arg, Core::FastU w)
   {
      return GetWord(arg.value, arg.off + w);
   }

   //
   // Info::GetWord
   //
   Core::FastU Info::GetWord(IR::Exp const *exp, Core::FastU w)
   {
      auto val = exp->getValue();

      switch(val.v)
      {
      case IR::ValueBase::DJump: return w ? 0 : val.vDJump.value;
      case IR::ValueBase::Fixed: return GetWord_Fixed(val.vFixed, w);
      case IR::ValueBase::Funct: return w ? 0 : val.vFunct.value;
      case IR::ValueBase::Point: return w ? 0 : val.vPoint.value;
      case IR::ValueBase::StrEn: return w ? 0 : val.vStrEn.value;

      case IR::ValueBase::Empty:
         throw Core::ExceptStr(exp->pos, "bad GetWord Value: Empty");
      case IR::ValueBase::Array:
         throw Core::ExceptStr(exp->pos, "bad GetWord Value: Array");
      case IR::ValueBase::Assoc:
         throw Core::ExceptStr(exp->pos, "bad GetWord Value: Assoc");
      case IR::ValueBase::Float:
         throw Core::ExceptStr(exp->pos, "bad GetWord Value: Float");
      case IR::ValueBase::Tuple:
         throw Core::ExceptStr(exp->pos, "bad GetWord Value: Tuple");
      case IR::ValueBase::Union:
         throw Core::ExceptStr(exp->pos, "bad GetWord Value: Union");
      }

      throw Core::ExceptStr(exp->pos, "bad GetWord Value");
   }

   //
   // GetWord_Fixed
   //
   Core::FastU Info::GetWord_Fixed(IR::Value_Fixed const &val, Core::FastU w)
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
}

// EOF

