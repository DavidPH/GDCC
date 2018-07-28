//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2018 David Hill
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
         Core::Error(stmnt ? stmnt->pos : func->getOrigin(), "bad getStkPtrIdx");
      }
   }

   //
   // Info::getStmntSizeW
   //
   Core::FastU Info::getStmntSizeW()
   {
      auto n = getStmntSize();

      if(n % 4) Core::Error(stmnt->pos, "expected word statement");

      return n / 4;
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
         Core::Error(pos, "DGE cannot use Arg: ", arg.a);
      }
   }
}

// EOF

