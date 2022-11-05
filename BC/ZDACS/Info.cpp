//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2022 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS target information and handling class.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "Core/Exception.hpp"
#include "Core/Option.hpp"
#include "Core/StringOption.hpp"

#include "IR/Program.hpp"

#include "Option/Bool.hpp"
#include "Option/Exception.hpp"
#include "Option/Int.hpp"

#include "Target/Info.hpp"

#include <climits>


//----------------------------------------------------------------------------|
// Options                                                                    |
//

namespace GDCC::BC::ZDACS
{
   //
   // --bc-zdacs-Sta-array
   //
   Option::Int<Core::FastU> Info::StaArray
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("bc-zdacs-Sta-array")
         .setGroup("codegen")
         .setDescS("Sets the global array used for Sta."),

      0
   };

   //
   // --bc-zdacs-chunk-STRE
   //
   Option::Bool Info::UseChunkSTRE
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("bc-zdacs-chunk-STRE")
         .setGroup("output")
         .setDescS("Generates an encrypted string table."),

      false
   };

   //
   // --bc-zdacs-fake-ACS0
   //
   Option::Bool Info::UseFakeACS0
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("bc-zdacs-fake-ACS0")
         .setGroup("output")
         .setDescS("Generates a fake ACS0 header."),

      false
   };

   //
   // --bc-zdacs-init-delay
   //
   Option::Bool Info::InitDelay
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("bc-zdacs-init-delay")
         .setGroup("output")
         .setDescS("Adds a delay before setting init flags.")
         .setDescL(
            "Adds a delay before setting init flags. This is needed if "
            "there might be multiple init scripts that need to run, "
            "such as from having multiple modules."),

      false
   };

   //
   // --bc-zdacs-init-gbl-array
   //
   static Option::Int<Core::FastU> InitGblArray
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("bc-zdacs-init-gbl-array")
         .setGroup("codegen")
         .setDescS("Sets the global array used to store init status.")
         .setDescL(
            "Sets the global array used to store initialization status. "
            "Default is to use the Sta array."),

      0
   };

   //
   // --bc-zdacs-init-gbl-index
   //
   static Option::Int<Core::FastU> InitGblIndex
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("bc-zdacs-init-gbl-index")
         .setGroup("codegen")
         .setDescS("Sets the global array index used to store init status.")
         .setDescL(
            "Sets the global array index used to store initialization "
            "status. Default is 0 if using the Sta array, -1 "
            "otherwise."),

      0
   };

   //
   // --bc-zdacs-init-hub-array
   //
   static Option::Int<Core::FastU> InitHubArray
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("bc-zdacs-init-hub-array")
         .setGroup("codegen")
         .setDescS("Sets the hub array used to store init status.")
         .setDescL(
            "Sets the hub array used to store initialization status. "
            "Default is to use any hub array with an initializer."),

      0
   };

   //
   // --bc-zdacs-init-hub-index
   //
   static Option::Int<Core::FastU> InitHubIndex
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("bc-zdacs-init-hub-index")
         .setGroup("codegen")
         .setDescS("Sets the hub array index used to store init status.")
         .setDescL(
            "Sets the hub array index used to store initialization "
            "status. Default is -1."),

      0xFFFFFFFF
   };

   //
   // --bc-zdacs-init-script-name
   //
   Core::StringOption Info::InitScriptName
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("bc-zdacs-init-script-name")
         .setGroup("codegen")
         .setDescS("Sets the name for the init script.")
         .setDescL(
            "Sets the script name for the initialization script. "
            "Default is '<output filename>$init'."),

      nullptr
   };

   //
   // --bc-zdacs-init-script-named
   //
   Option::Bool Info::InitScriptNamed
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("bc-zdacs-init-script-named")
         .setGroup("codegen")
         .setDescS("Enables a named init script.")
         .setDescL("Enables a named init script. Default is on."),

      true
   };

   //
   // --bc-zdacs-init-script-number
   //
   Option::Int<Core::FastU> Info::InitScriptNumber
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("bc-zdacs-init-script-number")
         .setGroup("codegen")
         .setDescS("Sets the number for the init script.")
         .setDescL(
            "Sets the script number for the initialization script. "
            "Default is 999."),

      999
   };

   //
   // --bc-zdacs-script-flag
   //
   Info::ScriptTypeMap Info::ScriptFlags
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("bc-zdacs-script-flag")
         .setGroup("codegen")
         .setDescS("Defines a script flag translation.")
         .setDescL(
            "Defines a script flag translation. Consumes two arguments, the "
            "script flag string and the numeric value in bytecode.\n"
            "\n"
            "If the long option is negated with no-, the custom translation "
            "for that script flag is removed and the option only takes the "
            "first argument.")
   };

   //
   // --bc-zdacs-script-type
   //
   Info::ScriptTypeMap Info::ScriptTypes
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("bc-zdacs-script-type")
         .setGroup("codegen")
         .setDescS("Defines a script type translation.")
         .setDescL(
            "Defines a script type translation. Consumes two arguments, the "
            "script type string and the numeric value in bytecode.\n"
            "\n"
            "If the long option is negated with no-, the custom translation "
            "for that script type is removed and the option only takes the "
            "first argument.")
   };

   //
   // --bc-zdacs-scripti-param
   //
   static Option::Int<Core::FastU> ScriptIParam
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("bc-zdacs-scripti-param")
         .setGroup("codegen")
         .setDescS("Sets the number of native parameters for numbered scripts.")
         .setDescL("Sets the number of native parameters for numbered scripts. "
            "Default is 4."),

      4
   };

   //
   // --bc-zdacs-scripts-param
   //
   static Option::Int<Core::FastU> ScriptSParam
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("bc-zdacs-scripts-param")
         .setGroup("codegen")
         .setDescS("Sets the number of native parameters for named scripts.")
         .setDescL("Sets the number of native parameters for named scripts. "
            "Default is 4."),

      4
   };

   //
   // --bc-zdacs-stkcall-retn
   //
   static Option::Int<Core::FastU> StkCallRetn
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("bc-zdacs-stkcall-retn")
         .setGroup("codegen")
         .setDescS("Sets the number of native return words for functions.")
         .setDescL("Sets the number of native return words for functions. "
            "Default is 1."),

      1
   };
}


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

