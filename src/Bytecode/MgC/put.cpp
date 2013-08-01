//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// MageCraft output.
//
//-----------------------------------------------------------------------------

#include "Info.hpp"

#include "GDCC/IR/Function.hpp"
#include "GDCC/IR/Object.hpp"

#include "GDCC/IR/Exp/ValueGlyph.hpp"
#include "GDCC/IR/Exp/ValueRoot.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Bytecode
{
   namespace MgC
   {
      //
      // Info::put
      //
      void Info::put(std::ostream &out)
      {
         out << std::hex << "MgC_NTS" << '\0' << "CODEDEFS" << '\0' << '\0';

         for(auto const &itr : GDCC::IR::Space::LocArs.obset)
         {
            if(!itr->exdef)
               putObj(out, *itr);
         }

         for(auto const &itr : GDCC::IR::FunctionRange())
         {
            if(!itr.second.exdef)
               putFunc(out, itr.second);
         }
      }

      //
      // Info::putExp
      //
      void Info::putExp(std::ostream &out, GDCC::IR::Exp const *exp)
      {
         switch(auto s = static_cast<GDCC::StringIndex>(exp->getName()))
         {
         case GDCC::STR_ValueGlyph:
            putGlyph(out, static_cast<GDCC::IR::Exp_ValueGlyph const *>(exp)->glyph);
            break;

         case GDCC::STR_ValueRoot:
            putValue(out, static_cast<GDCC::IR::Exp_ValueRoot const *>(exp)->value);
            break;

         default:
            std::cerr << "bad Exp\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::putFunc
      //
      void Info::putFunc(std::ostream &out, GDCC::IR::Function const &func)
      {
         // Put the function header.
         out << "function" << '\0' << func.glyph << '\0'
            << '{' << '\0'
               << "argCount" << '\0' << '=' << '\0' << func.param    << '\0' << ';' << '\0'
               << "label"    << '\0' << '=' << '\0' << func.label    << '\0' << ';' << '\0'
               << "regCount" << '\0' << '=' << '\0' << func.localReg << '\0' << ';' << '\0'
               << "retCount" << '\0' << '=' << '\0' << func.retrn    << '\0' << ';' << '\0'
            << '}' << '\0';

         // Put the function code.
         out << "code" << '\0' << func.label << '\0'
            << '{' << '\0';

         for(auto const &stmnt : func.block)
            putStmnt(out, stmnt);

         out << '}' << '\0';
      }

      //
      // Info::putGlyph
      //
      void Info::putGlyph(std::ostream &out, GDCC::IR::Glyph glyph)
      {
         out << '$' << static_cast<GDCC::String>(glyph) << '\0';
      }

      //
      // Info::putObj
      //
      void Info::putObj(std::ostream &out, GDCC::IR::Object const &obj)
      {
         out << "data" << '\0' << obj.glyph << '\0' << obj.words << '\0';

         if(obj.initi)
         {
            out << '(' << '\0';
            putObjValue(out, obj.initi->getValue());
            out << ')' << '\0';
         }
         else
            out << ';' << '\0';
      }

      //
      // Info::putObjValue
      //
      void Info::putObjValue(std::ostream &out, GDCC::IR::Value const &val)
      {
         switch(val.v)
         {
         case GDCC::IR::ValueBase::Fixed:
            out << val.vFixed.value << '\0';
            break;

         case GDCC::IR::ValueBase::Multi:
            putObjValue_Multi(out, val.vMulti);
            break;

         default:
            std::cerr << "bad ObjValue\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::putObjValue_Multi
      //
      void Info::putObjValue_Multi(std::ostream &out, GDCC::IR::Value_Multi const &val)
      {
         std::size_t i = 0, e = val.value.size();
         GDCC::FastU bucket = 0, bucketBits = 0;
         GDCC::FastU bits;

         auto flushBucket = [&]()
         {
            if(!bucketBits) return;

            out << bucket << '\0';

            if(i != e) out << ',' << '\0';

            bucket     = 0;
            bucketBits = 0;
         };

         auto writeBucket = [&](GDCC::FastU value)
         {
            if(bucketBits + bits > 32)
               flushBucket();

            value <<= bucketBits;
            bucket |= value;
            bucketBits += bits;
         };

         for(; i != e; ++i) switch(val.value[i].v)
         {
         case GDCC::IR::ValueBase::Fixed:
            bits = val.value[i].vFixed.vtype.getBits();
            if(bits > 32) goto defcase;
            writeBucket(number_cast<GDCC::FastU>(val.value[i].vFixed.value));
            break;

         default:
         defcase:
            flushBucket();
            putObjValue(out, val.value[i]);
            break;
         }

         flushBucket();
      }

      //
      // Info::putStmnt
      //
      void Info::putStmnt(std::ostream &out, GDCC::IR::Statement const &stmnt)
      {
         // Put labels.
         for(auto lab : stmnt.labs)
            out << "label" << '\0' << '(' << '\0' << lab << '\0' << ')' << '\0';

         // Put code and args.
         switch(stmnt.code)
         {
         case GDCC::IR::Code::Nop:
            out << "nop" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         case GDCC::IR::Code::AddU_W:
            out << "addu_stk" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         case GDCC::IR::Code::Call:
            out << "call_stk" << '\0' << '(' << '\0';
               out << (stmnt.args.size() - 2) << '\0';
            out << ')' << '\0';
            break;

         case GDCC::IR::Code::Cjmp_Nil:
            out << "cjmp_stk_nil" << '\0' << '(' << '\0';
               putExp(out, stmnt.args[1].aLit.value);
            out << ')' << '\0';
            break;

         case GDCC::IR::Code::Cjmp_Tru:
            out << "cjmp_stk_tru" << '\0' << '(' << '\0';
               putExp(out, stmnt.args[1].aLit.value);
            out << ')' << '\0';
            break;

         case GDCC::IR::Code::CmpU_EQ_W:
            out << "cmpu_stk_eq" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         case GDCC::IR::Code::CmpU_GE_W:
            out << "cmpu_stk_ge" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         case GDCC::IR::Code::CmpU_GT_W:
            out << "cmpu_stk_gt" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         case GDCC::IR::Code::CmpU_LE_W:
            out << "cmpu_stk_le" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         case GDCC::IR::Code::CmpU_LT_W:
            out << "cmpu_stk_lt" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         case GDCC::IR::Code::CmpU_NE_W:
            out << "cmpu_stk_ne" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         case GDCC::IR::Code::Jump:
            putStmnt_Jump(out, stmnt);
            break;

         case GDCC::IR::Code::Move_W:
            putStmnt_Move_W(out, stmnt);
            break;

         case GDCC::IR::Code::NotU_W:
            out << "notu_stk" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         case GDCC::IR::Code::Retn:
            out << "retn" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         case GDCC::IR::Code::SubU_W:
            out << "subu_stk" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         default:
            std::cerr << "bad Code\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::putStmnt_Jump
      //
      void Info::putStmnt_Jump(std::ostream &out, GDCC::IR::Statement const &stmnt)
      {
         switch(stmnt.args[0].a)
         {
         case GDCC::IR::ArgBase::Lit:
            out << "jump_imm" << '\0' << '(' << '\0';
               putExp(out, stmnt.args[0].aLit.value);
            out << ')' << '\0';
            break;

         case GDCC::IR::ArgBase::Stk:
            out << "jump_stk" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         default:
            std::cerr << "bad Code::Jump(?)\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::putStmnt_Move_W
      //
      void Info::putStmnt_Move_W(std::ostream &out, GDCC::IR::Statement const &stmnt)
      {
         // push_?
         if(stmnt.args[0].a == GDCC::IR::ArgBase::Stk) switch(stmnt.args[1].a)
         {
         case GDCC::IR::ArgBase::Lit:
            out << "push_imm" << '\0' << '(' << '\0';
               putExp(out, stmnt.args[1].aLit.value);
            out << ')' << '\0';
            break;

         case GDCC::IR::ArgBase::LocArs:
            switch(stmnt.args[1].aLocArs.idx->a)
            {
            case GDCC::IR::ArgBase::Lit:
               out << "push_dat" << '\0' << '(' << '\0';
                  out << '+' << '\0';
                     putExp(out, stmnt.args[1].aLocArs.off);
                     putExp(out, stmnt.args[1].aLocArs.idx->aLit.value);
               out << ')' << '\0';
               break;

            case GDCC::IR::ArgBase::Stk:
               out << "push_ptr" << '\0' << '(' << '\0';
                  putExp(out, stmnt.args[1].aLocArs.off);
               out << ')' << '\0';
               break;

            default:
               std::cerr << "bad Code::Move_W(Stk, LocArs(?))\n";
               throw EXIT_FAILURE;
            }
            break;

         case GDCC::IR::ArgBase::LocReg:
            out << "push_reg" << '\0' << '(' << '\0';
               putExp(out, stmnt.args[1].aLocReg.idx->aLit.value);
            out << ')' << '\0';
            break;

         default:
            std::cerr << "bad Code::Move_W(Stk, ?)\n";
            throw EXIT_FAILURE;
         }

         // drop_?
         else if(stmnt.args[1].a == GDCC::IR::ArgBase::Stk) switch(stmnt.args[0].a)
         {
         case GDCC::IR::ArgBase::Nul:
            out << "drop_nul" << '\0' << '(' << '\0' << ')' << '\0';
            break;

         case GDCC::IR::ArgBase::LocArs:
            switch(stmnt.args[0].aLocArs.idx->a)
            {
            case GDCC::IR::ArgBase::Lit:
               out << "drop_dat" << '\0' << '(' << '\0';
                  out << '+' << '\0';
                     putExp(out, stmnt.args[0].aLocArs.off);
                     putExp(out, stmnt.args[0].aLocArs.idx->aLit.value);
               out << ')' << '\0';
               break;

            case GDCC::IR::ArgBase::Stk:
               out << "drop_ptr" << '\0' << '(' << '\0';
                  putExp(out, stmnt.args[0].aLocArs.off);
               out << ')' << '\0';
               break;

            default:
               std::cerr << "bad Code::Move_W(LocArs(?), Stk)\n";
               throw EXIT_FAILURE;
            }
            break;

         case GDCC::IR::ArgBase::LocReg:
            out << "drop_reg" << '\0' << '(' << '\0';
               putExp(out, stmnt.args[0].aLocReg.idx->aLit.value);
            out << ')' << '\0';
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
      void Info::putValue(std::ostream &out, GDCC::IR::Value const &val)
      {
         switch(val.v)
         {
         case GDCC::IR::ValueBase::Fixed:
            out << val.vFixed.value << '\0';
            break;

         default:
            std::cerr << "bad Value\n";
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

