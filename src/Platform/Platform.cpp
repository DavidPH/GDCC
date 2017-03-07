//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Platform information/selection.
//
//-----------------------------------------------------------------------------

#include "Platform/Platform.hpp"

#include "Core/Option.hpp"
#include "Core/String.hpp"

#include "IR/Addr.hpp"
#include "IR/CallType.hpp"

#include "Option/Bool.hpp"
#include "Option/Exception.hpp"


//----------------------------------------------------------------------------|
// Options                                                                    |
//

namespace GDCC
{
   namespace Platform
   {

      //
      // --bc-format
      //
      static Option::Function FormatOpt
      {
         &Core::GetOptionList(), Option::Base::Info()
            .setName("bc-format")
            .setGroup("output")
            .setDescS("Selects bytecode format.")
            .setDescL("Selects bytecode format. This option may affect higher level "
               "codegen and should be set at all stages of compiling. Valid "
               "arguments are: ACSE, DGE_NTS, MgC_NTS."),

         [](Option::Base *, Option::Args const &args) -> std::size_t
         {
            if(!args.argC)
               Option::Exception::Error(args, "argument required");

            switch(Core::String::Find(args.argV[0]))
            {
            case Core::STR_ACSE:    FormatCur = Format::ACSE;    break;
            case Core::STR_DGE_NTS: FormatCur = Format::DGE_NTS; break;
            case Core::STR_MgC_NTS: FormatCur = Format::MgC_NTS; break;

            default:
               Option::Exception::Error(args, "argument invalid");
            }

            return 1;
         }
      };

      //
      // --bc-target
      //
      static Option::Function TargetOpt
      {
         &Core::GetOptionList(), Option::Base::Info()
            .setName("bc-target")
            .setGroup("output")
            .setDescS("Selects target engine.")
            .setDescL("Selects target engine. This option may affect higher level "
               "codegen and should be set at all stages of compiling. Valid "
               "arguments are: Doominati, MageCraft, ZDoom, Zandronum."),

         [](Option::Base *, Option::Args const &args) -> std::size_t
         {
            if(!args.argC)
               Option::Exception::Error(args, "argument required");

            switch(Core::String::Find(args.argV[0]))
            {
            case Core::STR_Doominati: TargetCur = Target::Doominati; break;
            case Core::STR_MageCraft: TargetCur = Target::MageCraft; break;
            case Core::STR_ZDoom:     TargetCur = Target::ZDoom;     break;
            case Core::STR_Zandronum: TargetCur = Target::Zandronum; break;

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
               "on. Option has no effect for MageCraft target."),

         &ZeroNull_StrEn
      };
   }
}


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

namespace GDCC
{
   namespace Platform
   {
      Format FormatCur = Format::None;
      Target TargetCur = Target::None;
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace Platform
   {
      //
      // GetByteBitsI
      //
      unsigned GetByteBitsI()
      {
         switch(TargetCur)
         {
         case Target::None:      return 8;
         case Target::Doominati: return 8;
         case Target::MageCraft: return 8;
         case Target::Zandronum: return 32;
         case Target::ZDoom:     return 32;
         }

         return 0;
      }

      //
      // GetCallAutoAdd
      //
      unsigned GetCallAutoAdd(IR::CallType call)
      {
         switch(TargetCur)
         {
         case Target::Doominati:
         case Target::Zandronum:
         case Target::ZDoom:
            switch(call)
            {
            case IR::CallType::SScriptI:
            case IR::CallType::SScriptS:
            case IR::CallType::StdCall:
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
         switch(TargetCur)
         {
         case Target::None:      return 1;
         case Target::Doominati: return 4;
         case Target::MageCraft: return 4;
         case Target::Zandronum: return 1;
         case Target::ZDoom:     return 1;
         }

         return 0;
      }

      //
      // GetWordBits
      //
      unsigned GetWordBits()
      {
         switch(TargetCur)
         {
         case Target::None:      return 32;
         case Target::Doominati: return 32;
         case Target::MageCraft: return 32;
         case Target::Zandronum: return 32;
         case Target::ZDoom:     return 32;
         }

         return 0;
      }

      //
      // GetWordBytes
      //
      unsigned GetWordBytes()
      {
         switch(TargetCur)
         {
         case Target::None:      return 1;
         case Target::Doominati: return 4;
         case Target::MageCraft: return 4;
         case Target::Zandronum: return 1;
         case Target::ZDoom:     return 1;
         }

         return 0;
      }

      //
      // GetWordPoint
      //
      unsigned GetWordPoint()
      {
         switch(TargetCur)
         {
         case Target::None:      return 1;
         case Target::Doominati: return 4;
         case Target::MageCraft: return 1;
         case Target::Zandronum: return 1;
         case Target::ZDoom:     return 1;
         }

         return 0;
      }

      //
      // GetWordShift
      //
      unsigned GetWordShift()
      {
         switch(TargetCur)
         {
         case Target::None:      return 1;
         case Target::Doominati: return 1;
         case Target::MageCraft: return 4;
         case Target::Zandronum: return 1;
         case Target::ZDoom:     return 1;
         }

         return 0;
      }

      //
      // IsCallAutoProp
      //
      bool IsCallAutoProp(IR::CallType call)
      {
         switch(TargetCur)
         {
         case Target::Doominati:
         case Target::Zandronum:
         case Target::ZDoom:
            switch(call)
            {
            case IR::CallType::SScriptI:
            case IR::CallType::SScriptS:
            case IR::CallType::StdCall:
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
      bool IsCallVaria(IR::CallType call)
      {
         switch(TargetCur)
         {
         case Target::Doominati:
         case Target::Zandronum:
         case Target::ZDoom:
            switch(call)
            {
            case IR::CallType::SScriptI:
            case IR::CallType::SScriptS:
            case IR::CallType::StdCall:
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
         switch(TargetCur)
         {
         case Target::Zandronum:
         case Target::ZDoom:
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
      bool IsZeroNull_Funct(IR::CallType call)
      {
         switch(call)
         {
         case IR::CallType::ScriptS:
            return IsZeroNull_StrEn();

         default:
            return true;
         }
      }

      //
      // IsZeroNull_Point
      //
      bool IsZeroNull_Point(IR::AddrBase addr)
      {
         switch(addr)
         {
         case IR::AddrBase::GblArr: return false;
         case IR::AddrBase::HubArr: return false;
         case IR::AddrBase::ModArr: return false;
         case IR::AddrBase::StrArr: return false;

         default: return true;
         }
      }

      //
      // IsZeroNull_StrEn
      //
      bool IsZeroNull_StrEn()
      {
         switch(TargetCur)
         {
         case Target::Doominati:
         case Target::MageCraft:
            return true;

         default:
            return ZeroNull_StrEn;
         }

      }

      //
      // MustEmitObject
      //
      bool MustEmitObject(IR::AddrBase addr)
      {
         switch(addr)
         {
         case IR::AddrBase::ModArr: return true;

         default: return false;
         }
      }
   }
}

// EOF

