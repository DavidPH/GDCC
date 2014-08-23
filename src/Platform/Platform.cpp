//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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

#include "Option/Exception.hpp"


//----------------------------------------------------------------------------|
// Options                                                                    |
//

//
// --bc-format
//
static GDCC::Option::Function FormatOpt
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("bc-format")
      .setGroup("output")
      .setDescS("Selects bytecode format.")
      .setDescL("Selects bytecode format. This option may affect higher level "
         "codegen and should be set at all stages of compiling."),

   [](GDCC::Option::Base *, GDCC::Option::Args const &args) -> std::size_t
   {
      using namespace GDCC;

      if(!args.argC)
         Option::Exception::Error(args, "argument required");

      switch(Core::String::Find(args.argV[0]))
      {
      case Core::STR_ACSE:
         Platform::FormatCur = Platform::Format::ACSE;    break;
      case Core::STR_MgC_NTS:
         Platform::FormatCur = Platform::Format::MgC_NTS; break;

      default:
         Option::Exception::Error(args, "argument invalid");
      }

      return 1;
   }
};

//
// --bc-target
//
static GDCC::Option::Function TargetOpt
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("bc-target")
      .setGroup("output")
      .setDescS("Selects target engine."),

   [](GDCC::Option::Base *, GDCC::Option::Args const &args) -> std::size_t
   {
      using namespace GDCC;

      if(!args.argC)
         Option::Exception::Error(args, "argument required");

      switch(Core::String::Find(args.argV[0]))
      {
      case Core::STR_MageCraft:
         Platform::TargetCur = Platform::Target::MageCraft; break;
      case Core::STR_ZDoom:
         Platform::TargetCur = Platform::Target::ZDoom;     break;

      default:
         Option::Exception::Error(args, "argument invalid");
      }

      return 1;
   }
};


//----------------------------------------------------------------------------|
// Global Variables                                                           |
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
// Global Functions                                                           |
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
         case Target::ZDoom:     return 32;
         case Target::MageCraft: return 8;
         }

         return 0;
      }

      //
      // GetWordAlign
      //
      unsigned GetWordAlign()
      {
         switch(TargetCur)
         {
         case Target::None:      return 1;
         case Target::ZDoom:     return 1;
         case Target::MageCraft: return 4;
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
         case Target::ZDoom:     return 32;
         case Target::MageCraft: return 32;
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
         case Target::ZDoom:     return 1;
         case Target::MageCraft: return 4;
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
         case Target::ZDoom:     return 1;
         case Target::MageCraft: return 1;
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
         case Target::ZDoom:     return 1;
         case Target::MageCraft: return 4;
         }

         return 0;
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
         case IR::AddrBase::MapArr: return false;
         case IR::AddrBase::StrArr: return false;
         case IR::AddrBase::WldArr: return false;

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
         case Target::None:      return true;
         case Target::ZDoom:     return false;
         case Target::MageCraft: return true;
         }

         return true;
      }
   }
}

// EOF

