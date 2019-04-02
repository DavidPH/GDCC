//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
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

#include "IR/DJump.hpp"
#include "IR/Glyph.hpp"
#include "IR/Import.hpp"
#include "IR/Linkage.hpp"
#include "IR/Object.hpp"
#include "IR/Space.hpp"
#include "IR/StrEnt.hpp"


//----------------------------------------------------------------------------|
// Options                                                                    |
//

namespace GDCC::IRDump
{
   //
   // -a, --dump-all
   //
   static Option::Function DumpAll
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("dump-all").setName('a')
         .setGroup("output")
         .setDescS("Enables all --dump-* options."),

      [](Option::Base *, Option::Args const &args) -> std::size_t
      {
         DumpBlock      = !args.optFalse;
         DumpDJump      = !args.optFalse;
         DumpFunction   = !args.optFalse;
         DumpGlyph      = !args.optFalse;
         DumpHeaders    = !args.optFalse;
         DumpImport     = !args.optFalse;
         DumpLabels     = !args.optFalse;
         DumpObject     = !args.optFalse;
         DumpOrigin     = !args.optFalse;
         DumpSpace      = !args.optFalse;
         DumpStatistics = !args.optFalse;
         DumpStrEnt     = !args.optFalse;

         return 0;
      }
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IRDump
{
   //
   // PutAddrSpace
   //
   void PutAddrSpace(std::ostream &out, IR::AddrSpace const &space)
   {
      out << space.base << ' '; PutString(out, space.name);
   }

   //
   // PutDJump
   //
   void PutDJump(std::ostream &out, IR::DJump const &jump)
   {
      out << "\nDJump "; PutString(out, jump.glyph); out << "\n(";

      if(jump.alloc) out << "\n   alloc=" << jump.alloc;
      if(jump.defin) out << "\n   defin=" << jump.defin;
      if(jump.label){out << "\n   label=";   PutString(out, jump.label);}
      if(jump.value) out << "\n   value=" << jump.value;

      out << "\n)\n";
   }

   //
   // PutGlyphData
   //
   void PutGlyphData(std::ostream &out, IR::GlyphData const &data)
   {
      out << "\nGlyph "; PutString(out, data.glyph); out << "\n(\n";

      if(data.type) {out << "\n   type=";  PutType(out, data.type );}
      if(data.value){out << "\n   value="; PutExp (out, data.value);}

      out << "\n)\n";
   }

   //
   // PutImport
   //
   void PutImport(std::ostream &out, IR::Import const &imp)
   {
      out << "\nImport "; PutString(out, imp.glyph); out << "()\n";
   }

   //
   // PutObject
   //
   void PutObject(std::ostream &out, IR::Object const &obj)
   {
      out << "\nObject "; PutString(out, obj.glyph); out << "\n(";

      if(obj.alias) out << "\n   alias=" << obj.alias;
      if(obj.alloc) out << "\n   alloc=" << obj.alloc;
      if(obj.defin) out << "\n   defin=" << obj.defin;
      if(obj.initi){out << "\n   initi=";   PutExp      (out, obj.initi);}
                    out << "\n   linka=" << obj.linka;
                    out << "\n   space=";   PutAddrSpace(out, obj.space);
      if(obj.value) out << "\n   value=" << obj.value;
      if(obj.words) out << "\n   words=" << obj.words;

      out << "\n)\n";
   }

   //
   // PutSpace
   //
   void PutSpace(std::ostream &out, IR::Space const &sp)
   {
      out << "\nSpace "; PutAddrSpace(out, sp.space); out << "\n(";

      if(sp.alloc) out << "\n   alloc=" << sp.alloc;
      if(sp.defin) out << "\n   defin=" << sp.defin;
                   out << "\n   linka=" << sp.linka;
      if(sp.value) out << "\n   value=" << sp.value;
      if(sp.words) out << "\n   words=" << sp.words;

      out << "\n)\n";
   }

   //
   // PutStrEnt
   //
   void PutStrEnt(std::ostream &out, IR::StrEnt const &str)
   {
      out << "\nStrEnt "; PutString(out, str.glyph); out << "\n(";

      if(str.alias)    out << "\n   alias="    << str.alias;
      if(str.alloc)    out << "\n   alloc="    << str.alloc;
      if(str.defin)    out << "\n   defin="    << str.defin;
      if(str.multiDef) out << "\n   multiDef=" << str.multiDef;
      if(str.valueInt) out << "\n   valueInt=" << str.valueInt;
      if(str.valueStr){out << "\n   valueStr=";   PutString(out, str.valueStr);}

      out << "\n)\n";
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

// EOF

