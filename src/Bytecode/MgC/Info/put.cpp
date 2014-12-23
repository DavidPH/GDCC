//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// MageCraft output.
//
//-----------------------------------------------------------------------------

#include "Bytecode/MgC/Info.hpp"

#include "IR/Exp/Binary.hpp"
#include "IR/Exp/Glyph.hpp"
#include "IR/Exp/Multi.hpp"
#include "IR/Exp/Value.hpp"
#include "IR/Program.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Bytecode
   {
      namespace MgC
      {
         //
         // Info::put
         //
         void Info::put()
         {
            *out << std::hex << "MgC_NTS" << '\0' << "CODEDEFS" << '\0' << '\0';

            for(auto &itr : prog->rangeObject())
            {
               if(itr.space.base == IR::AddrBase::LocArs && itr.defin)
                  putObj(itr);
            }

            for(auto &itr : prog->rangeFunction())
            {
               if(itr.defin)
                  putFunc(itr);
            }
         }

         //
         // Info::putExp
         //
         void Info::putExp(IR::Exp const *exp)
         {
            switch(auto s = static_cast<Core::StringIndex>(exp->getName()))
            {
            case Core::STR_Add:
               *out << '+' << '\0';
               putExp(static_cast<IR::Exp_Binary const *>(exp)->expL);
               putExp(static_cast<IR::Exp_Binary const *>(exp)->expR);
               break;

            case Core::STR_Array:
               // Has problems. See Tuple below.
               {
                  auto multi = static_cast<IR::Exp_Array const *>(exp);
                  auto itr = multi->elemV.begin(), end = multi->elemV.end();
                  if(itr != end) for(putExp(*itr++); itr != end; ++itr)
                  {
                     *out << ',' << '\0';
                     putExp(*itr);
                  }
               }
               break;

            case Core::STR_Div:
               *out << '/' << '\0';
               putExp(static_cast<IR::Exp_Binary const *>(exp)->expL);
               putExp(static_cast<IR::Exp_Binary const *>(exp)->expR);
               break;

            case Core::STR_Glyph:
               putGlyph(static_cast<IR::Exp_Glyph const *>(exp)->glyph);
               break;

            case Core::STR_Mod:
               *out << '%' << '\0';
               putExp(static_cast<IR::Exp_Binary const *>(exp)->expL);
               putExp(static_cast<IR::Exp_Binary const *>(exp)->expR);
               break;

            case Core::STR_Mul:
               *out << '*' << '\0';
               putExp(static_cast<IR::Exp_Binary const *>(exp)->expL);
               putExp(static_cast<IR::Exp_Binary const *>(exp)->expR);
               break;

            case Core::STR_Sub:
               *out << '-' << '\0';
               putExp(static_cast<IR::Exp_Binary const *>(exp)->expL);
               putExp(static_cast<IR::Exp_Binary const *>(exp)->expR);
               break;

            case Core::STR_Tuple:
               // This is kind of unfortunate, since it can easily result in
               // incorrect codegen by adding unexpected commas. However, it is
               // needed by putObj and is really a higher level problem.
               // FIXME/TODO: This does need to handle non-words properly, though.
               {
                  auto multi = static_cast<IR::Exp_Tuple const *>(exp);
                  auto itr = multi->elemV.begin(), end = multi->elemV.end();
                  if(itr != end) for(putExp(*itr++); itr != end; ++itr)
                  {
                     *out << ',' << '\0';
                     putExp(*itr);
                  }
               }
               break;

            case Core::STR_Value:
               putValue(static_cast<IR::Exp_Value const *>(exp)->value);
               break;

            default:
               std::cerr << "ERROR: " << exp->pos << ": cannot put Exp for MgC: "
                  << static_cast<Core::String>(s) << "\n";
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::putFunc
         //
         void Info::putFunc()
         {
            // Put the function header.
            *out << "function" << '\0' << func->glyph << '\0'
               << '{' << '\0'
                  << "argCount" << '\0' << '=' << '\0' << func->param    << '\0' << ';' << '\0'
                  << "label"    << '\0' << '=' << '\0' << func->label    << '\0' << ';' << '\0'
                  << "regCount" << '\0' << '=' << '\0' << func->localReg << '\0' << ';' << '\0'
                  << "retCount" << '\0' << '=' << '\0' << func->retrn    << '\0' << ';' << '\0'
               << '}' << '\0';

            // Put the function code.
            *out << "code" << '\0' << func->label << '\0'
               << '{' << '\0';

            putBlock(func->block);

            *out << '}' << '\0';
         }

         //
         // Info::putGlyph
         //
         void Info::putGlyph(IR::Glyph glyph)
         {
            auto s = static_cast<Core::String>(glyph);

            switch(s.data()[0])
            {
            case '@':
            case '$':
            case '%':
            case '&':
            case '*':
            case '{':
            case '"':
            case '\'':
               *out        << s << '\0'; break;
            default:
               *out << '$' << s << '\0'; break;
            }
         }

         //
         // Info::putObj
         //
         void Info::putObj()
         {
            *out << "data" << '\0' << obj->glyph << '\0' << obj->words << '\0';

            if(obj->initi)
            {
               *out << '(' << '\0';

               if(obj->initi->isValue())
                  putObjValue(obj->initi->getValue());
               else
                  putExp(obj->initi);

               *out << ')' << '\0';
            }
            else
               *out << ';' << '\0';
         }

         //
         // Info::putObjValue
         //
         void Info::putObjValue(IR::Value const &val)
         {
            switch(val.v)
            {
            case IR::ValueBase::Array:
               putObjValue_Multi(val.vArray.value);
               break;

            case IR::ValueBase::Fixed:
               *out << val.vFixed.value << '\0';
               break;

            case IR::ValueBase::Tuple:
               putObjValue_Multi(val.vTuple.value);
               break;

            default:
               std::cerr << "bad ObjValue\n";
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::putObjValue_Multi
         //
         void Info::putObjValue_Multi(Core::Array<IR::Value> const &val)
         {
            std::size_t i = 0, e = val.size();
            Core::FastU bucket = 0, bucketBits = 0;
            Core::FastU bits;

            //
            // flushBucket
            //
            auto flushBucket = [&]()
            {
               if(!bucketBits) return;

               *out << bucket << '\0';

               if(i != e) *out << ',' << '\0';

               bucket     = 0;
               bucketBits = 0;
            };

            //
            // writeBucket
            //
            auto writeBucket = [&](Core::FastU value)
            {
               if(bucketBits + bits > 32)
                  flushBucket();

               value <<= bucketBits;
               bucket |= value;
               bucketBits += bits;
            };

            for(; i != e; ++i) switch(val[i].v)
            {
            case IR::ValueBase::Fixed:
               bits = val[i].vFixed.vtype.getBits();
               if(bits > 32) goto defcase;
               writeBucket(Core::NumberCast<Core::FastU>(val[i].vFixed.value));
               break;

            default:
            defcase:
               flushBucket();
               putObjValue(val[i]);
               break;
            }

            flushBucket();
         }

         //
         // Info::putStmnt
         //
         void Info::putStmnt()
         {
            // Put labels.
            for(auto lab : stmnt->labs)
               *out << "label" << '\0' << '(' << '\0' << lab << '\0' << ')' << '\0';

            // Put code and args.
            switch(stmnt->op.code)
            {
            case IR::Code::Nop:
               *out << "nop" << '\0' << '(' << '\0' << ')' << '\0';
               break;

            case IR::Code::AddU_W:
               *out << "addu_stk" << '\0' << '(' << '\0' << ')' << '\0';
               break;

            case IR::Code::AndU_W:
               *out << "andu_stk" << '\0' << '(' << '\0' << ')' << '\0';
               break;

            case IR::Code::Call:
               *out << "call_stk" << '\0' << '(' << '\0';
                  *out << (stmnt->args.size() - 2) << '\0';
               *out << ')' << '\0';
               break;

            case IR::Code::CmpU_EQ_W:
               *out << "cmpu_stk_eq" << '\0' << '(' << '\0' << ')' << '\0';
               break;

            case IR::Code::CmpU_GE_W:
               *out << "cmpu_stk_ge" << '\0' << '(' << '\0' << ')' << '\0';
               break;

            case IR::Code::CmpU_GT_W:
               *out << "cmpu_stk_gt" << '\0' << '(' << '\0' << ')' << '\0';
               break;

            case IR::Code::CmpU_LE_W:
               *out << "cmpu_stk_le" << '\0' << '(' << '\0' << ')' << '\0';
               break;

            case IR::Code::CmpU_LT_W:
               *out << "cmpu_stk_lt" << '\0' << '(' << '\0' << ')' << '\0';
               break;

            case IR::Code::CmpU_NE_W:
               *out << "cmpu_stk_ne" << '\0' << '(' << '\0' << ')' << '\0';
               break;

            case IR::Code::Jcnd_Nil:
               *out << "cjmp_stk_nil" << '\0' << '(' << '\0';
                  putExp(stmnt->args[1].aLit.value);
               *out << ')' << '\0';
               break;

            case IR::Code::Jcnd_Tru:
               *out << "cjmp_stk_tru" << '\0' << '(' << '\0';
                  putExp(stmnt->args[1].aLit.value);
               *out << ')' << '\0';
               break;

            case IR::Code::Jump:
               putStmnt_Jump();
               break;

            case IR::Code::Move_W:
               putStmnt_Move_W();
               break;

            case IR::Code::MulU_W:
               *out << "mulu_stk" << '\0' << '(' << '\0' << ')' << '\0';
               break;

            case IR::Code::NotU_W:
               *out << "notu_stk" << '\0' << '(' << '\0' << ')' << '\0';
               break;

            case IR::Code::Retn:
               *out << "retn" << '\0' << '(' << '\0' << ')' << '\0';
               break;

            case IR::Code::ShRU_W:
               *out << "shru_stk" << '\0' << '(' << '\0' << ')' << '\0';
               break;

            case IR::Code::SubU_W:
               *out << "subu_stk" << '\0' << '(' << '\0' << ')' << '\0';
               break;

            default:
               std::cerr << "bad Code\n";
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::putStmnt_Jump
         //
         void Info::putStmnt_Jump()
         {
            switch(stmnt->args[0].a)
            {
            case IR::ArgBase::Lit:
               *out << "jump_imm" << '\0' << '(' << '\0';
                  putExp(stmnt->args[0].aLit.value);
               *out << ')' << '\0';
               break;

            case IR::ArgBase::Stk:
               *out << "jump_stk" << '\0' << '(' << '\0' << ')' << '\0';
               break;

            default:
               std::cerr << "bad Code::Jump(?)\n";
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::putStmnt_Move_W
         //
         void Info::putStmnt_Move_W()
         {
            // push_?
            if(stmnt->args[0].a == IR::ArgBase::Stk) switch(stmnt->args[1].a)
            {
            case IR::ArgBase::Lit:
               *out << "push_imm" << '\0' << '(' << '\0';
                  putExp(stmnt->args[1].aLit.value);
               *out << ')' << '\0';
               break;

            case IR::ArgBase::LocArs:
               switch(stmnt->args[1].aLocArs.idx->a)
               {
               case IR::ArgBase::Lit:
                  *out << "push_dat" << '\0' << '(' << '\0';
                     *out << '+' << '\0';
                        *out << stmnt->args[1].aLocArs.off;
                        putExp(stmnt->args[1].aLocArs.idx->aLit.value);
                  *out << ')' << '\0';
                  break;

               case IR::ArgBase::Stk:
                  *out << "push_ptr" << '\0' << '(' << '\0';
                     *out << stmnt->args[1].aLocArs.off;
                  *out << ')' << '\0';
                  break;

               default:
                  std::cerr << "bad Code::Move_W(Stk, LocArs(?))\n";
                  throw EXIT_FAILURE;
               }
               break;

            case IR::ArgBase::LocReg:
               *out << "push_reg" << '\0' << '(' << '\0';
                  putExp(stmnt->args[1].aLocReg.idx->aLit.value);
               *out << ')' << '\0';
               break;

            default:
               std::cerr << "bad Code::Move_W(Stk, ?)\n";
               throw EXIT_FAILURE;
            }

            // drop_?
            else if(stmnt->args[1].a == IR::ArgBase::Stk) switch(stmnt->args[0].a)
            {
            case IR::ArgBase::Nul:
               *out << "drop_nul" << '\0' << '(' << '\0' << ')' << '\0';
               break;

            case IR::ArgBase::LocArs:
               switch(stmnt->args[0].aLocArs.idx->a)
               {
               case IR::ArgBase::Lit:
                  *out << "drop_dat" << '\0' << '(' << '\0';
                     *out << '+' << '\0';
                        *out << stmnt->args[0].aLocArs.off;
                        putExp(stmnt->args[0].aLocArs.idx->aLit.value);
                  *out << ')' << '\0';
                  break;

               case IR::ArgBase::Stk:
                  *out << "drop_ptr" << '\0' << '(' << '\0';
                     *out << stmnt->args[0].aLocArs.off;
                  *out << ')' << '\0';
                  break;

               default:
                  std::cerr << "bad Code::Move_W(LocArs(?), Stk)\n";
                  throw EXIT_FAILURE;
               }
               break;

            case IR::ArgBase::LocReg:
               *out << "drop_reg" << '\0' << '(' << '\0';
                  putExp(stmnt->args[0].aLocReg.idx->aLit.value);
               *out << ')' << '\0';
               break;

            default:
               std::cerr << "bad Code::Move_W(?, Stk)\n";
               throw EXIT_FAILURE;
            }

            // ???
            else
            {
               std::cerr << "bad Code::Move_W\n";
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::putValue
         //
         void Info::putValue(IR::Value const &val)
         {
            switch(val.v)
            {
            case IR::ValueBase::Fixed:
               *out << val.vFixed.value << '\0';
               break;

            default:
               std::cerr << "bad Value\n";
               throw EXIT_FAILURE;
            }
         }
      }
   }
}

// EOF

