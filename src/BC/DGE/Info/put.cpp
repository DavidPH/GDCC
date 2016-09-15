//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
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

#include "IR/Exp/Binary.hpp"
#include "IR/Exp/Glyph.hpp"
#include "IR/Exp/Unary.hpp"
#include "IR/Program.hpp"

#include <cstdio>

#include <iostream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace BC
   {
      namespace DGE
      {
         //
         // Info::put
         //
         void Info::put()
         {
            // DGE_NTS header.
            putData("DGE_NTS\0CODEDEFS\0", 18);

            for(auto &itr : prog->rangeFunction()) putFunc(itr);
            for(auto &itr : prog->rangeObject())   putObj(itr);
         }

         //
         // Info::putCode
         //
         void Info::putCode(char const *code)
         {
            putNTS(code);
            putNTS('(');
            putNTS(')');
         }

         //
         // Info::putCode
         //
         void Info::putCode(char const *code, char const *arg0)
         {
            putNTS(code);
            putNTS('(');
            putNTS(arg0);
            putNTS(')');
         }

         //
         // Info::putCode
         //
         void Info::putCode(char const *code, Core::FastU arg0)
         {
            putNTS(code);
            putNTS('(');
            putInt(arg0);
            putNTS(')');
         }

         //
         // Info::putCode
         //
         void Info::putCode(char const *code, Core::FastU arg0, IR::Arg_Lit const &arg1)
         {
            putNTS(code);
            putNTS('(');
            putInt(arg0);
            putNTS(',');
            putExp(arg1.value, arg1.off);
            putNTS(')');
         }

         //
         // Info::putCode
         //
         void Info::putCode(char const *code, IR::Arg_Lit const &arg0)
         {
            putNTS(code);
            putNTS('(');
            putExp(arg0.value, arg0.off);
            putNTS(')');
         }

         //
         // Info::putExp
         //
         void Info::putExp(IR::Exp const *exp)
         {
            if(exp->isValue())
               return putValue(exp->getValue());

            switch(exp->getName())
            {
            case Core::STR_Add:
               putNTS("Add");
               putExp(static_cast<IR::Exp_Binary const *>(exp)->expL);
               putExp(static_cast<IR::Exp_Binary const *>(exp)->expR);
               break;

            case Core::STR_Cst:
               putExp_Cst(static_cast<IR::Exp_Cst const *>(exp));
               break;

            case Core::STR_Glyph:
               putNTS(static_cast<IR::Exp_Glyph const *>(exp)->glyph);
               break;

            default:
               throw Core::ExceptStr(exp->pos, "bad getExp");
            }
         }

         //
         // Info::putExp
         //
         void Info::putExp(IR::Exp const *exp, Core::FastU w)
         {
            if(exp->isValue())
               return putValue(exp->getValue(), w);

            if(w)
               throw Core::ExceptStr(exp->pos, "putExp w");

            switch(exp->getName())
            {
            case Core::STR_Add:
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
               throw Core::ExceptStr(exp->pos, "bad getExp");
            }
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
            case IR::TypeBase::Point:
               switch(tOut.t)
               {
               case IR::TypeBase::Point:
                  putExp(exp->exp);
                  break;

               default:
                  throw Core::ExceptStr(exp->pos, "unknown getExp_Cst Point->");
               }
               break;

            default:
               throw Core::ExceptStr(exp->pos, "unknown getExp_Cst");
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
               if(func->allocAut)
               {
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
         // Info::putValue
         //
         void Info::putValue(IR::Value const &val)
         {
            Core::FastU bits;

            switch(val.v)
            {
            case IR::ValueBase::Array:
               putValueMulti(val.vArray.value);
               break;

            case IR::ValueBase::Fixed:
               bits = val.vFixed.vtype.getBits();
               putInt(GetWord_Fixed(val.vFixed, 0));
               if(bits > 32) for(Core::FastU w = bits / 32, i = 1; i != w; ++i)
                  putInt(GetWord_Fixed(val.vFixed, i));
               break;

            case IR::ValueBase::Point:
               putInt(val.vPoint.value);
               break;

            case IR::ValueBase::Tuple:
               putValueMulti(val.vTuple.value);
               break;

            default:
               throw Core::ExceptStr({}, "bad putValue full");
            }
         }

         //
         // Info::putValue
         //
         void Info::putValue(IR::Value const &val, Core::FastU w)
         {
            switch(val.v)
            {
            case IR::ValueBase::Fixed:
               putInt(GetWord_Fixed(val.vFixed, w));
               break;

            case IR::ValueBase::Point:
               putInt(w == 0 ? val.vPoint.value : 0);
               break;

            default:
               throw Core::ExceptStr({}, "bad putValue");
            }
         }

         //
         // Info::putValueMulti
         //
         void Info::putValueMulti(Core::Array<IR::Value> const &val)
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
                  putBucket(GetWord_Fixed(v.vFixed, 0));
                  break;

               default:
               defcase:
                  flushBucket();
                  putValue(v);
                  break;
               }
            }
         }
      }
   }
}

// EOF

