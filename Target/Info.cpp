//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2025 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Target information/selection.
//
//-----------------------------------------------------------------------------

#include "Target/Info.hpp"

#include "Target/Addr.hpp"
#include "Target/CallType.hpp"

#include "Core/Option.hpp"

#include "Option/Bool.hpp"
#include "Option/Exception.hpp"

#include <cstring>


//----------------------------------------------------------------------------|
// Options                                                                    |
//

namespace GDCC::Target
{
   //
   // --target-format
   //
   static Option::Function FormatOpt
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("target-format")
         .setGroup("output")
         .setDescS("Selects target format.")
         .setDescL("Selects target format. This option may affect higher level "
            "codegen and should be set at all stages of compiling. Valid "
            "arguments are: ACSE, DGE_NTS."),

      [](Option::Base *, Option::Args const &args) -> std::size_t
      {
         if(!args.argC)
            Option::Exception::Error(args, "argument required");

         if(!std::strcmp(args.argV[0], "ACS0"))
            FormatCur = Format::ACS0;
         else if(!std::strcmp(args.argV[0], "ACSE"))
            FormatCur = Format::ACSE;
         else if(!std::strcmp(args.argV[0], "DGE_NTS"))
            FormatCur = Format::DGE_NTS;
         else
            Option::Exception::Error(args, "argument invalid");

         return 1;
      }
   };

   //
   // --target-engine
   //
   static Option::Function EngineOpt
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("target-engine")
         .setGroup("output")
         .setDescS("Selects target engine.")
         .setDescL("Selects target engine. This option may affect higher level "
            "codegen and should be set at all stages of compiling. Valid "
            "arguments are: Doominati, Eternity, Hexen, ZDoom, Zandronum."),

      [](Option::Base *, Option::Args const &args) -> std::size_t
      {
         if(!args.argC)
            Option::Exception::Error(args, "argument required");

         auto setFormat = [](Format fmt)
            {if(!FormatOpt.processed) FormatCur = fmt;};

         if(!std::strcmp(args.argV[0], "Doominati"))
            EngineCur = Engine::Doominati, setFormat(Format::DGE_NTS);
         else if(!std::strcmp(args.argV[0], "Eternity"))
            EngineCur = Engine::Eternity, setFormat(Format::ACSE);
         else if(!std::strcmp(args.argV[0], "Hexen"))
            EngineCur = Engine::Hexen, setFormat(Format::ACS0);
         else if(!std::strcmp(args.argV[0], "ZDoom"))
            EngineCur = Engine::ZDoom, setFormat(Format::ACSE);
         else if(!std::strcmp(args.argV[0], "Zandronum"))
            EngineCur = Engine::Zandronum, setFormat(Format::ACSE);
         else
            Option::Exception::Error(args, "argument invalid");

         return 1;
      }
   };

   //
   // --zero-null-StrEn
   //
   static Option::Bool ZeroNull_StrEn
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("zero-null-StrEn")
         .setGroup("codegen")
         .setDescS("Enables zero representation for StrEn nulls.")
         .setDescL("Enables zero representation for StrEn nulls. Default is "
            "on. Option has no effect for DGE target."),

      true
   };
}


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

