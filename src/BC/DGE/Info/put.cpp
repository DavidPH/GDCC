//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2017 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Doominati output.
//
//-----------------------------------------------------------------------------

#include "BC/DGE/Info.hpp"

#include "Core/Exception.hpp"
#include "Core/Option.hpp"

#include "IR/Exp/Binary.hpp"
#include "IR/Exp/Glyph.hpp"
#include "IR/Exp/Unary.hpp"
#include "IR/Program.hpp"

#include "Option/Bool.hpp"

#include <cstdio>

#include <iostream>


//----------------------------------------------------------------------------|
// Options                                                                    |
//

namespace GDCC::BC::DGE
{
   //
   // --bc-dge-origins
   //
   static bool OutputOrigin = false;
   static Option::Bool OutputOriginOpt
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("bc-dge-origins")
         .setGroup("output")
         .setDescS("Output origin data in bytecode."),

      &OutputOrigin
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::DGE
{
   //
   // Info::put
   //
   void Info::put()
   {
      // DGE_NTS header.
      putData("DGE_NTS\0CODEDEFS\0", 18);

      for(auto &itr : prog->rangeDJump())    putDJump(itr);
      for(auto &itr : prog->rangeFunction()) putFunc(itr);
      for(auto &itr : prog->rangeObject())   putObj(itr);
      for(auto &itr : prog->rangeStrEnt())   putStr(itr);
   }

   //
   // Info::putCodeArg
   //
   void Info::putCodeArg(Core::String arg)
   {
      putNTS(arg);
   }

   //
   // Info::putCodeArg
   //
   void Info::putCodeArg(IR::Arg_Lit const &arg)
   {
      putExp(arg.value, arg.off);
   }

   //
   // Info::putDJump
   //
   void Info::putDJump()
   {
      if(!djump->defin) return;

      putNTS("define");
      putNTS(djump->glyph);
      putNTS('=');
      putNTS(djump->label);
      putNTS(';');
   }

   //
   // Info::putExp
   //
   void Info::putExp(IR::Exp const *exp)
   {
      if(exp->isValue())
         return putValue(exp->pos, exp->getValue());

      switch(exp->getName())
      {
      case Core::STR_Add:
         // TODO: Do pointer adjustments as needed.
         putNTS("Add");
         putExp(static_cast<IR::Exp_Binary const *>(exp)->expL);
         putExp(static_cast<IR::Exp_Binary const *>(exp)->expR);
         break;

      case Core::STR_AddPtrRaw:
         putNTS("Add");
         putExp(static_cast<IR::Exp_Binary const *>(exp)->expL);
         putExp(static_cast<IR::Exp_Binary const *>(exp)->expR);
         break;

      case Core::STR_Array:
      case Core::STR_Assoc:
         {
            auto words = getWords(exp);
            auto itr = words.begin(), end = words.end();
            if(itr != end) for(putWord(*itr++); itr != end; ++itr)
               putNTS(','), putWord(*itr);
         }
         break;

      case Core::STR_Cst:
         putExp_Cst(static_cast<IR::Exp_Cst const *>(exp));
         break;

      case Core::STR_Glyph:
         putNTS(static_cast<IR::Exp_Glyph const *>(exp)->glyph);
         break;

      default:
         throw Core::ExceptStr(exp->pos, "bad putExp full");
      }
   }

   //
   // Info::putExp
   //
   void Info::putExp(IR::Exp const *exp, Core::FastU w)
   {
      if(exp->isValue())
         return putValue(exp->pos, exp->getValue(), w);

      if(w)
         throw Core::ExceptStr(exp->pos, "putExp w");

      switch(exp->getName())
      {
      case Core::STR_Add:
         // TODO: Do pointer adjustments as needed.
         putNTS("Add");
         putExp(static_cast<IR::Exp_Binary const *>(exp)->expL, 0);
         putExp(static_cast<IR::Exp_Binary const *>(exp)->expR, 0);
         break;

      case Core::STR_AddPtrRaw:
         putNTS("Add");
         putExp(static_cast<IR::Exp_Binary const *>(exp)->expL, 0);
         putExp(static_cast<IR::Exp_Binary const *>(exp)->expR, 0);
         break;

      case Core::STR_Cst:
         putExp_Cst(static_cast<IR::Exp_Cst const *>(exp));
         break;

      case Core::STR_Glyph:
         putNTS(static_cast<IR::Exp_Glyph const *>(exp)->glyph);
         break;

      default:
         throw Core::ExceptStr(exp->pos, "bad putExp");
      }
   }

   //
   // Info::putExpAdd
   //
   void Info::putExpAdd(IR::Exp const *expL, Core::FastU expR)
   {
      if(expL->isValue())
         return putInt(getWord(expL) + expR);

      putNTS("Add");
      putExp(expL);
      putInt(expR);
   }

   //
   // Info::putExp_Cst
   //
   void Info::putExp_Cst(IR::Exp_Cst const *exp)
   {
      IR::Type const &tIn  = exp->exp->getType();
      IR::Type const &tOut = exp->type;

      switch(tIn.t)
      {
      case IR::TypeBase::Funct:
         switch(tOut.t)
         {
         case IR::TypeBase::Funct:
            putExp(exp->exp);
            break;

         default:
            throw Core::ExceptStr(exp->pos, "unknown putExp_Cst Funct->");
         }
         break;

      case IR::TypeBase::Point:
         switch(tOut.t)
         {
         case IR::TypeBase::Point:
            putExp(exp->exp);
            break;

         default:
            throw Core::ExceptStr(exp->pos, "unknown putExp_Cst Point->");
         }
         break;

      default:
         throw Core::ExceptStr(exp->pos, "unknown putExp_Cst");
      }
   }

   //
   // Info::putFunc
   //
   void Info::putFunc()
   {
      if(!func->defin) return;

      // Put the function header.
      putNTS("function"); putNTS(func->glyph);
      putNTS('{');
         putNTS("param"); putNTS('='); putInt(func->param);         putNTS(';');
         putNTS("label"); putNTS('='); putNTS(func->label);         putNTS(';');
         putNTS("local"); putNTS('='); putInt(func->getLocalReg()); putNTS(';');
         putNTS("retrn"); putNTS('='); putInt(func->retrn);         putNTS(';');
      putNTS('}');

      // Put the function code.
      putNTS("block"); putNTS(func->label);
      putNTS('{');
         putOriginFunc(func->glyph);
         if(func->allocAut)
         {
            if(!func->block.empty())
               putOrigin(func->block.begin()->pos);

            putCode("Push_Lit", func->allocAut);
            putCode("Push_Lit", "___GDCC__Plsa");
            putCode("Call",     1);
            putCode("Drop_Reg", getStkPtrIdx());
         }
         putBlock(func->block);
      putNTS('}');
   }

   //
   // Info::putInt
   //
   void Info::putInt(int i)
   {
      putInt(static_cast<Core::FastU>(i));
   }

   //
   // Info::putInt
   //
   void Info::putInt(unsigned int i)
   {
      putInt(static_cast<Core::FastU>(i));
   }

   //
   // Info::putInt
   //
   void Info::putInt(Core::FastU i)
   {
      char buf[sizeof(std::uintmax_t) * CHAR_BIT / 4 + 1];
      std::size_t len;

      len = std::sprintf(buf, "%jX", static_cast<std::uintmax_t>(i));

      if(!std::isdigit(buf[0]))
         out->put('0'), ++putPos;

      putData(buf, len + 1);
   }

   //
   // Info::putNTS
   //
   void Info::putNTS(char nts)
   {
      out->put(nts);
      out->put('\0');
      putPos += 2;
   }

   //
   // Info::putNTS
   //
   void Info::putNTS(char const *nts)
   {
      putData(nts, std::strlen(nts) + 1);
   }

   //
   // Info::putNTS
   //
   void Info::putNTS(Core::String nts)
   {
      putData(nts.data(), nts.size0() + 1);
   }

   //
   // Info::putObj
   //
   void Info::putObj()
   {
      if(!obj->defin || obj->space.base != IR::AddrBase::Sta)
         return;

      putNTS("global"); putNTS(obj->glyph); putInt(obj->words);

      if(obj->initi && obj->words)
      {
         putNTS('(');
         putExp(obj->initi);
         putNTS(')');
      }
      else
         putNTS(';');
   }

   //
   // Info::putOrigin
   //
   void Info::putOrigin(Core::Origin pos)
   {
      if(!OutputOrigin || !pos) return;

      if(lastOrigin.file != pos.file && pos.file)
      {
         lastOrigin.file = pos.file;
         putNTS("@file"); putNTS(pos.file);
      }

      if(lastOrigin.line != pos.line && pos.line)
      {
         lastOrigin.line = pos.line;
         putNTS("@l"); putInt(static_cast<Core::FastU>(pos.line));
      }
   }

   //
   // Info::putOriginFunc
   //
   void Info::putOriginFunc(Core::String pos)
   {
      if(!OutputOrigin) return;

      putNTS("@func"); putNTS(pos);
   }

   //
   // Info::putStr
   //
   void Info::putStr()
   {
      if(!str->defin) return;

      putNTS("define");
      putNTS(str->glyph);
      putNTS('=');
      putData("{s}", 3); putNTS(str->valueStr);
      putNTS(';');
   }

   //
   // Info::putValue
   //
   void Info::putValue(Core::Origin pos, IR::Value const &val)
   {
      Core::FastU bits;

      switch(val.v)
      {
      case IR::ValueBase::Array:
         putValueMulti(pos, val.vArray.value);
         break;

      case IR::ValueBase::Assoc:
         // TODO: Not assume members are in order.
         putValueMulti(pos, val.vAssoc.value);
         break;

      case IR::ValueBase::Fixed:
         bits = val.vFixed.vtype.getBits();
         putInt(getWord_Fixed(val.vFixed, 0));
         if(bits > 32) for(Core::FastU w = bits / 32, i = 1; i != w; ++i)
            putNTS(','), putInt(getWord_Fixed(val.vFixed, i));
         break;

      case IR::ValueBase::Float:
         bits = val.vFloat.vtype.getBits();
         putInt(getWord_Float(val.vFloat, 0));
         if(bits > 32) for(Core::FastU w = bits / 32, i = 1; i != w; ++i)
            putNTS(','), putInt(getWord_Float(val.vFloat, i));
         break;

      case IR::ValueBase::Funct:
         putInt(val.vFunct.value);
         break;

      case IR::ValueBase::Point:
         putInt(val.vPoint.value);
         break;

      case IR::ValueBase::Tuple:
         putValueMulti(pos, val.vTuple.value);
         break;

      default:
         throw Core::ExceptStr(pos, "bad putValue full");
      }
   }

   //
   // Info::putValue
   //
   void Info::putValue(Core::Origin pos, IR::Value const &val, Core::FastU w)
   {
      putInt(getWord(pos, val, w));
   }

   //
   // Info::putValueMulti
   //
   void Info::putValueMulti(Core::Origin pos, Core::Array<IR::Value> const &val)
   {
      Core::FastU bucket = 0, bucketBits = 0;
      Core::FastU bits;
      bool needComma = false;

      //
      // putComma
      //
      auto putComma = [&]()
      {
         if(needComma)
            putNTS(',');
         needComma = true;
      };

      //
      // flushBucket
      //
      auto flushBucket = [&]()
      {
         putComma();

         if(!bucketBits) return;

         putInt(bucket);

         bucket     = 0;
         bucketBits = 0;
      };

      //
      // putBucket
      //
      auto putBucket = [&](Core::FastU value)
      {
         if(bucketBits + bits > 32)
            flushBucket();

         value &= (static_cast<Core::FastU>(1) << bits) - 1;
         value <<= bucketBits;
         bucket |= value;
         bucketBits += bits;
      };

      for(auto const &v : val)
      {
         switch(v.v)
         {
         case IR::ValueBase::Fixed:
            bits = v.vFixed.vtype.getBits();
            if(bits >= 32) goto defcase;
            putBucket(getWord_Fixed(v.vFixed, 0));
            break;

         default:
         defcase:
            flushBucket();
            putValue(pos, v);
            break;
         }
      }

      flushBucket();
   }

   //
   // Info::putWord
   //
   void Info::putWord(WordValue const &w)
   {
      if(w.exp)
         putExp(w.exp, w.val);
      else
         putInt(w.val);
   }
}

// EOF

