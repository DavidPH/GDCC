//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2019 David Hill
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
#include "Option/Exception.hpp"
#include "Option/Function.hpp"
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
   static Option::Bool OutputIROpt
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("ir-output").setName('c')
         .setGroup("output")
         .setDescS("Generate an IR file instead of bytecode."),

      &OutputIR
   };

   //
   // --ir-process
   //
   static std::vector<std::string> ProcessIRList;
   static Option::Function ProcessIROpt
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("ir-process")
         .setGroup("debugging")
         .setDescS("Specifies IR processing steps.")
         .setDescL("Specifies IR processing steps. Steps are specified as a "
            "colon-separated list of step names. This option is only "
            "useful for debugging compiler internals."),

      [](Option::Base *, Option::Args const &args) -> std::size_t
      {
         if(args.optFalse)
            return ProcessIRList.clear(), 0;

         if(!args.argC)
            Option::Exception::Error(args, "argument required");

         char const *itr = args.argV[0];
         for(char const *sep; (sep = std::strchr(itr, ':')); itr = sep + 1)
            ProcessIRList.emplace_back(itr, sep);
         ProcessIRList.emplace_back(itr);

         return 1;
      }
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
   // ProcessIR
   //
   void ProcessIR(IR::Program &prog, BC::Info *info)
   {
      if(!info) return;

      if(ProcessIROpt.processed) for(auto const &proc : ProcessIRList)
      {
              if(proc == "chk") info->chk(prog);
         else if(proc == "gen") info->gen(prog);
         else if(proc == "opt") info->opt(prog);
         else if(proc == "pre") info->pre(prog);
         else if(proc == "tr")  info->tr(prog);
      }
      else
      {
         info->chk(prog);
         info->pre(prog);
         info->opt(prog);
         info->tr(prog);
         info->opt(prog);
         info->tr(prog);
      }
   }

   //
   // PutBytecode
   //
   void PutBytecode(std::ostream &out, IR::Program &prog, BC::Info *info)
   {
      if(!info)
         Core::Error({}, "invalid target");

      ProcessIR(prog, info);

      info->gen(prog);
      info->put(prog, out);
   }

   //
   // PutIR
   //
   void PutIR(std::ostream &out, IR::Program &prog, BC::Info *info)
   {
      if(ProcessIROpt.processed)
         ProcessIR(prog, info);

      IR::OArchive arc{out};
      arc.putHead();
      arc << prog;
      arc.putTail();
   }
}

// EOF