namespace GDCC::Target
{
   Engine EngineCur = Engine::None;
   Format FormatCur = Format::None;
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Target
{
   //
   // GetByteBitsI
   //
   unsigned GetByteBitsI()
   {
      switch(EngineCur)
      {
      case Engine::None:      return 8;
      case Engine::Doominati: return 8;
      case Engine::Eternity:  return 32;
      case Engine::Hexen:     return 32;
      case Engine::ZDoom:     return 32;
      case Engine::Zandronum: return 32;
      }

      return 0;
   }

   //
   // GetCallAutoAdd
   //
   unsigned GetCallAutoAdd(CallType call)
   {
      switch(EngineCur)
      {
      case Engine::Doominati:
      case Engine::Eternity:
      case Engine::Hexen:
      case Engine::ZDoom:
      case Engine::Zandronum:
         switch(call)
         {
         case CallType::SScriptI:
         case CallType::SScriptS:
         case CallType::StdCall:
            return 1;

         default:
            return 0;
         }

      default:
         return 0;
      }
   }

   //
   // GetWordAlign
   //
   unsigned GetWordAlign()
   {
      switch(EngineCur)
      {
      case Engine::None:      return 1;
      case Engine::Doominati: return 4;
      case Engine::Eternity:  return 1;
      case Engine::Hexen:     return 1;
      case Engine::ZDoom:     return 1;
      case Engine::Zandronum: return 1;
      }

      return 0;
   }

   //
   // GetWordBits
   //
   unsigned GetWordBits()
   {
      switch(EngineCur)
      {
      case Engine::None:      return 32;
      case Engine::Doominati: return 32;
      case Engine::Eternity:  return 32;
      case Engine::Hexen:     return 32;
      case Engine::ZDoom:     return 32;
      case Engine::Zandronum: return 32;
      }

      return 0;
   }

   //
   // GetWordBytes
   //
   unsigned GetWordBytes()
   {
      switch(EngineCur)
      {
      case Engine::None:      return 1;
      case Engine::Doominati: return 4;
      case Engine::Eternity:  return 1;
      case Engine::Hexen:     return 1;
      case Engine::ZDoom:     return 1;
      case Engine::Zandronum: return 1;
      }

      return 0;
   }

   //
   // GetWordPoint
   //
   unsigned GetWordPoint()
   {
      switch(EngineCur)
      {
      case Engine::None:      return 1;
      case Engine::Doominati: return 4;
      case Engine::Eternity:  return 1;
      case Engine::Hexen:     return 1;
      case Engine::ZDoom:     return 1;
      case Engine::Zandronum: return 1;
      }

      return 0;
   }

   //
   // GetWordShift
   //
   unsigned GetWordShift()
   {
      switch(EngineCur)
      {
      case Engine::None:      return 1;
      case Engine::Doominati: return 1;
      case Engine::Eternity:  return 1;
      case Engine::Hexen:     return 1;
      case Engine::ZDoom:     return 1;
      case Engine::Zandronum: return 1;
      }

      return 0;
   }

   //
   // IsAddrFirst
   //
   bool IsAddrFirst(AddrBase addr)
   {
      switch(addr)
      {
      case AddrBase::GblArr:
      case AddrBase::HubArr:
      case AddrBase::LocArr:
      case AddrBase::ModArr:
      case AddrBase::Sta:
         switch(EngineCur)
         {
         case Engine::Eternity:
         case Engine::Hexen:
         case Engine::ZDoom:
         case Engine::Zandronum:
            return true;

         default:
            return false;
         }

      default:
         return false;
      }
   }

   //
   // IsCallAutoProp
   //
   bool IsCallAutoProp(CallType call)
   {
      switch(EngineCur)
      {
      case Engine::Doominati:
      case Engine::Eternity:
      case Engine::Hexen:
      case Engine::ZDoom:
      case Engine::Zandronum:
         switch(call)
         {
         case CallType::SScriptI:
         case CallType::SScriptS:
         case CallType::StdCall:
            return true;

         default:
            return false;
         }

      default:
         return false;
      }
   }

   //
   // IsCallVaria
   //
   bool IsCallVaria(CallType call)
   {
      switch(EngineCur)
      {
      case Engine::Doominati:
      case Engine::Eternity:
      case Engine::Hexen:
      case Engine::ZDoom:
      case Engine::Zandronum:
         switch(call)
         {
         case CallType::SScriptI:
         case CallType::SScriptS:
         case CallType::StdCall:
            return true;

         default:
            return false;
         }

      default:
         return false;
      }
   }

   //
   // IsDelayFunction
   //
   bool IsDelayFunction()
   {
      switch(EngineCur)
      {
      case Engine::None:
      case Engine::Doominati:
      case Engine::Eternity:
         return true;

      case Engine::Hexen:
      case Engine::ZDoom:
      case Engine::Zandronum:
         return false;
      }

      return false;
   }

   //
   // IsFamily_ZDACS
   //
   bool IsFamily_ZDACS()
   {
      switch(EngineCur)
      {
      case Engine::Eternity:
      case Engine::ZDoom:
      case Engine::Zandronum:
         return true;

      default:
         return false;
      }
   }

   //
   // IsZeroNull_DJump
   //
   bool IsZeroNull_DJump()
   {
      return true;
   }

   //
   // IsZeroNull_Funct
   //
   bool IsZeroNull_Funct(CallType call)
   {
      switch(call)
      {
      case CallType::ScriptS:
         return IsZeroNull_StrEn();

      default:
         return true;
      }
   }

   //
   // IsZeroNull_Point
   //
   bool IsZeroNull_Point(AddrBase addr)
   {
      switch(addr)
      {
      case AddrBase::GblArr: return false;
      case AddrBase::HubArr: return false;
      case AddrBase::ModArr: return false;
      case AddrBase::StrArr: return false;

      default: return true;
      }
   }

   //
   // IsZeroNull_StrEn
   //
   bool IsZeroNull_StrEn()
   {
      switch(EngineCur)
      {
      case Engine::Doominati:
         return true;

      default:
         return ZeroNull_StrEn;
      }

   }
}

// EOF

