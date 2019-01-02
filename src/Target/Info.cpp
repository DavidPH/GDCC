//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
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
#include "Core/String.hpp"

#include "Option/Bool.hpp"
#include "Option/Exception.hpp"


//----------------------------------------------------------------------------|
// Options                                                                    |
//

namespace GDCC::Target
{
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
            "arguments are: Doominati, ZDoom, Zandronum."),

      [](Option::Base *, Option::Args const &args) -> std::size_t
      {
         if(!args.argC)
            Option::Exception::Error(args, "argument required");

         switch(Core::String::Find(args.argV[0]))
         {
         case Core::STR_Doominati: EngineCur = Engine::Doominati; break;
         case Core::STR_ZDoom:     EngineCur = Engine::ZDoom;     break;
         case Core::STR_Zandronum: EngineCur = Engine::Zandronum; break;

         default:
            Option::Exception::Error(args, "argument invalid");
         }

         return 1;
      }
   };

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

         switch(Core::String::Find(args.argV[0]))
         {
         case Core::STR_ACSE:    FormatCur = Format::ACSE;    break;
         case Core::STR_DGE_NTS: FormatCur = Format::DGE_NTS; break;

         default:
            Option::Exception::Error(args, "argument invalid");
         }

         return 1;
      }
   };

   //
   // --zero-null-StrEn
   //
   static bool ZeroNull_StrEn = true;
   static Option::Bool ZeroNull_StrEnOpt
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("zero-null-StrEn")
         .setGroup("codegen")
         .setDescS("Enables zero representation for StrEn nulls.")
         .setDescL("Enables zero representation for StrEn nulls. Default is "
            "on. Option has no effect for DGE target."),

      &ZeroNull_StrEn
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
      case Engine::Zandronum: return 32;
      case Engine::ZDoom:     return 32;
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
      case Engine::Zandronum:
      case Engine::ZDoom:
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
      case Engine::Zandronum: return 1;
      case Engine::ZDoom:     return 1;
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
      case Engine::Zandronum: return 32;
      case Engine::ZDoom:     return 32;
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
      case Engine::Zandronum: return 1;
      case Engine::ZDoom:     return 1;
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
      case Engine::Zandronum: return 1;
      case Engine::ZDoom:     return 1;
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
      case Engine::Zandronum: return 1;
      case Engine::ZDoom:     return 1;
      }

      return 0;
   }

   //
   // IsCallAutoProp
   //
   bool IsCallAutoProp(CallType call)
   {
      switch(EngineCur)
      {
      case Engine::Doominati:
      case Engine::Zandronum:
      case Engine::ZDoom:
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
      case Engine::Zandronum:
      case Engine::ZDoom:
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
   // IsFamily_ZDACS
   //
   bool IsFamily_ZDACS()
   {
      switch(EngineCur)
      {
      case Engine::Zandronum:
      case Engine::ZDoom:
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

   //
   // MustEmitObject
   //
   bool MustEmitObject(AddrBase addr)
   {
      switch(addr)
      {
      case AddrBase::ModArr: return true;

      default: return false;
      }
   }
}

// EOF

