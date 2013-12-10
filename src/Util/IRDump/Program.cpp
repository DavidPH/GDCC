//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// IR::Program dump/disassembly functions.
//
//-----------------------------------------------------------------------------

#include "../IRDump.hpp"

#include "GDCC/IR/Program.hpp"

#include "Option/Option.hpp"


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace Option
{
   //
   // --dump-function
   //
   OptionDPtr<bool> DumpFunction{'\0', "dump-function", "output",
      "Dump IR::Function objects.", nullptr, &Util::IRDumpOpt_Function};

   //
   // --dump-headers
   //
   OptionDPtr<bool> DumpHeaders{'\0', "dump-headers", "output",
      "Include assembly headers in output.", nullptr, &Util::IRDumpOpt_Headers};

   //
   // --dump-import
   //
   OptionDPtr<bool> DumpImport{'\0', "dump-import", "output",
      "Dump IR::Import objects.", nullptr, &Util::IRDumpOpt_Import};

   //
   // --dump-object
   //
   OptionDPtr<bool> DumpObject{'\0', "dump-object", "output",
      "Dump IR::Object objects.", nullptr, &Util::IRDumpOpt_Object};

   //
   // --dump-space
   //
   OptionDPtr<bool> DumpSpace{'\0', "dump-space", "output",
      "Dump IR::Space objects.", nullptr, &Util::IRDumpOpt_Space};

   //
   // --dump-statistics
   //
   OptionDPtr<bool> DumpStatistics{'\0', "dump-statistics", "output",
      "Dump overall statistics.", nullptr, &Util::IRDumpOpt_Statistics};

   //
   // --dump-strent
   //
   OptionDPtr<bool> DumpStrEnt{'\0', "dump-strent", "output",
      "Dump IR::StrEnt objects.", nullptr, &Util::IRDumpOpt_StrEnt};
}


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// IRDumpHeader
//
static void IRDumpHeader(std::ostream &out, char const *str)
{
   auto sp = 75 - std::strlen(str);

   out << "\n\n;;";     for(int i = 76; i--;) out << '-'; out << "|\n";
   out << ";; " << str; for(int i = sp; i--;) out << ' '; out << "|\n";
   out << ";;\n";
}


//----------------------------------------------------------------------------|
// Glboal Functions                                                           |
//

namespace Util
{
   //
   // IRDump_Program
   //
   void IRDump_Program(std::ostream &out, GDCC::IR::Program const &prog)
   {
      // File header.
      if(IRDumpOpt_Headers)
      {
         out << ";;"; for(int i = 77; i--;) out << '-'; out << '\n';
         out << ";;\n";
         out << ";; IR dump from gdcc-irdump " << Option::Option::Help_Version << '\n';
         out << ";;\n";
         out << ";;"; for(int i = 77; i--;) out << '-'; out << '\n';
      }

      // Overall statistics.
      if(IRDumpOpt_Statistics)
      {
         if(IRDumpOpt_Headers) out << ";;\n";

         out << ";; Functions: "               << prog.sizeFunction()    << '\n';
         out << ";; Imports: "                 << prog.sizeImport()      << '\n';
         out << ";; Objects: "                 << prog.sizeObject()      << '\n';
         out << ";; Address Spaces (GblArs): " << prog.sizeSpaceGblArs() << '\n';
         out << ";; Address Spaces (MapArs): " << prog.sizeSpaceMapArs() << '\n';
         out << ";; Address Spaces (WldArs): " << prog.sizeSpaceWldArs() << '\n';
         out << ";; String Table Entries: "    << prog.sizeStrEnt()      << '\n';

         if(IRDumpOpt_Headers)
         {
            out << ";;\n";
            out << ";;"; for(int i = 77; i--;) out << '-'; out << '\n';
         }
      }

      // Functions
      if(IRDumpOpt_Function)
      {
         if(IRDumpOpt_Headers) IRDumpHeader(out, "Functions");
         for(auto const &fn : prog.rangeFunction()) IRDump_Function(out, fn);
      }

      // Imports
      if(IRDumpOpt_Import)
      {
         if(IRDumpOpt_Headers) IRDumpHeader(out, "Imports");
         for(auto const &imp : prog.rangeImport()) IRDump_Import(out, imp);
      }

      // Objects
      if(IRDumpOpt_Object)
      {
         if(IRDumpOpt_Headers) IRDumpHeader(out, "Objects");
         for(auto const &obj : prog.rangeObject()) IRDump_Object(out, obj);
      }

      // Spaces
      if(IRDumpOpt_Space)
      {
         if(IRDumpOpt_Headers) IRDumpHeader(out, "Address Spaces");
         for(auto const &sp : prog.rangeSpaceGblArs()) IRDump_Space(out, sp);
         for(auto const &sp : prog.rangeSpaceMapArs()) IRDump_Space(out, sp);
         for(auto const &sp : prog.rangeSpaceWldArs()) IRDump_Space(out, sp);
      }

      // StrEnts
      if(IRDumpOpt_StrEnt)
      {
         if(IRDumpOpt_Headers) IRDumpHeader(out, "String Table Entries");
         for(auto const &str : prog.rangeStrEnt()) IRDump_StrEnt(out, str);
      }

      // File footer.
      if(IRDumpOpt_Headers)
      {
         out << "\n;; EOF\n\n";
      }
   }
}

// EOF

