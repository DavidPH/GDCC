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

#include "IRDump/Put.hpp"

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

   &GDCC::IRDump::OptFunction
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

   &GDCC::IRDump::OptGlyph
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

   &GDCC::IRDump::OptHeaders
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

   &GDCC::IRDump::OptImport
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

   &GDCC::IRDump::OptObject
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

   &GDCC::IRDump::OptSpace
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

   &GDCC::IRDump::OptStatistics
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

   &GDCC::IRDump::OptStrEnt
};


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// PutHeader
//
static void PutHeader(std::ostream &out, char const *str)
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
      // PutProgram
      //
      void PutProgram(std::ostream &out, IR::Program const &prog)
      {
         // File header.
         if(OptHeaders)
         {
            out << ";;"; for(int i = 77; i--;) out << '-'; out << '\n';
            out << ";;\n";
            out << ";; IR dump from gdcc-irdump " << Core::GetOptionList().version << '\n';
            out << ";;\n";
            out << ";;"; for(int i = 77; i--;) out << '-'; out << '\n';
         }

         // Overall statistics.
         if(OptStatistics)
         {
            if(OptHeaders) out << ";;\n";

            out << ";; Functions: "               << prog.sizeFunction()    << '\n';
            out << ";; Glyphs: "                  << prog.sizeGlyphData()   << '\n';
            out << ";; Imports: "                 << prog.sizeImport()      << '\n';
            out << ";; Objects: "                 << prog.sizeObject()      << '\n';
            out << ";; Address Spaces (GblArs): " << prog.sizeSpaceGblArs() << '\n';
            out << ";; Address Spaces (MapArs): " << prog.sizeSpaceMapArs() << '\n';
            out << ";; Address Spaces (WldArs): " << prog.sizeSpaceWldArs() << '\n';
            out << ";; String Table Entries: "    << prog.sizeStrEnt()      << '\n';

            if(OptHeaders)
            {
               out << ";;\n";
               out << ";;"; for(int i = 77; i--;) out << '-'; out << '\n';
            }
         }

         // Functions
         if(OptFunction)
         {
            if(OptHeaders) PutHeader(out, "Functions");
            for(auto const &fn : prog.rangeFunction()) PutFunction(out, fn);
         }

         // Glyphs
         if(OptGlyph)
         {
            if(OptHeaders) PutHeader(out, "Glyphs");
            for(auto const &fn : prog.rangeGlyphData()) PutGlyphData(out, fn);
         }

         // Imports
         if(OptImport)
         {
            if(OptHeaders) PutHeader(out, "Imports");
            for(auto const &imp : prog.rangeImport()) PutImport(out, imp);
         }

         // Objects
         if(OptObject)
         {
            if(OptHeaders) PutHeader(out, "Objects");
            for(auto const &obj : prog.rangeObject()) PutObject(out, obj);
         }

         // Spaces
         if(OptSpace)
         {
            if(OptHeaders) PutHeader(out, "Address Spaces");
            for(auto const &sp : prog.rangeSpaceGblArs()) PutSpace(out, sp);
            for(auto const &sp : prog.rangeSpaceMapArs()) PutSpace(out, sp);
            for(auto const &sp : prog.rangeSpaceWldArs()) PutSpace(out, sp);
         }

         // StrEnts
         if(OptStrEnt)
         {
            if(OptHeaders) PutHeader(out, "String Table Entries");
            for(auto const &str : prog.rangeStrEnt()) PutStrEnt(out, str);
         }

         // File footer.
         if(OptHeaders)
         {
            out << "\n;; EOF\n\n";
         }
      }
   }
}

// EOF

