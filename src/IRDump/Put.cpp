//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation dump/disassembly functions.
//
//-----------------------------------------------------------------------------

#include "IRDump/Put.hpp"

#include "Core/Option.hpp"

#include "IR/Glyph.hpp"
#include "IR/Import.hpp"
#include "IR/Linkage.hpp"
#include "IR/Object.hpp"
#include "IR/Space.hpp"
#include "IR/StrEnt.hpp"


//----------------------------------------------------------------------------|
// Options                                                                    |
//

//
// -a, --dump-all
//
static GDCC::Option::Function DumpAll
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("dump-all").setName('a')
      .setGroup("output")
      .setDescS("Enables all --dump-* options."),

   [](GDCC::Option::Base *, GDCC::Option::Args const &args) -> std::size_t
   {
      GDCC::IRDump::OptBlock      = !args.optFalse;
      GDCC::IRDump::OptFunction   = !args.optFalse;
      GDCC::IRDump::OptGlyph      = !args.optFalse;
      GDCC::IRDump::OptHeaders    = !args.optFalse;
      GDCC::IRDump::OptImport     = !args.optFalse;
      GDCC::IRDump::OptLabels     = !args.optFalse;
      GDCC::IRDump::OptObject     = !args.optFalse;
      GDCC::IRDump::OptOrigin     = !args.optFalse;
      GDCC::IRDump::OptSpace      = !args.optFalse;
      GDCC::IRDump::OptStatistics = !args.optFalse;
      GDCC::IRDump::OptStrEnt     = !args.optFalse;

      return 0;
   }
};


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace IRDump
   {
      bool OptBlock      = false;
      bool OptExpName    = false;
      bool OptFunction   = false;
      bool OptGlyph      = false;
      bool OptHeaders    = false;
      bool OptImport     = false;
      bool OptLabels     = false;
      bool OptObject     = false;
      bool OptOrigin     = false;
      bool OptSpace      = false;
      bool OptStatistics = false;
      bool OptStrEnt     = false;
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IRDump
   {
      //
      // PutAddrSpace
      //
      void PutAddrSpace(std::ostream &out, IR::AddrSpace const &space)
      {
         out << space.base << ' '; PutString(out, space.name);
      }

      //
      // PutGlyphData
      //
      void PutGlyphData(std::ostream &out, IR::GlyphData const &data)
      {
         out << "\nGlyph "; PutString(out, data.glyph);

         if(data.type) {out << " \\\n   type=";  PutType(out, data.type );}
         if(data.value){out << " \\\n   value="; PutExp (out, data.value);}

         out << '\n';
      }

      //
      // PutImport
      //
      void PutImport(std::ostream &out, IR::Import const &imp)
      {
         out << "\nImport "; PutString(out, imp.glyph);

         out << '\n';
      }

      //
      // PutObject
      //
      void PutObject(std::ostream &out, IR::Object const &obj)
      {
         out << "\nObject "; PutString(out, obj.glyph);

         if(obj.alias) out << " \\\n   alias=" << obj.alias;
         if(obj.alloc) out << " \\\n   alloc=" << obj.alloc;
         if(obj.defin) out << " \\\n   defin=" << obj.defin;
         if(obj.initi){out << " \\\n   initi=";   PutExp      (out, obj.initi);}
                       out << " \\\n   linka=" << obj.linka;
                      {out << " \\\n   space=";   PutAddrSpace(out, obj.space);}
         if(obj.value) out << " \\\n   value=" << obj.value;
         if(obj.words) out << " \\\n   words=" << obj.words;

         out << '\n';
      }

      //
      // PutSpace
      //
      void PutSpace(std::ostream &out, IR::Space const &sp)
      {
         out << "\nSpace "; PutAddrSpace(out, sp.space);

         if(sp.alloc) out << " \\\n   alloc=" << sp.alloc;
         if(sp.defin) out << " \\\n   defin=" << sp.defin;
                      out << " \\\n   linka=" << sp.linka;
         if(sp.value) out << " \\\n   value=" << sp.value;
         if(sp.words) out << " \\\n   words=" << sp.words;

         out << '\n';
      }

      //
      // PutStrEnt
      //
      void PutStrEnt(std::ostream &out, IR::StrEnt const &str)
      {
         out << "\nStrEnt "; PutString(out, str.glyph);

         if(str.alias)    out << " \\\n   alias="    << str.alias;
         if(str.alloc)    out << " \\\n   alloc="    << str.alloc;
         if(str.defin)    out << " \\\n   defin="    << str.defin;
         if(str.valueInt) out << " \\\n   valueInt=" << str.valueInt;
         if(str.valueStr){out << " \\\n   valueStr=";   PutString(out, str.valueStr);}

         out << '\n';
      }

      //
      // PutString
      //
      void PutString(std::ostream &out, Core::String str)
      {
         out << '"';

         for(char c : str) switch(c)
         {
         case '\"': out << "\\\""; break;
         case '\\': out << "\\\\"; break;
         case '\n': out << "\\n";  break;
         default:   out << c;      break;
         }

         out << '"';
      }
   }
}

// EOF