namespace GDCC::BC::ZDACS
{
   IR::Type_Fixed const Info::TypeWord{32, 0, false, false};

   Core::FastU Info::FarJumpIndex = 1;
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   //
   // Info default constructor
   //
   Info::Info() :
      codeInit   {0},
      codeInitEnd{0},

      numChunkAIMP{0},
      numChunkAINI{0},
      numChunkARAY{0},
      numChunkASTR{0},
      numChunkATAG{0},
      numChunkCODE{0},
      numChunkFNAM{0},
      numChunkFUNC{0},
      numChunkJUMP{0},
      numChunkLOAD{0},
      numChunkMEXP{0},
      numChunkMIMP{0},
      numChunkMINI{0},
      numChunkMSTR{0},
      numChunkSFLG{0},
      numChunkSNAM{0},
      numChunkSPTR{0},
      numChunkSTRL{0},
      numChunkSVCT{0}
   {
   }

   //
   // Info::backGlyphDJump
   //
   void Info::backGlyphDJump(Core::String glyph, Core::FastU val)
   {
      auto &data = prog->getGlyphData(glyph);

      data.type  = IR::Type_DJump();
      data.value = IR::ExpCreate_Value(IR::Value_DJump(val, {}), {nullptr});
   }

   //
   // Info::backGlyphFunc
   //
   void Info::backGlyphFunc(Core::String glyph, Core::FastU val,
      IR::CallType ctype)
   {
      auto &data = prog->getGlyphData(glyph);

      data.type  = IR::Type_Funct(ctype);
      data.value = IR::ExpCreate_Value(
         IR::Value_Funct(val, IR::Type_Funct(ctype)),
         Core::Origin(Core::STRNULL, 0));
   }

   //
   // Info::backGlyphGlyph
   //
   void Info::backGlyphGlyph(Core::String glyph, Core::String val)
   {
      auto &data = prog->getGlyphData(glyph);

      data.type  = prog->getGlyphData(val).type;
      data.value = IR::ExpCreate_Glyph(
         IR::Glyph(prog, val),
         Core::Origin(Core::STRNULL, 0));
   }

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
   // Info::backGlyphStrEnt
   //
   void Info::backGlyphStrEnt(Core::String glyph, Core::FastU val)
   {
      auto &data = prog->getGlyphData(glyph);

      data.type  = IR::Type_StrEn();
      data.value = IR::ExpCreate_Value(
         IR::Value_StrEn(val, IR::Type_StrEn()),
         Core::Origin(Core::STRNULL, 0));
   }

