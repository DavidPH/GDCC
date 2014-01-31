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

#include "Bytecode/Platform.hpp"

#include "Core/Option.hpp"
#include "Core/String.hpp"

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
      .setDescS("Selects output format."),

   [](GDCC::Option::Base *, GDCC::Option::Args const &args) -> std::size_t
   {
      if(!args.argC)
         GDCC::Option::Exception::Error(args, "argument required");

      switch(static_cast<GDCC::Core::StringIndex>(GDCC::Core::FindString(args.argV[0])))
      {
      case GDCC::Core::STR_ACSE:
         GDCC::Bytecode::FormatCur = GDCC::Bytecode::Format::ACSE;    break;
      case GDCC::Core::STR_MgC_NTS:
         GDCC::Bytecode::FormatCur = GDCC::Bytecode::Format::MgC_NTS; break;

      default:
         GDCC::Option::Exception::Error(args, "argument invalid");
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
      if(!args.argC)
         GDCC::Option::Exception::Error(args, "argument required");

      switch(static_cast<GDCC::Core::StringIndex>(GDCC::Core::FindString(args.argV[0])))
      {
      case GDCC::Core::STR_MageCraft:
         GDCC::Bytecode::TargetCur = GDCC::Bytecode::Target::MageCraft; break;
      case GDCC::Core::STR_ZDoom:
         GDCC::Bytecode::TargetCur = GDCC::Bytecode::Target::ZDoom;     break;

      default:
         GDCC::Option::Exception::Error(args, "argument invalid");
      }

      return 1;
   }
};


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace Bytecode
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
   namespace Bytecode
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
   }
}

// EOF

