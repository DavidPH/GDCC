//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Target information and handling base class.
//
//-----------------------------------------------------------------------------

#include "BC/Info.hpp"

#include "IR/Exception.hpp"
#include "IR/Program.hpp"


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
      for(auto &itr : prog->rangeStrEnt()) set##StrEnt(itr); \
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
   DeferFunc(StrEnt,    set##StrEnt,   strent) \


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
   // Info::errorCode
   //
   [[noreturn]]
   void Info::errorCode(char const *msg)
   {
      IR::ErrorCode(stmnt, msg);
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

      switch(stmnt->code.base)
      {
      case IR::CodeBase::Nop:
         return 0;

      case IR::CodeBase::None:
      case IR::CodeBase::Casm:
      case IR::CodeBase::Jfar_Set:
      case IR::CodeBase::Jfar_Sta:
      case IR::CodeBase::Jump:
      case IR::CodeBase::Pltn:
      case IR::CodeBase::Tr:
      case IR::CodeBase::Xcod_SID:
         Core::Error(stmnt->pos, "irregular statement size");

      case IR::CodeBase::AddX:
      case IR::CodeBase::SubX:
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

      case IR::CodeBase::Add:
      case IR::CodeBase::BAnd:
      case IR::CodeBase::BOrI:
      case IR::CodeBase::BOrX:
      case IR::CodeBase::Div:
      case IR::CodeBase::Mod:
      case IR::CodeBase::Mul:
      case IR::CodeBase::Sub:
         size = stmnt->args[0].getSize();
         if(stmnt->args[1].getSize() != size || stmnt->args[2].getSize() != size)
            Core::Error(stmnt->pos, "irregular statement size");
         return size;

      case IR::CodeBase::BNot:
      case IR::CodeBase::Bges:
      case IR::CodeBase::Bget:
      case IR::CodeBase::Bset:
      case IR::CodeBase::Copy:
      case IR::CodeBase::Move:
      case IR::CodeBase::Neg:
      case IR::CodeBase::ShL:
      case IR::CodeBase::ShR:
      case IR::CodeBase::Swap:
         size = stmnt->args[0].getSize();
         if(stmnt->args[1].getSize() != size)
            Core::Error(stmnt->pos, "irregular statement size");
         return size;

      case IR::CodeBase::Bclo:
      case IR::CodeBase::Bclz:
      case IR::CodeBase::Jfar_Pro:
      case IR::CodeBase::LNot:
         return stmnt->args[1].getSize();

      case IR::CodeBase::Call:
      case IR::CodeBase::Cnat:
      case IR::CodeBase::Cscr_IA:
      case IR::CodeBase::Cscr_SA:
      case IR::CodeBase::Cspe:
         size = 0;
         if(stmnt->args.size() > 2)
            for(auto i = stmnt->args.begin() + 2, e = stmnt->args.end(); i != e; ++i)
               size += i->getSize();
         return size;

      case IR::CodeBase::CmpEQ:
      case IR::CodeBase::CmpGE:
      case IR::CodeBase::CmpGT:
      case IR::CodeBase::CmpLE:
      case IR::CodeBase::CmpLT:
      case IR::CodeBase::CmpNE:
      case IR::CodeBase::LAnd:
      case IR::CodeBase::LOrI:
         size = stmnt->args[1].getSize();
         if(stmnt->args[2].getSize() != size)
            Core::Error(stmnt->pos, "irregular statement size");
         return size;

      case IR::CodeBase::Cscr_IS:
      case IR::CodeBase::Cscr_SS:
         size = 0;
         if(stmnt->args.size() > 3)
            for(auto i = stmnt->args.begin() + 3, e = stmnt->args.end(); i != e; ++i)
               size += i->getSize();
         return size;

      case IR::CodeBase::DivX:
      case IR::CodeBase::MulX:
         size = stmnt->args[1].getSize();
         if(stmnt->args[0].getSize() != size * 2 || stmnt->args[2].getSize() != size)
            Core::Error(stmnt->pos, "irregular statement size");
         return size;

      case IR::CodeBase::Jcnd_Nil:
      case IR::CodeBase::Jcnd_Tab:
      case IR::CodeBase::Jcnd_Tru:
      case IR::CodeBase::Jdyn:
      case IR::CodeBase::Retn:
      case IR::CodeBase::Rjnk:
         return stmnt->args[0].getSize();
      }

      Core::Error(stmnt->pos, "irregular statement size");
   }
}

// EOF