   //
   // Info::backGlyphWord
   //
   void Info::backGlyphWord(Core::String glyph, Core::FastU val)
   {
      auto &data = prog->getGlyphData(glyph);

      data.type  = TypeWord;
      data.value = IR::ExpCreate_Value(
         IR::Value_Fixed(Core::NumberCast<Core::Integ>(val), TypeWord), {});
   }

   //
   // Info::getAllocDJump
   //
   Core::NumberAllocMerge<Core::FastU> &Info::getAllocDJump()
   {
      if(allocDJump)
         return *allocDJump;

      allocDJump.reset(new Core::NumberAllocMerge<Core::FastU>);

      for(auto const &j : prog->rangeDJump())
         if(!j.alloc) allocDJump->allocAt(1, j.value);

      return *allocDJump;
   }

   //
   // Info::getAllocFunc
   //
   Core::NumberAllocMerge<Core::FastU> &Info::getAllocFunc(IR::CallType call)
   {
      auto trans = [](IR::CallType ctype)
      {
         switch(ctype)
         {
         case IR::CallType::StdCall:  return IR::CallType::StkCall;
         case IR::CallType::SScriptI: return IR::CallType::ScriptI;
         case IR::CallType::SScriptS: return IR::CallType::ScriptS;
         default:                     return ctype;
         }
      };

      IR::CallType ctype = trans(call);

      if(allocFunc.count(ctype))
         return allocFunc[ctype];

      auto &alloc = allocFunc[ctype];

      for(auto const &fn : prog->rangeFunction())
         if(!fn.alloc && trans(fn.ctype) == ctype) alloc.allocAt(1, fn.valueInt);

      return alloc;
   }

   //
   // Info::getAllocObj
   //
   Core::NumberAllocMerge<Core::FastU> &Info::getAllocObj(IR::AddrSpace addr)
   {
      if(allocObj.count(addr))
         return allocObj[addr];

      auto &alloc = allocObj[addr];

      for(auto const &ob : prog->rangeObjectBySpace(addr))
         if(!ob->alloc) alloc.allocAt(ob->words, ob->value);

      if(addr.base == IR::AddrBase::ModReg)
      {
         auto &as = getAllocSpace(addr.base);
         for(auto const &ob : prog->rangeObjectBySpace(addr))
            if(!ob->alloc) as.allocAt(ob->words, ob->value);
      }

      return alloc;
   }

   //
   // Info::getAllocSpace
   //
   Core::NumberAllocMerge<Core::FastU> &Info::getAllocSpace(IR::AddrBase addr)
   {
      if(allocSpace.count(addr))
         return allocSpace[addr];

      auto &alloc = allocSpace[addr];

      switch(addr)
      {
      case IR::AddrBase::GblArr:
         for(auto const &sp : prog->rangeSpaceGblArs())
            if(!sp.alloc) alloc.allocAt(1, sp.value);

         alloc.allocAt(1, StaArray);
         break;

      case IR::AddrBase::ModArr:
         for(auto const &sp : prog->rangeSpaceModArs())
            if(!sp.alloc) alloc.allocAt(1, sp.value);

         {
            auto &ao = getAllocObj({IR::AddrBase::ModReg, Core::STR_});
            for(auto const &sp : prog->rangeSpaceModArs())
               if(!sp.alloc) ao.allocAt(1, sp.value);
         }

         break;

      case IR::AddrBase::HubArr:
         for(auto const &sp : prog->rangeSpaceHubArs())
            if(!sp.alloc) alloc.allocAt(1, sp.value);
         break;

      default:
         break;
      }

      return alloc;
   }

   //
   // Info::getAllocStrEnt
   //
   Core::NumberAllocMerge<Core::FastU> &Info::getAllocStrEnt()
   {
      if(allocStrEnt)
         return *allocStrEnt;

      allocStrEnt.reset(new Core::NumberAllocMerge<Core::FastU>);

      for(auto const &s : prog->rangeStrEnt())
         if(!s.alloc) allocStrEnt->allocAt(1, s.valueInt);

      return *allocStrEnt;
   }

