//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
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

#include "IR/Program.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// TryPointer
//
#define TryPointer(fun, ptr) \
   try \
   { \
      ptr = &ptr##_; \
      fun(); \
      ptr = nullptr; \
   } \
   catch(...) \
   { \
      ptr = nullptr; \
      throw; \
   }


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC
{
   //
   // Info::gen
   //
   void Info::gen()
   {
      for(auto &itr : prog->rangeSpaceGblArs()) genSpace(itr);
      for(auto &itr : prog->rangeSpaceHubArs()) genSpace(itr);
      for(auto &itr : prog->rangeSpaceLocArs()) genSpace(itr);
      for(auto &itr : prog->rangeSpaceModArs()) genSpace(itr);

      genSpace(prog->getSpaceGblReg());
      genSpace(prog->getSpaceHubReg());
      genSpace(prog->getSpaceModReg());
      genSpace(prog->getSpaceSta());

      for(auto &itr : prog->rangeFunction()) genFunc(itr);

      for(auto &itr : prog->rangeDJump())  genDJump(itr);
      for(auto &itr : prog->rangeObject()) genObj(itr);
      for(auto &itr : prog->rangeStrEnt()) genStr(itr);
   }

   //
   // Info::gen
   //
   void Info::gen(IR::Program &prog_)
   {
      TryPointer(gen, prog);
   }

   //
   // Info::genBlock
   //
   void Info::genBlock()
   {
      for(auto &stmnt_ : *block)
         genStmnt(stmnt_);
   }

   //
   // Info::genBlock
   //
   void Info::genBlock(IR::Block &block_)
   {
      TryPointer(genBlock, block);
   }

   //
   // Info::genDJump
   //
   void Info::genDJump(IR::DJump &djump_)
   {
      TryPointer(genDJump, djump);
   }

   //
   // Info::genFunc
   //
   void Info::genFunc()
   {
      genBlock(func->block);
   }

   //
   // Info::genFunc
   //
   void Info::genFunc(IR::Function &func_)
   {
      TryPointer(genFunc, func);
   }

   //
   // Info::genObj
   //
   void Info::genObj(IR::Object &obj_)
   {
      TryPointer(genObj, obj);
   }

   //
   // Info::genSpace
   //
   void Info::genSpace(IR::Space &space_)
   {
      TryPointer(genSpace, space);
   }

   //
   // Info::genStmnt
   //
   void Info::genStmnt(IR::Statement &stmnt_)
   {
      TryPointer(genStmnt, stmnt);
   }

   //
   // Info::genStr
   //
   void Info::genStr(IR::StrEnt &str_)
   {
      TryPointer(genStr, str);
   }

   //
   // Info::opt
   //
   void Info::opt()
   {
      for(auto &itr : prog->rangeSpaceGblArs()) optSpace(itr);
      for(auto &itr : prog->rangeSpaceHubArs()) optSpace(itr);
      for(auto &itr : prog->rangeSpaceLocArs()) optSpace(itr);
      for(auto &itr : prog->rangeSpaceModArs()) optSpace(itr);

      optSpace(prog->getSpaceGblReg());
      optSpace(prog->getSpaceHubReg());
      optSpace(prog->getSpaceModReg());
      optSpace(prog->getSpaceSta());

      for(;;) try
      {
         for(auto &itr : prog->rangeFunction()) optFunc(itr);
         break;
      }
      catch(ResetFunc const &) {}

      for(auto &itr : prog->rangeDJump())  optDJump(itr);
      for(auto &itr : prog->rangeObject()) optObj(itr);
      for(auto &itr : prog->rangeStrEnt()) optStr(itr);
   }

   //
   // Info::opt
   //
   void Info::opt(IR::Program &prog_)
   {
      TryPointer(opt, prog);
   }

   //
   // Info::optBlock
   //
   void Info::optBlock()
   {
      for(auto &stmnt_ : *block)
         optStmnt(stmnt_);
   }

   //
   // Info::optBlock
   //
   void Info::optBlock(IR::Block &block_)
   {
      TryPointer(optBlock, block);
   }

   //
   // Info::optDJump
   //
   void Info::optDJump(IR::DJump &djump_)
   {
      TryPointer(optDJump, djump);
   }

   //
   // Info::optFunc
   //
   void Info::optFunc()
   {
      optBlock(func->block);
   }

   //
   // Info::optFunc
   //
   void Info::optFunc(IR::Function &func_)
   {
      TryPointer(optFunc, func);
   }

   //
   // Info::optObj
   //
   void Info::optObj(IR::Object &obj_)
   {
      TryPointer(optObj, obj);
   }

   //
   // Info::optSpace
   //
   void Info::optSpace(IR::Space &space_)
   {
      TryPointer(optSpace, space);
   }

   //
   // Info::optStmnt
   //
   void Info::optStmnt(IR::Statement &stmnt_)
   {
      TryPointer(optStmnt, stmnt);
   }

   //
   // Info::optStr
   //
   void Info::optStr(IR::StrEnt &str_)
   {
      TryPointer(optStr, str);
   }

   //
   // Info::pre
   //
   void Info::pre()
   {
      for(auto &itr : prog->rangeSpaceGblArs()) preSpace(itr);
      for(auto &itr : prog->rangeSpaceHubArs()) preSpace(itr);
      for(auto &itr : prog->rangeSpaceLocArs()) preSpace(itr);
      for(auto &itr : prog->rangeSpaceModArs()) preSpace(itr);

      preSpace(prog->getSpaceGblReg());
      preSpace(prog->getSpaceHubReg());
      preSpace(prog->getSpaceModReg());
      preSpace(prog->getSpaceSta());

      for(;;) try
      {
         for(auto &itr : prog->rangeFunction()) preFunc(itr);
         break;
      }
      catch(ResetFunc const &) {}

      for(auto &itr : prog->rangeDJump())  preDJump(itr);
      for(auto &itr : prog->rangeObject()) preObj(itr);
      for(auto &itr : prog->rangeStrEnt()) preStr(itr);
   }

   //
   // Info::pre
   //
   void Info::pre(IR::Program &prog_)
   {
      TryPointer(pre, prog);
   }

   //
   // Info::preBlock
   //
   void Info::preBlock()
   {
      for(auto &stmnt_ : *block)
         preStmnt(stmnt_);
   }

   //
   // Info::preBlock
   //
   void Info::preBlock(IR::Block &block_)
   {
      TryPointer(preBlock, block);
   }

   //
   // Info::preDJump
   //
   void Info::preDJump(IR::DJump &djump_)
   {
      TryPointer(preDJump, djump);
   }

   //
   // Info::preFunc
   //
   void Info::preFunc()
   {
      preBlock(func->block);
   }

   //
   // Info::preFunc
   //
   void Info::preFunc(IR::Function &func_)
   {
      TryPointer(preFunc, func);
   }

   //
   // Info::preObj
   //
   void Info::preObj(IR::Object &obj_)
   {
      TryPointer(preObj, obj);
   }

   //
   // Info::preSpace
   //
   void Info::preSpace(IR::Space &space_)
   {
      TryPointer(preSpace, space);
   }

   //
   // Info::preStmnt
   //
   void Info::preStmnt(IR::Statement &stmnt_)
   {
      TryPointer(preStmnt, stmnt);
   }

   //
   // Info::preStr
   //
   void Info::preStr(IR::StrEnt &str_)
   {
      TryPointer(preStr, str);
   }

   //
   // Info::put
   //
   void Info::put(IR::Program &prog_, std::ostream &out_)
   {
      try
      {
         out  = &out_;
         prog = &prog_;

         putPos = 0;
         put();

         out  = nullptr;
         prog = nullptr;
      }
      catch(...)
      {
         out  = nullptr;
         prog = nullptr;
         throw;
      }
   }

   //
   // Info::putBlock
   //
   void Info::putBlock()
   {
      for(auto &stmnt_ : *block)
         putStmnt(stmnt_);
   }

   //
   // Info::putBlock
   //
   void Info::putBlock(IR::Block &block_)
   {
      TryPointer(putBlock, block);
   }

   //
   // Info::putDJump
   //
   void Info::putDJump(IR::DJump &djump_)
   {
      TryPointer(putDJump, djump);
   }

   //
   // Info::putFunc
   //
   void Info::putFunc()
   {
      putBlock(func->block);
   }

   //
   // Info::putFunc
   //
   void Info::putFunc(IR::Function &func_)
   {
      TryPointer(putFunc, func);
   }

   //
   // Info::putObj
   //
   void Info::putObj(IR::Object &obj_)
   {
      TryPointer(putObj, obj);
   }

   //
   // Info::putSpace
   //
   void Info::putSpace(IR::Space &space_)
   {
      TryPointer(putSpace, space);
   }

   //
   // Info::putStmnt
   //
   void Info::putStmnt(IR::Statement &stmnt_)
   {
      TryPointer(putStmnt, stmnt);
   }

   //
   // Info::putStr
   //
   void Info::putStr(IR::StrEnt &str_)
   {
      TryPointer(putStr, str);
   }

   //
   // Info::putExtra
   //
   void Info::putExtra(IR::Program &prog_)
   {
      try
      {
         prog = &prog_;
         putExtra();
         prog = nullptr;
      }
      catch(...)
      {
         prog = nullptr;
         throw;
      }
   }

   //
   // Info::tr
   //
   void Info::tr()
   {
      for(auto &itr : prog->rangeSpaceGblArs()) trSpace(itr);
      for(auto &itr : prog->rangeSpaceLocArs()) trSpace(itr);
      for(auto &itr : prog->rangeSpaceModArs()) trSpace(itr);
      for(auto &itr : prog->rangeSpaceHubArs()) trSpace(itr);

      trSpace(prog->getSpaceGblReg());
      trSpace(prog->getSpaceHubReg());
      trSpace(prog->getSpaceModReg());
      trSpace(prog->getSpaceSta());

      for(auto &itr : prog->rangeFunction()) trFunc(itr);

      for(auto &itr : prog->rangeDJump())  trDJump(itr);
      for(auto &itr : prog->rangeObject()) trObj(itr);
      for(auto &itr : prog->rangeStrEnt()) trStr(itr);
   }

   //
   // Info::tr
   //
   void Info::tr(IR::Program &prog_)
   {
      TryPointer(tr, prog);
   }

   //
   // Info::trBlock
   //
   void Info::trBlock()
   {
      try
      {
         auto end   = static_cast<IR::Statement *>(block->end());
               stmnt = static_cast<IR::Statement *>(block->begin());
         while(stmnt != end)
         {
            try
            {
               trStmnt();
               stmnt = stmnt->next;
            }
            catch(ResetStmnt const &)
            {
            }
         }
         stmnt = nullptr;
      }
      catch(...)
      {
         stmnt = nullptr;
         throw;
      }
   }

   //
   // Info::trBlock
   //
   void Info::trBlock(IR::Block &block_)
   {
      TryPointer(trBlock, block);
   }

   //
   // Info::trDJump
   //
   void Info::trDJump(IR::DJump &djump_)
   {
      TryPointer(trDJump, djump);
   }

   //
   // Info::trFunc
   //
   void Info::trFunc()
   {
      trBlock(func->block);
   }

   //
   // Info::trFunc
   //
   void Info::trFunc(IR::Function &func_)
   {
      TryPointer(trFunc, func);
   }

   //
   // Info::trObj
   //
   void Info::trObj(IR::Object &obj_)
   {
      TryPointer(trObj, obj);
   }

   //
   // Info::trSpace
   //
   void Info::trSpace(IR::Space &space_)
   {
      TryPointer(trSpace, space);
   }

   //
   // Info::trStmnt
   //
   void Info::trStmnt(IR::Statement &stmnt_)
   {
      TryPointer(trStmnt, stmnt);
   }

   //
   // Info::trStr
   //
   void Info::trStr(IR::StrEnt &str_)
   {
      TryPointer(trStr, str);
   }

   //
   // Info::getFixedInfo
   //
   FixedInfo Info::getFixedInfo(Core::FastU n, bool s)
   {
      FixedInfo fi;

      fi.bitsS = s;
      fi.bitsF = n <= 1 ? 16 : 32;
      fi.bitsI = n * 32 - fi.bitsF - s;

      fi.wordsF = (fi.bitsF + 31) / 32;
      fi.wordsI = (fi.bitsI + 31) / 32;

      return fi;
   }

   //
   // Info::getFloatInfo
   //
   FloatInfo Info::getFloatInfo(Core::FastU n)
   {
      FloatInfo fi;

      // TODO: Somehow not assume a 32 bit word.

      switch(n)
      {
      case  1: fi.bitsExp =  8; break;
      case  2: fi.bitsExp = 11; break;
      case  3:
      case  4: fi.bitsExp = 15; break;
      case  5:
      case  6:
      case  7:
      case  8: fi.bitsExp = 19; break;
      default: fi.bitsExp = 23; break;
      }

      fi.bitsMan     = 31 - fi.bitsExp;
      fi.bitsManFull = n * 32 - fi.bitsExp - 1;
      fi.bitsSig     = 1;

      fi.maxExp  = (Core::FastU(1) << fi.bitsExp) - 1;
      fi.offExp  = fi.maxExp / 2;

      fi.maskExp = fi.maxExp << fi.bitsMan;
      fi.maskMan = (Core::FastU(1) << fi.bitsMan) - 1;
      fi.maskSig = 0x80000000;

      return fi;
   }

   //
   // Info::getStmntSize
   //
   Core::FastU Info::getStmntSize()
   {
      if(stmnt->args.empty())
         return 0;

      Core::FastU size;

      switch(stmnt->code)
      {
      case IR::Code::Nop:
         return 0;

      case IR::Code::None:
      case IR::Code::Casm:
      case IR::Code::Jset:
      case IR::Code::Jump:
      case IR::Code::Pltn:
      case IR::Code::Xcod_SID:
         throw Core::ExceptStr(stmnt->pos, "irregular statement size");

      case IR::Code::AdXU:
      case IR::Code::SuXU:
         size = stmnt->args[2].getSize();
         if(stmnt->args.size() == 3)
         {
            if(stmnt->args[1].getSize() != size ||
               stmnt->args[0].getSize() <= size)
               throw Core::ExceptStr(stmnt->pos, "irregular statement size");
         }
         else
         {
            if(stmnt->args[3].getSize() != size ||
               stmnt->args[0].getSize() - stmnt->args[1].getSize() != size)
               throw Core::ExceptStr(stmnt->pos, "irregular statement size");
         }
         return size;

      case IR::Code::AddF:
      case IR::Code::AddI:
      case IR::Code::AddU:
      case IR::Code::BAnd:
      case IR::Code::BOrI:
      case IR::Code::BOrX:
      case IR::Code::DivA:
      case IR::Code::DivF:
      case IR::Code::DivI:
      case IR::Code::DivK:
      case IR::Code::DivR:
      case IR::Code::DivU:
      case IR::Code::DivX:
      case IR::Code::ModI:
      case IR::Code::ModU:
      case IR::Code::MulA:
      case IR::Code::MulF:
      case IR::Code::MulI:
      case IR::Code::MulK:
      case IR::Code::MulR:
      case IR::Code::MulU:
      case IR::Code::MulX:
      case IR::Code::SubF:
      case IR::Code::SubI:
      case IR::Code::SubU:
         size = stmnt->args[0].getSize();
         if(stmnt->args[1].getSize() != size || stmnt->args[2].getSize() != size)
            throw Core::ExceptStr(stmnt->pos, "irregular statement size");
         return size;

      case IR::Code::BNot:
      case IR::Code::Bges:
      case IR::Code::Bget:
      case IR::Code::Bset:
      case IR::Code::Copy:
      case IR::Code::Move:
      case IR::Code::NegF:
      case IR::Code::NegI:
      case IR::Code::ShLF:
      case IR::Code::ShLU:
      case IR::Code::ShRF:
      case IR::Code::ShRI:
      case IR::Code::ShRU:
      case IR::Code::Swap:
         size = stmnt->args[0].getSize();
         if(stmnt->args[1].getSize() != size)
            throw Core::ExceptStr(stmnt->pos, "irregular statement size");
         return size;

      case IR::Code::Bclo:
      case IR::Code::Bclz:
      case IR::Code::Jfar:
      case IR::Code::LNot:
         return stmnt->args[1].getSize();

      case IR::Code::Call:
      case IR::Code::Cnat:
      case IR::Code::Cscr_IA:
      case IR::Code::Cscr_SA:
      case IR::Code::Cspe:
         size = 0;
         if(stmnt->args.size() > 2)
            for(auto i = stmnt->args.begin() + 2, e = stmnt->args.end(); i != e; ++i)
               size += i->getSize();
         return size;

      case IR::Code::CmpF_EQ:
      case IR::Code::CmpF_GE:
      case IR::Code::CmpF_GT:
      case IR::Code::CmpF_LE:
      case IR::Code::CmpF_LT:
      case IR::Code::CmpF_NE:
      case IR::Code::CmpI_EQ:
      case IR::Code::CmpI_GE:
      case IR::Code::CmpI_GT:
      case IR::Code::CmpI_LE:
      case IR::Code::CmpI_LT:
      case IR::Code::CmpI_NE:
      case IR::Code::CmpU_EQ:
      case IR::Code::CmpU_GE:
      case IR::Code::CmpU_GT:
      case IR::Code::CmpU_LE:
      case IR::Code::CmpU_LT:
      case IR::Code::CmpU_NE:
      case IR::Code::LAnd:
      case IR::Code::LOrI:
         size = stmnt->args[1].getSize();
         if(stmnt->args[2].getSize() != size)
            throw Core::ExceptStr(stmnt->pos, "irregular statement size");
         return size;

      case IR::Code::Cscr_IS:
      case IR::Code::Cscr_SS:
         size = 0;
         if(stmnt->args.size() > 3)
            for(auto i = stmnt->args.begin() + 3, e = stmnt->args.end(); i != e; ++i)
               size += i->getSize();
         return size;

      case IR::Code::DiXI:
      case IR::Code::DiXU:
      case IR::Code::MuXU:
         size = stmnt->args[1].getSize();
         if(stmnt->args[0].getSize() != size * 2 || stmnt->args[2].getSize() != size)
            throw Core::ExceptStr(stmnt->pos, "irregular statement size");
         return size;

      case IR::Code::Jcnd_Nil:
      case IR::Code::Jcnd_Tab:
      case IR::Code::Jcnd_Tru:
      case IR::Code::Jdyn:
      case IR::Code::Retn:
         return stmnt->args[0].getSize();
      }

      throw Core::ExceptStr(stmnt->pos, "irregular statement size");
   }

   //
   // Info::CheckArgB
   //
   void Info::CheckArgB(IR::Arg const &arg, IR::AddrBase b, Core::Origin pos)
   {
      if(arg.a != b)
      {
         std::cerr << "ERROR: " << pos << ": " << arg.a << " must have "
            << b << " parameter\n";
         throw EXIT_FAILURE;
      }
   }

   //
   // Info::CheckArgB
   //
   void Info::CheckArgB(IR::Statement *stmnt, std::size_t a, IR::ArgBase b)
   {
      if(stmnt->args[a].a != b)
      {
         std::cerr << "ERROR: " << stmnt->pos << ": " << stmnt->code
            << " must have " << b << " args[" << a << "]\n";
         throw EXIT_FAILURE;
      }
   }

   //
   // Info::CheckArgC
   //
   void Info::CheckArgC(IR::Statement *stmnt, std::size_t c)
   {
      if(stmnt->args.size() < c)
      {
         std::cerr << "ERROR: " << stmnt->pos << ": bad argc for " << stmnt->code
            << ": " << stmnt->args.size() << " < " << c << '\n';
         throw EXIT_FAILURE;
      }
   }
}

// EOF

