//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Platform information/selection.
//
//-----------------------------------------------------------------------------

#include "Platform.hpp"

#include "GDCC/String.hpp"

#include "Option/Option.hpp"


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace Bytecode
{
   Format FormatCur = Format::None;
   Target TargetCur = Target::None;
}

namespace Option
{
   //
   // --bc-format
   //
   OptionCall FormatOpt{'\0', "bc-format", "output", "Selects output format.",
      nullptr, [](strp opt, uint optf, uint argc, strv argv) -> uint
   {
      if(!argc)
         Exception::Error(opt, optf, "requires argument");

      switch(static_cast<GDCC::StringIndex>(GDCC::FindString(argv[0])))
      {
      case GDCC::STR_ACSE:    Bytecode::FormatCur = Bytecode::Format::ACSE;    break;
      case GDCC::STR_MgC_NTS: Bytecode::FormatCur = Bytecode::Format::MgC_NTS; break;

      default:
         Exception::Error(opt, optf, "invalid argument");
      }

      return 1;
   }};

   //
   // --bc-target
   //
   OptionCall TargetOpt{'\0', "bc-target", "output", "Selects target engine.",
      nullptr, [](strp opt, uint optf, uint argc, strv argv) -> uint
   {
      if(!argc)
         Exception::Error(opt, optf, "requires argument");

      switch(static_cast<GDCC::StringIndex>(GDCC::FindString(argv[0])))
      {
      case GDCC::STR_MageCraft: Bytecode::TargetCur = Bytecode::Target::MageCraft; break;
      case GDCC::STR_ZDoom:     Bytecode::TargetCur = Bytecode::Target::ZDoom;     break;

      default:
         Exception::Error(opt, optf, "invalid argument");
      }

      return 1;
   }};
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Bytecode
{
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
   }
}

// EOF