   //
   // Info::getInitGblArray
   //
   Core::FastU Info::getInitGblArray()
   {
      if(InitGblArray.processed)
         return InitGblArray;

      return StaArray;
   }

   //
   // Info::getInitGblIndex
   //
   Core::FastU Info::getInitGblIndex()
   {
      if(InitGblIndex.processed)
         return InitGblIndex;

      if(!InitGblArray.processed || InitGblArray == StaArray)
         return 0;

      return -1;
   }

   //
   // Info::getInitHubArray
   //
   Core::FastU Info::getInitHubArray()
   {
      if(InitHubArray.processed)
         return InitHubArray;

      for(auto const &itr : prog->rangeSpaceHubArs())
      {
         if(!init[&itr].vals.empty())
            return itr.value;
      }

      return 0;
   }

   //
   // Info::getInitHubIndex
   //
   Core::FastU Info::getInitHubIndex()
   {
      return InitHubIndex;
   }

   //
   // Info::getStkPtrIdx
   //
   Core::FastU Info::getStkPtrIdx()
   {
      switch(func->ctype)
      {
      case IR::CallType::SScriptI:
      case IR::CallType::SScriptS:
      case IR::CallType::StdCall:
         return 0;

      case IR::CallType::ScriptI:
      case IR::CallType::ScriptS:
      case IR::CallType::StkCall:
         if(func->allocAut)
            return func->localReg - 1;

         goto case_def;

      default:
      case_def:
         Core::Error(stmnt->pos, "bad getStkPtrIdx");
      }
   }

   //
   // Info::getWordType_Funct
   //
   IR::TypeBase Info::getWordType_Funct(IR::Type_Funct const &type, Core::FastU w)
   {
      if(IsScriptS(type.callT)) return w ? IR::TypeBase::Fixed : IR::TypeBase::StrEn;
      if(IsScript(type.callT))  return IR::TypeBase::Fixed;
      return w ? IR::TypeBase::Fixed : IR::TypeBase::Funct;
   }

   //
   // Info::getWordType_StrEn
   //
   IR::TypeBase Info::getWordType_StrEn(IR::Type_StrEn const &, Core::FastU w)
   {
      return w ? IR::TypeBase::Fixed : IR::TypeBase::StrEn;
   }

   //
   // Info::isDropArg
   //
   bool Info::isDropArg(IR::Arg const &arg)
   {
      switch(arg.a)
      {
      case IR::ArgBase::Aut:    return isPushArg(*arg.aAut.idx);
      case IR::ArgBase::GblArr: return isPushArg(*arg.aGblArr.idx);
      case IR::ArgBase::GblReg: return true;
      case IR::ArgBase::HubArr: return isPushArg(*arg.aHubArr.idx);
      case IR::ArgBase::HubReg: return true;
      case IR::ArgBase::LocArr: return isPushArg(*arg.aLocArr.idx);
      case IR::ArgBase::LocReg: return true;
      case IR::ArgBase::ModArr: return isPushArg(*arg.aModArr.idx);
      case IR::ArgBase::ModReg: return true;
      case IR::ArgBase::Nul:    return true;
      case IR::ArgBase::Sta:    return isPushArg(*arg.aSta.idx);
      default:                  return false;
      }
   }

   //
   // Info::isCopyArg
   //
   bool Info::isCopyArg(IR::Arg const &arg)
   {
      switch(arg.a)
      {
      case IR::ArgBase::Aut:    return isPushArg(*arg.aAut.idx);
      case IR::ArgBase::GblArr: return isPushArg(*arg.aGblArr.idx);
      case IR::ArgBase::GblReg: return true;
      case IR::ArgBase::HubArr: return isPushArg(*arg.aHubArr.idx);
      case IR::ArgBase::HubReg: return true;
      case IR::ArgBase::LocArr: return isPushArg(*arg.aLocArr.idx);
      case IR::ArgBase::LocReg: return true;
      case IR::ArgBase::ModArr: return isPushArg(*arg.aModArr.idx);
      case IR::ArgBase::ModReg: return true;
      case IR::ArgBase::Sta:    return isPushArg(*arg.aSta.idx);
      default:                  return false;
      }
   }

