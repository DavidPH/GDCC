//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
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

namespace GDCC::IRDump
{
   //
   // --dump-djump
   //
   Option::Bool DumpDJump
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("dump-djump")
         .setGroup("output")
         .setDescS("Dump IR::DJump objects."),

      false
   };

   //
   // --dump-function
   //
   Option::Bool DumpFunction
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("dump-function")
         .setGroup("output")
         .setDescS("Dump IR::Function objects."),

      false
   };

   //
   // --dump-glyph
   //
   Option::Bool DumpGlyph
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("dump-glyph")
         .setGroup("output")
         .setDescS("Dump IR::GlyphData objects."),

      false
   };

   //
   // --dump-headers
   //
   Option::Bool DumpHeaders
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("dump-headers")
         .setGroup("output")
         .setDescS("Include comment headers in output."),

      false
   };

   //
   // --dump-import
   //
   Option::Bool DumpImport
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("dump-import")
         .setGroup("output")
         .setDescS("Dump IR::Import objects."),

      false
   };

   //
   // --dump-object
   //
   Option::Bool DumpObject
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("dump-object")
         .setGroup("output")
         .setDescS("Dump IR::Object objects."),

      false
   };

   //
   // --dump-space
   //
   Option::Bool DumpSpace
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("dump-space")
         .setGroup("output")
         .setDescS("Dump IR::Space objects."),

      false
   };

   //
   // --dump-statistics
   //
   Option::Bool DumpStatistics
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("dump-statistics")
         .setGroup("output")
         .setDescS("Dump overall statistics."),

      false
   };

   //
   // --dump-strent
   //
   Option::Bool DumpStrEnt
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("dump-strent")
         .setGroup("output")
         .setDescS("Dump IR::StrEnt objects."),

      false
   };
}


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::IRDump
{
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
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IRDump
{
   //
   // PutProgram
   //
   void PutProgram(std::ostream &out, IR::Program const &prog)
   {
      // File header.
      if(DumpHeaders)
      {
         out << ";;"; for(int i = 77; i--;) out << '-'; out << '\n';
         out << ";;\n";
         out << ";; IR dump from gdcc-irdump " << Core::GetOptionList().version << '\n';
         out << ";;\n";
         out << ";;"; for(int i = 77; i--;) out << '-'; out << '\n';
      }

      // Overall statistics.
      if(DumpStatistics)
      {
         if(DumpHeaders) out << ";;\n";

         out << ";; DJumps: "                  << prog.sizeDJump()       << '\n';
         out << ";; Functions: "               << prog.sizeFunction()    << '\n';
         out << ";; Glyphs: "                  << prog.sizeGlyphData()   << '\n';
         out << ";; Imports: "                 << prog.sizeImport()      << '\n';
         out << ";; Objects: "                 << prog.sizeObject()      << '\n';
         out << ";; Address Spaces (GblArs): " << prog.sizeSpaceGblArs() << '\n';
         out << ";; Address Spaces (HubArs): " << prog.sizeSpaceHubArs() << '\n';
         out << ";; Address Spaces (LocArs): " << prog.sizeSpaceLocArs() << '\n';
         out << ";; Address Spaces (ModArs): " << prog.sizeSpaceModArs() << '\n';
         out << ";; String Table Entries: "    << prog.sizeStrEnt()      << '\n';

         if(DumpHeaders)
         {
            out << ";;\n";
            out << ";;"; for(int i = 77; i--;) out << '-'; out << '\n';
         }
      }

      // Functions
      if(DumpDJump)
      {
         if(DumpHeaders) PutHeader(out, "DJumps");
         for(auto const &jump : prog.rangeDJump()) PutDJump(out, jump);
      }

      // Functions
      if(DumpFunction)
      {
         if(DumpHeaders) PutHeader(out, "Functions");
         for(auto const &fn : prog.rangeFunction()) PutFunction(out, fn);
      }

      // Glyphs
      if(DumpGlyph)
      {
         if(DumpHeaders) PutHeader(out, "Glyphs");
         for(auto const &glyph : prog.rangeGlyphData()) PutGlyphData(out, glyph);
      }

      // Imports
      if(DumpImport)
      {
         if(DumpHeaders) PutHeader(out, "Imports");
         for(auto const &imp : prog.rangeImport()) PutImport(out, imp);
      }

      // Objects
      if(DumpObject)
      {
         if(DumpHeaders) PutHeader(out, "Objects");
         for(auto const &obj : prog.rangeObject()) PutObject(out, obj);
      }

      // Spaces
      if(DumpSpace)
      {
         if(DumpHeaders) PutHeader(out, "Address Spaces");
         for(auto const &sp : prog.rangeSpaceGblArs()) PutSpace(out, sp);
         for(auto const &sp : prog.rangeSpaceHubArs()) PutSpace(out, sp);
         for(auto const &sp : prog.rangeSpaceLocArs()) PutSpace(out, sp);
         for(auto const &sp : prog.rangeSpaceModArs()) PutSpace(out, sp);
      }

      // StrEnts
      if(DumpStrEnt)
      {
         if(DumpHeaders) PutHeader(out, "String Table Entries");
         for(auto const &str : prog.rangeStrEnt()) PutStrEnt(out, str);
      }

      // File footer.
      if(DumpHeaders)
      {
         out << "\n;; EOF\n\n";
      }
   }
}

// EOF

