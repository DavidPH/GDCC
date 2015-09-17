//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Program entry point.
//
//-----------------------------------------------------------------------------

#include "AS/Parse.hpp"

#include "CC/Parse.hpp"

#include "Core/Option.hpp"
#include "Core/Path.hpp"

#include "IR/Program.hpp"

#include "LD/Linker.hpp"

#include "Platform/Platform.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// MakeLib_AS
//
static void MakeLib_AS(GDCC::IR::Program &prog, std::string path, char const *name)
{
   GDCC::Core::PathAppend(path, name);
   GDCC::AS::ParseFile(path.data(), prog);
}

//
// MakeLib_CC
//
static void MakeLib_CC(GDCC::IR::Program &prog, std::string path, char const *name)
{
   GDCC::Core::PathAppend(path, name);
   GDCC::CC::ParseFile(path.data(), prog);
}

//
// MakeLib_libGDCC
//
static void MakeLib_libGDCC(GDCC::IR::Program &prog)
{
   std::string path = GDCC::Core::GetSystemPath();
   GDCC::Core::PathAppend(path, "lib");
   GDCC::Core::PathAppend(path, "src");
   GDCC::Core::PathAppend(path, "libGDCC");

   MakeLib_CC(prog, path, "alloc.c");

   if(GDCC::Platform::IsFamily_ZDACS())
   {
      std::string pathSub = path;
      GDCC::Core::PathAppend(pathSub, "ZDACS");

      MakeLib_AS(prog, pathSub, "AddF.asm");
      MakeLib_AS(prog, pathSub, "DivF.asm");
      MakeLib_AS(prog, pathSub, "MulF.asm");
      MakeLib_AS(prog, pathSub, "SubF.asm");
   }
}

//
// MakeLib_libacs
//
static void MakeLib_libacs(GDCC::IR::Program &)
{
}

//
// MakeLib_libc
//
static void MakeLib_libc(GDCC::IR::Program &prog)
{
   std::string path = GDCC::Core::GetSystemPath();
   GDCC::Core::PathAppend(path, "lib");
   GDCC::Core::PathAppend(path, "src");
   GDCC::Core::PathAppend(path, "libc");

   MakeLib_CC(prog, path, "ctype.c");
   MakeLib_CC(prog, path, "errno.c");
   MakeLib_CC(prog, path, "exp.c");
   MakeLib_CC(prog, path, "format.c");
   MakeLib_CC(prog, path, "locale.c");
   MakeLib_CC(prog, path, "math.c");
   MakeLib_CC(prog, path, "printf.c");
   MakeLib_CC(prog, path, "round.c");
   MakeLib_CC(prog, path, "setjmp.c");
   MakeLib_CC(prog, path, "sort.c");
   MakeLib_CC(prog, path, "stdfix.c");
   MakeLib_CC(prog, path, "stdio.c");
   MakeLib_CC(prog, path, "stdlib.c");
   MakeLib_CC(prog, path, "string.c");
   MakeLib_CC(prog, path, "strto.c");
   MakeLib_CC(prog, path, "trig.c");

   if(GDCC::Platform::IsFamily_ZDACS())
   {
      std::string pathSub = path;
      GDCC::Core::PathAppend(pathSub, "ZDACS");

      MakeLib_AS(prog, pathSub, "approx.asm");
      MakeLib_AS(prog, pathSub, "fpclassify.asm");
   }
}

//
// MakeLib
//
static void MakeLib()
{
   GDCC::IR::Program prog;

   for(auto const &arg : GDCC::Core::GetOptionArgs())
   {
           if(!strcmp(arg, "libGDCC")) MakeLib_libGDCC(prog);
      else if(!strcmp(arg, "libacs"))  MakeLib_libacs(prog);
      else if(!strcmp(arg, "libc"))    MakeLib_libc(prog);
      else
      {
         std::cerr << "ERROR: unknown library: '" << arg << "'\n";
         throw EXIT_FAILURE;
      }
   }

   // Write output.
   GDCC::LD::Link(prog, GDCC::Core::GetOptionOutput());
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// main
//
int main(int argc, char *argv[])
{
   auto &opts = GDCC::Core::GetOptions();

   opts.list.name     = "gdcc-makelib";
   opts.list.nameFull = "GDCC MakeLib";

   opts.list.usage = "[option]... [source]...";

   opts.list.descS =
      "Compiles GDCC target libraries. Output defaults to last loose argument.";

   opts.list.descL =
      "Compiles GDCC target libraries. Inputs are names of libraries to "
      "compile. Current target libraries: libGDCC, libacs, libc.\n"
      "\n"
      "Output defaults to last loose argument.";

   try
   {
      GDCC::Core::ProcessOptions(opts, argc, argv);
      MakeLib();
   }
   catch(std::exception const &e)
   {
      std::cerr << e.what() << std::endl;
      return EXIT_FAILURE;
   }
   catch(int e)
   {
      return e;
   }
}

// EOF