   //
   // Info::isFastArg
   //
   bool Info::isFastArg(IR::Arg const &arg)
   {
      switch(arg.a)
      {
      case IR::ArgBase::GblReg: return true;
      case IR::ArgBase::HubReg: return true;
      case IR::ArgBase::Lit:    return true;
      case IR::ArgBase::LocReg: return true;
      case IR::ArgBase::ModReg: return true;
      default:                  return false;
      }
   }

   //
   // Info::isFuncJfar_Set
   //
   bool Info::isFuncJfar_Set(IR::Function const *f)
   {
      return true;

      if(auto itr = funcJfar_Set.find(f); itr != funcJfar_Set.end())
         return itr->second;

      bool found = false;
      for(auto const &itr : f->block)
         if(itr.code.base == IR::CodeBase::Jfar_Set)
            {found = true; break;}

      return funcJfar_Set.emplace(f, found).second;
   }

   //
   // Info::isInitiGblArr
   //
   bool Info::isInitiGblArr()
   {
      for(auto const &itr : prog->rangeSpaceGblArs())
         if(!init[&itr].vals.empty()) return true;

      return !init[&prog->getSpaceSta()].vals.empty();
   }

   //
   // Info::isInitiHubArr
   //
   bool Info::isInitiHubArr()
   {
      for(auto const &itr : prog->rangeSpaceHubArs())
         if(!init[&itr].vals.empty()) return true;

      return false;
   }

   //
   // Info::isPushArg
   //
   bool Info::isPushArg(IR::Arg const &arg)
   {
      switch(arg.a)
      {
      case IR::ArgBase::Aut:    return isPushArg(*arg.aAut.idx);
      case IR::ArgBase::GblArr: return isPushArg(*arg.aGblArr.idx);
      case IR::ArgBase::GblReg: return true;
      case IR::ArgBase::HubArr: return isPushArg(*arg.aHubArr.idx);
      case IR::ArgBase::HubReg: return true;
      case IR::ArgBase::Lit:    return true;
      case IR::ArgBase::LocArr: return isPushArg(*arg.aLocArr.idx);
      case IR::ArgBase::LocReg: return true;
      case IR::ArgBase::ModArr: return isPushArg(*arg.aModArr.idx);
      case IR::ArgBase::ModReg: return true;
      case IR::ArgBase::Sta:    return isPushArg(*arg.aSta.idx);
      default:                  return false;
      }
   }

   //
   // Info::lenDropArg
   //
   Core::FastU Info::lenDropArg(IR::Arg const &arg, Core::FastU w)
   {
      //
      // lenArr
      //
      auto lenArr = [&](IR::ArgPtr2 const &a) -> Core::FastU
      {
         if(a.idx->a == IR::ArgBase::Lit)
            return 20;
         else
         {
            Core::FastU len = lenPushArg(*a.idx, 0) + 12;

            if(a.off + w)
               len += 12;

            return len;
         }
      };

      //
      // lenAut
      //
      auto lenAut = [&](IR::Arg_Aut const &a) -> Core::FastU
      {
         if(a.idx->a == IR::ArgBase::Lit)
            return 32;
         else
         {
            Core::FastU len = lenPushArg(*a.idx, 0) + 24;

            if(a.off + w)
               len += 12;

            return len;
         }
      };

      //
      // lenSta
      //
      auto lenSta = [&](IR::Arg_Sta const &a) -> Core::FastU
      {
         if(a.idx->a == IR::ArgBase::Lit)
            return 20;
         else
         {
            Core::FastU len = lenPushArg(*a.idx, 0) + 12;

            if(a.off + w)
               len += 12;

            return len;
         }
      };

      switch(arg.a)
      {
      case IR::ArgBase::Aut:    return lenAut(arg.aAut);
      case IR::ArgBase::GblArr: return lenArr(arg.aGblArr);
      case IR::ArgBase::GblReg: return 8;
      case IR::ArgBase::HubArr: return lenArr(arg.aHubArr);
      case IR::ArgBase::HubReg: return 8;
      case IR::ArgBase::LocArr: return lenArr(arg.aLocArr);
      case IR::ArgBase::LocReg: return 8;
      case IR::ArgBase::ModArr: return lenArr(arg.aModArr);
      case IR::ArgBase::ModReg: return 8;
      case IR::ArgBase::Nul:    return 4;
      case IR::ArgBase::Sta:    return lenSta(arg.aSta);
      default:
         Core::Error(stmnt->pos, "bad lenDropArg");
      }
   }

