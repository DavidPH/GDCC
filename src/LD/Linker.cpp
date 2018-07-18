//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Utilities for program linking and output.
//
//-----------------------------------------------------------------------------

#include "LD/Linker.hpp"

#include "BC/Info.hpp"

#if GDCC_BC_DGE
#include "BC/DGE/Info.hpp"
#endif

#if GDCC_BC_ZDACS
#include "BC/ZDACS/Info.hpp"
#endif

#include "Core/File.hpp"
#include "Core/Option.hpp"

#include "IR/OArchive.hpp"
#include "IR/Program.hpp"

#include "Option/Bool.hpp"
#include "Option/CStrV.hpp"

#include "Platform/Platform.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Options                                                                    |
//

namespace GDCC::LD
{
   //
   // -c, --ir-output
   //
   static Option::Bool OutputIROpt
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("ir-output").setName('c')
         .setGroup("output")
         .setDescS("Generate an IR file instead of bytecode."),

      &OutputIR
   };

   //
   // -l, --library
   //
   static Option::CStrV Libraries
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("library").setName('l')
         .setGroup("output")
         .setDescS("Adds a library name to import at runtime."),

      1
   };
}


//----------------------------------------------------------------------------|
// Extern Obects                                                              |
//

namespace GDCC::LD
{
   bool OutputIR = false;
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::LD
{
   //
   // GetBytecodeInfo
   //
   std::unique_ptr<BC::Info> GetBytecodeInfo(Platform::Target target,
      Platform::Format)
   {
      switch(target)
      {
      case Platform::Target::None:
         return nullptr;

      case Platform::Target::Doominati:
         #if GDCC_BC_DGE
         return std::unique_ptr<BC::Info>{new BC::DGE::Info};
         #else
         return nullptr;
         #endif

      case Platform::Target::ZDoom:
      case Platform::Target::Zandronum:
         #if GDCC_BC_ZDACS
         return std::unique_ptr<BC::Info>{new BC::ZDACS::Info};
         #else
         return nullptr;
         #endif
      }

      return nullptr;
   }

   //
   // Link
   //
   void Link(IR::Program &prog, char const *outName)
   {
      // Add libraries.
      for(auto const &lib : Libraries)
         prog.getImport(lib);

      auto buf = Core::FileOpenStream(outName, std::ios_base::out | std::ios_base::binary);
      std::ostream out{buf.get()};

      auto info = GetBytecodeInfo(Platform::TargetCur, Platform::FormatCur);

      if(OutputIR)
         PutIR(out, prog, info.get());
      else
      {
         PutBytecode(out, prog, info.get());

         if(info)
            info->putExtra(prog);
      }
   }

   //
   // ProcessIR
   //
   void ProcessIR(IR::Program &prog, BC::Info *info)
   {
      if(!info) return;

      info->pre(prog);
      info->opt(prog);
      info->tr(prog);
      info->opt(prog);
      info->tr(prog);
   }

   //
   // PutBytecode
   //
   void PutBytecode(std::ostream &out, IR::Program &prog, BC::Info *info)
   {
      if(!info)
      {
         std::cerr << "invalid target\n";
         throw EXIT_FAILURE;
      }

      ProcessIR(prog, info);

      info->gen(prog);
      info->put(prog, out);
   }

   //
   // PutIR
   //
   void PutIR(std::ostream &out, IR::Program &prog, BC::Info *)
   {
      IR::OArchive(out).putHeader() << prog;
   }
}

// EOF

