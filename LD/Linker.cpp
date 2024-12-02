//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2024 David Hill
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

#include "Core/Exception.hpp"
#include "Core/File.hpp"
#include "Core/Option.hpp"

#include "IR/OArchive.hpp"
#include "IR/Program.hpp"

#include "Option/Bool.hpp"
#include "Option/CStrV.hpp"

#include "Target/Info.hpp"


//----------------------------------------------------------------------------|
// Options                                                                    |
//

namespace GDCC::LD
{
   //
   // -c, --ir-output
   //
   Option::Bool OutputIR
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("ir-output").setName('c')
         .setGroup("output")
         .setDescS("Generate an IR file instead of bytecode."),

      false
   };

   //
   // --ir-process
   //
   static Option::CStr ProcessIROpt
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("ir-process")
         .setGroup("debugging")
         .setDescS("Specifies IR processing steps.")
         .setDescL("Specifies IR processing steps. Steps are specified as a "
            "colon-separated list of step names. This option is only "
            "useful for debugging compiler internals.")
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
// Static Functions                                                           |
//

namespace GDCC::LD
{
   //
   // ProcessIR
   //
   static void ProcessIR(IR::Program &prog, BC::Info *info)
   {
      char const *data = ProcessIROpt.data();

      if(!data) return;

      auto proc = [&](char const *str, std::size_t len)
      {
         if(len == 0) {}

         else if(len == 2 && !std::memcmp(str, "tr", 2)) info->tr(prog);

         else if(len == 3 && !std::memcmp(str, "chk", 3)) info->chk(prog);
         else if(len == 3 && !std::memcmp(str, "gen", 3)) info->gen(prog);
         else if(len == 3 && !std::memcmp(str, "opt", 3)) info->opt(prog);
         else if(len == 3 && !std::memcmp(str, "pre", 3)) info->pre(prog);

         else
            Core::ErrorExpect({}, "IR processing step", {str, len});
      };

      for(char const *next; (next = std::strchr(data, ':')); data = next + 1)
         proc(data, next - data);
      proc(data, std::strlen(data));
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::LD
{
   //
   // GetBytecodeInfo
   //
   std::unique_ptr<BC::Info> GetBytecodeInfo(Target::Engine engine,
      Target::Format)
   {
      switch(engine)
      {
      case Target::Engine::None:
         return nullptr;

      case Target::Engine::Doominati:
         #if GDCC_BC_DGE
         return std::unique_ptr<BC::Info>{new BC::DGE::Info};
         #else
         return nullptr;
         #endif

      case Target::Engine::Eternity:
      case Target::Engine::Hexen:
      case Target::Engine::ZDoom:
      case Target::Engine::Zandronum:
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

      auto info = GetBytecodeInfo(Target::EngineCur, Target::FormatCur);

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
   // PutBytecode
   //
   void PutBytecode(std::ostream &out, IR::Program &prog, BC::Info *info)
   {
      if(!info)
         Core::Error({}, "invalid target");

      if(ProcessIROpt.processed)
      {
         ProcessIR(prog, info);
      }
      else
      {
         info->chk(prog);
         info->pre(prog);
         info->opt(prog);
         info->tr(prog);
         info->opt(prog);
         info->tr(prog);
         info->gen(prog);
      }

      info->put(prog, out);
   }

   //
   // PutIR
   //
   void PutIR(std::ostream &out, IR::Program &prog, BC::Info *info)
   {
      if(ProcessIROpt.processed && info)
         ProcessIR(prog, info);

      IR::OArchive arc{out};
      arc.putHead();
      arc << prog;
      arc.putTail();
   }
}

// EOF

