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

//
// DefaultFunc_Base
//
#define DefaultFunc_Base(set) \
   void Info::set() \
   { \
      for(auto &itr : prog->rangeSpaceGblArs()) set##Space(itr); \
      for(auto &itr : prog->rangeSpaceHubArs()) set##Space(itr); \
      for(auto &itr : prog->rangeSpaceLocArs()) set##Space(itr); \
      for(auto &itr : prog->rangeSpaceModArs()) set##Space(itr); \
      \
      set##Space(prog->getSpaceGblReg()); \
      set##Space(prog->getSpaceHubReg()); \
      set##Space(prog->getSpaceModReg()); \
      set##Space(prog->getSpaceSta()); \
      \
      for(;;) try \
      { \
         for(auto &itr : prog->rangeFunction()) set##Func(itr); \
         break; \
      } \
      catch(ResetFunc const &) {} \
      \
      for(auto &itr : prog->rangeDJump())  set##DJump(itr); \
      for(auto &itr : prog->rangeObject()) set##Obj(itr); \
      for(auto &itr : prog->rangeStrEnt()) set##Str(itr); \
   }

//
// DefaultFunc_Block
//
#define DefaultFunc_Block(set) \
   void Info::set##Block() \
   { \
      try \
      { \
         auto end   = static_cast<IR::Statement *>(block->end()); \
              stmnt = static_cast<IR::Statement *>(block->begin()); \
         while(stmnt != end) \
         { \
            try \
            { \
               set##Stmnt(); \
               stmnt = stmnt->next; \
            } \
            catch(ResetStmnt const &) {} \
         } \
         stmnt = nullptr; \
      } \
      catch(...) \
      { \
         stmnt = nullptr; \
         throw; \
      } \
   }

//
// DefaultFunc_Func
//
#define DefaultFunc_Func(set) \
   void Info::set##Func() \
   { \
      set##Block(func->block); \
   }

//
// DefaultFuncSet
//
#define DefaultFuncSet(set) \
   DefaultFunc_Block(set) \
   DefaultFunc_Func(set)

//
// DeferFunc
//
#define DeferFunc(obj, fun, ptr) \
   void Info::fun(IR::obj &ptr##_) \
   { \
      TryPointer(fun, ptr); \
   }

//
// DeferFuncSet
//
#define DeferFuncSet(set) \
   DeferFunc(Block,     set##Block,    block) \
   DeferFunc(DJump,     set##DJump,    djump) \
   DeferFunc(Function,  set##Func,     func) \
   DeferFunc(Object,    set##Obj,      obj) \
   DeferFunc(Space,     set##Space,    space) \
   DeferFunc(Statement, set##Stmnt,    stmnt) \
   DeferFunc(StrEnt,    set##Str,      str) \


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC
{
   DefaultFunc_Base(chk)
   DefaultFunc_Base(gen)
   DefaultFunc_Base(opt)
   DefaultFunc_Base(pre)
   DefaultFunc_Base(tr)

   DefaultFuncSet(chk)
   DefaultFuncSet(gen)
   DefaultFuncSet(opt)
   DefaultFuncSet(pre)
   DefaultFuncSet(put)
   DefaultFuncSet(tr)

   DeferFunc(Program, chk, prog)
   DeferFunc(Program, gen, prog)
   DeferFunc(Program, opt, prog)
   DeferFunc(Program, pre, prog)
   DeferFunc(Program, tr,  prog)

   DeferFuncSet(chk)
   DeferFuncSet(gen)
   DeferFuncSet(opt)
   DeferFuncSet(pre)
   DeferFuncSet(put)
   DeferFuncSet(tr)

   DeferFunc(Program, putExtra, prog)

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
         Core::Error(stmnt->pos, "irregular statement size");

      case IR::Code::AdXU:
      case IR::Code::SuXU:
         size = stmnt->args[2].getSize();
         if(stmnt->args.size() == 3)
         {
            if(stmnt->args[1].getSize() != size ||
               stmnt->args[0].getSize() <= size)
               Core::Error(stmnt->pos, "irregular statement size");
         }
         else
         {
            if(stmnt->args[3].getSize() != size ||
               stmnt->args[0].getSize() - stmnt->args[1].getSize() != size)
               Core::Error(stmnt->pos, "irregular statement size");
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
            Core::Error(stmnt->pos, "irregular statement size");
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
            Core::Error(stmnt->pos, "irregular statement size");
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
            Core::Error(stmnt->pos, "irregular statement size");
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
            Core::Error(stmnt->pos, "irregular statement size");
         return size;

      case IR::Code::Jcnd_Nil:
      case IR::Code::Jcnd_Tab:
      case IR::Code::Jcnd_Tru:
      case IR::Code::Jdyn:
      case IR::Code::Retn:
         return stmnt->args[0].getSize();
      }

      Core::Error(stmnt->pos, "irregular statement size");
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

