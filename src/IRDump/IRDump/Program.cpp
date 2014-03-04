//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// IR::Program dump/disassembly functions.
//
//-----------------------------------------------------------------------------

#include "IRDump/IRDump.hpp"

#include "Core/Option.hpp"

#include "IR/Program.hpp"

#include "Option/Bool.hpp"


//----------------------------------------------------------------------------|
// Options                                                                    |
//

//
// --dump-function
//
static GDCC::Option::Bool DumpFunction
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("dump-function")
      .setGroup("output")
      .setDescS("Dump IR::Function objects."),

   &GDCC::IRDump::IRDumpOpt_Function
};

//
// --dump-glyph
//
static GDCC::Option::Bool DumpGlyph
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("dump-glyph")
      .setGroup("output")
      .setDescS("Dump IR::GlyphData objects."),

   &GDCC::IRDump::IRDumpOpt_Glyph
};

//
// --dump-headers
//
static GDCC::Option::Bool DumpHeaders
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("dump-headers")
      .setGroup("output")
      .setDescS("Include comment headers in output."),

   &GDCC::IRDump::IRDumpOpt_Headers
};

//
// --dump-import
//
static GDCC::Option::Bool DumpImport
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("dump-import")
      .setGroup("output")
      .setDescS("Dump IR::Import objects."),

   &GDCC::IRDump::IRDumpOpt_Import
};

//
// --dump-object
//
static GDCC::Option::Bool DumpObject
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("dump-object")
      .setGroup("output")
      .setDescS("Dump IR::Object objects."),

   &GDCC::IRDump::IRDumpOpt_Object
};

//
// --dump-space
//
static GDCC::Option::Bool DumpSpace
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("dump-space")
      .setGroup("output")
      .setDescS("Dump IR::Space objects."),

   &GDCC::IRDump::IRDumpOpt_Space
};

//
// --dump-statistics
//
static GDCC::Option::Bool DumpStatistics
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("dump-statistics")
      .setGroup("output")
      .setDescS("Dump overall statistics."),

   &GDCC::IRDump::IRDumpOpt_Statistics
};

//
// --dump-strent
//
static GDCC::Option::Bool DumpStrEnt
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("dump-strent")
      .setGroup("output")
      .setDescS("Dump IR::StrEnt objects."),

   &GDCC::IRDump::IRDumpOpt_StrEnt
};


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

namespace GDCC
{
   namespace IRDump
   {
      //
      // IRDump_Program
      //
      void IRDump_Program(std::ostream &out, IR::Program const &prog)
      {
         // File header.
         if(IRDumpOpt_Headers)
         {
            out << ";;"; for(int i = 77; i--;) out << '-'; out << '\n';
            out << ";;\n";
            out << ";; IR dump from gdcc-irdump " << Core::GetOptionList().version << '\n';
            out << ";;\n";
            out << ";;"; for(int i = 77; i--;) out << '-'; out << '\n';
         }

         // Overall statistics.
         if(IRDumpOpt_Statistics)
         {
            if(IRDumpOpt_Headers) out << ";;\n";

            out << ";; Functions: "               << prog.sizeFunction()    << '\n';
            out << ";; Glyphs: "                  << prog.sizeGlyphData()   << '\n';
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

         // Glyphs
         if(IRDumpOpt_Glyph)
         {
            if(IRDumpOpt_Headers) IRDumpHeader(out, "Glyphs");
            for(auto const &fn : prog.rangeGlyphData()) IRDump_GlyphData(out, fn);
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
}

// EOF

