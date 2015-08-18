//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Utilities for program linking and output.
//
//-----------------------------------------------------------------------------

#include "LD/Linker.hpp"

#include "Bytecode/MgC/Info.hpp"
#include "Bytecode/ZDACS/Info.hpp"

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

namespace GDCC
{
   namespace LD
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
}


//----------------------------------------------------------------------------|
// Extern Obects                                                              |
//

namespace GDCC
{
   namespace LD
   {
      bool OutputIR = false;
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace LD
   {
      //
      // GetBytecodeInfo
      //
      std::unique_ptr<Bytecode::Info> GetBytecodeInfo(Platform::Target target,
         Platform::Format)
      {
         switch(target)
         {
         case Platform::Target::None:
            return nullptr;

         case Platform::Target::MageCraft:
            return std::unique_ptr<Bytecode::Info>{new Bytecode::MgC::Info};

         case Platform::Target::ZDoom:
            return std::unique_ptr<Bytecode::Info>{new Bytecode::ZDACS::Info};
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
         if(!buf)
         {
            std::cerr << "couldn't open '" << outName << "' for writing\n";
            throw EXIT_FAILURE;
         }
         std::ostream out{buf.get()};

         auto info = GetBytecodeInfo(Platform::TargetCur, Platform::FormatCur);

         if(OutputIR)
            PutIR(out, prog, info.get());
         else
            PutBytecode(out, prog, info.get());
      }

      //
      // ProcessIR
      //
      void ProcessIR(IR::Program &prog, Bytecode::Info *info)
      {
         if(!info) return;

         info->pre(prog);
         info->tr(prog);
      }

      //
      // PutBytecode
      //
      void PutBytecode(std::ostream &out, IR::Program &prog, Bytecode::Info *info)
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
      void PutIR(std::ostream &out, IR::Program &prog, Bytecode::Info *)
      {
         IR::OArchive(out).putHeader() << prog;
      }
   }
}

// EOF