   //
   // Info::lenDropArg
   //
   Core::FastU Info::lenDropArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi)
   {
      Core::FastU len = 0;

      for(; lo != hi; ++lo)
         len += lenDropArg(arg, lo);

      return len;
   }

   //
   // Info::lenDropTmp
   //
   Core::FastU Info::lenDropTmp(Core::FastU)
   {
      return 8;
   }

   //
   // Info::lenIncUArg
   //
   Core::FastU Info::lenIncUArg(IR::Arg const &arg, Core::FastU)
   {
      switch(arg.a)
      {
      case IR::ArgBase::GblReg: return 8;
      case IR::ArgBase::HubReg: return 8;
      case IR::ArgBase::LocReg: return 8;
      case IR::ArgBase::ModReg: return 8;
      default:
         Core::Error(stmnt->pos, "bad lenIncUArg");
      }
   }

   //
   // Info::lenIncUArg
   //
   Core::FastU Info::lenIncUArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi)
   {
      Core::FastU len = 0;

      for(; lo != hi; ++lo)
         len += lenIncUArg(arg, lo);

      return len;
   }

   //
   // Info::lenPushArg
   //
   Core::FastU Info::lenPushArg(IR::Arg const &arg, Core::FastU w)
   {
      //
      // lenArr
      //
      auto lenArr = [&](IR::ArgPtr2 const &a) -> Core::FastU
      {
         if(a.idx->a == IR::ArgBase::Lit)
            return 16;
         else
         {
            Core::FastU len = lenPushArg(*a.idx, 0) + 8;

            if(a.off + w)
               len += 12;

            return len;
         }
      };

      //
      // lenAut
      //
      auto lenAut = [&](IR::Arg_Aut const &a) -> Core::FastU
      {
         if(a.idx->a == IR::ArgBase::Lit)
            return 28;
         else
         {
            Core::FastU len = lenPushArg(*a.idx, 0) + 20;

            if(a.off + w)
               len += 12;

            return len;
         }
      };

      //
      // lenLit
      //
      auto lenLit = [&]() -> Core::FastU
      {
         auto type = arg.aLit.value->getType();
         auto wOff = arg.aLit.off + w;

         return getWordType(type, wOff) == IR::TypeBase::StrEn ? 12 : 8;
      };

      //
      // lenSta
      //
      auto lenSta = [&](IR::Arg_Sta const &a) -> Core::FastU
      {
         if(a.idx->a == IR::ArgBase::Lit)
            return 16;
         else
         {
            Core::FastU len = lenPushArg(*a.idx, 0) + 8;

            if(a.off + w)
               len += 12;

            return len;
         }
      };

      switch(arg.a)
      {
      case IR::ArgBase::Aut:    return lenAut(arg.aAut);
      case IR::ArgBase::GblArr: return lenArr(arg.aGblArr);
      case IR::ArgBase::GblReg: return 8;
      case IR::ArgBase::HubArr: return lenArr(arg.aHubArr);
      case IR::ArgBase::HubReg: return 8;
      case IR::ArgBase::Lit:    return lenLit();
      case IR::ArgBase::LocArr: return lenArr(arg.aLocArr);
      case IR::ArgBase::LocReg: return 8;
      case IR::ArgBase::ModArr: return lenArr(arg.aModArr);
      case IR::ArgBase::ModReg: return 8;
      case IR::ArgBase::Sta:    return lenSta(arg.aSta);

      default:
         Core::Error(stmnt->pos, "bad lenPushArg");
      }
   }

   //
   // Info::lenPushArg
   //
   Core::FastU Info::lenPushArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi)
   {
      Core::FastU len = 0;

      for(; lo != hi; ++lo)
         len += lenPushArg(arg, lo);

      return len;
   }

   //
   // lenPushIdx
   //
   Core::FastU Info::lenPushIdx(IR::Arg const &arg, Core::FastU w)
   {
      //
      // lenSta
      //
      auto lenSta = [&](IR::Arg_Sta const &a) -> Core::FastU
      {
         if(a.idx->a == IR::ArgBase::Lit)
            return 8;
         else
         {
            Core::FastU len = lenPushArg(*a.idx, 0);

            if(a.off + w)
               len += 12;

            return len;
         }
      };

      switch(arg.a)
      {
      case IR::ArgBase::Sta:    return lenSta(arg.aSta);

      default:
         Core::Error(stmnt->pos, "bad lenPushIdx");
      }
   }

   //
   // Info::lenPushTmp
   //
   Core::FastU Info::lenPushTmp(Core::FastU)
   {
      return 8;
   }

   //
   // Info::lenString
   //
   std::size_t Info::lenString(Core::String s)
   {
      std::size_t len = 0;

      for(auto i = s.begin(), e = s.end(); i != e; ++i) switch(*i)
      {
      case '\0':
         if('0' <= i[1] && i[1] <= '7')
            len += 4;
         else
            len += 2;
         break;

      case '\\':
         len += 2;
         break;

      default:
         len += 1;
         break;
      }

      return len + 1;
   }

   //
   // Info::resolveGlyph
   //
   IR::Exp::CRef Info::resolveGlyph(Core::String glyph)
   {
      if(auto exp = prog->getGlyphData(glyph).value)
         return static_cast<IR::Exp::CRef>(exp);

      Core::ErrorUndef(stmnt->pos, "glyph", glyph);
   }

   //
   // Info::CodeBase
   //
   Core::FastU Info::CodeBase()
   {
      return 24;
   }

   //
   // Info:GetParamMax
   //
   Core::FastU Info::GetParamMax(IR::CallType call)
   {
      switch(call)
      {
         // Numbered Scripts.
      case IR::CallType::SScriptI:
      case IR::CallType::ScriptI:
         return ScriptIParam;

         // Named scripts.
      case IR::CallType::SScriptS:
      case IR::CallType::ScriptS:
         return ScriptSParam;

         // Normal functions.
      case IR::CallType::StdCall:
      case IR::CallType::StkCall:
         return 255;

      default:
         return -1;
      }
   }

   //
   // Info::GetRetnMax
   //
   Core::FastU Info::GetRetnMax(IR::CallType call)
   {
      switch(call)
      {
         // Sync Scripts.
      case IR::CallType::SScriptI:
      case IR::CallType::SScriptS:
         return 0;

         // Normal functions.
      case IR::CallType::StdCall:
      case IR::CallType::StkCall:
         return StkCallRetn;

      default:
         return 1;
      }
   }

   //
   // Info::GetScriptValue
   //
   Core::FastU Info::GetScriptValue(IR::Function const &script)
   {
      if(IsScriptS(script.ctype))
         return ~script.valueInt;
      else
         return script.valueInt;
   }

   //
   // Info::IsNull
   //
   bool Info::IsNull(IR::Value_Funct const &val)
   {
      if(Target::IsZeroNull_Funct(val.vtype.callT))
         return (val.value & 0xFFFFFFFF) == 0;
      else
         return (val.value & 0xFFFFFFFF) == 0xFFFFFFFF;
   }

   //
   // Info::IsNull
   //
   bool Info::IsNull(IR::Value_StrEn const &val)
   {
      if(Target::IsZeroNull_StrEn())
         return (val.value & 0xFFFFFFFF) == 0;
      else
         return (val.value & 0xFFFFFFFF) == 0xFFFFFFFF;
   }

   //
   // Info::isNull_Funct
   //
   bool Info::IsNull_Funct(Core::FastU val)
   {
      if(Target::IsZeroNull_Funct(IR::CallType::StdCall))
         return (val & 0xFFFFFFFF) == 0;
      else
         return (val & 0xFFFFFFFF) == 0xFFFFFFFF;
   }

   //
   // Info::IsNull_StrEn
   //
   bool Info::IsNull_StrEn(Core::FastU val)
   {
      if(Target::IsZeroNull_StrEn())
         return (val & 0xFFFFFFFF) == 0;
      else
         return (val & 0xFFFFFFFF) == 0xFFFFFFFF;
   }

   //
   // Info::IsScript
   //
   bool Info::IsScript(IR::CallType ctype)
   {
      return
         ctype == IR::CallType::SScriptI ||
         ctype == IR::CallType::SScriptS ||
         ctype == IR::CallType::ScriptI ||
         ctype == IR::CallType::ScriptS;
   }

   //
   // Info::IsScriptS
   //
   bool Info::IsScriptS(IR::CallType ctype)
   {
      return
         ctype == IR::CallType::SScriptS ||
         ctype == IR::CallType::ScriptS;
   }
}

// EOF

