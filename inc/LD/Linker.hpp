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

#ifndef GDCC__LD__Linker_H__
#define GDCC__LD__Linker_H__

#include <memory>
#include <ostream>


//----------------------------------------------------------------------------|
// Tyoes                                                                      |
//

namespace GDCC
{
   namespace Bytecode
   {
      class Info;
   }

   namespace IR
   {
      class Program;
   }

   namespace Platform
   {
      enum class Format;
      enum class Target;
   }
}


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

namespace GDCC
{
   namespace LD
   {
      extern bool OutputIR;
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace LD
   {
      std::unique_ptr<Bytecode::Info> GetBytecodeInfo(Platform::Target target,
         Platform::Format format);

      void Link(IR::Program &prog, char const *outName);

      void ProcessIR(IR::Program &prog, Bytecode::Info *info);

      void PutBytecode(std::ostream &out, IR::Program &prog, Bytecode::Info *info);
      void PutIR(std::ostream &out, IR::Program &prog, Bytecode::Info *info);
   }
}

#endif//GDCC__LD__Linker_H__

