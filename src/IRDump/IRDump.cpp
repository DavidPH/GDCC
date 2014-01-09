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

#include "IRDump/IRDump.hpp"

#include "Core/Option.hpp"

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
      GDCC::IRDump::IRDumpOpt_Block      = !args.optFalse;
      GDCC::IRDump::IRDumpOpt_Function   = !args.optFalse;
      GDCC::IRDump::IRDumpOpt_Headers    = !args.optFalse;
      GDCC::IRDump::IRDumpOpt_Import     = !args.optFalse;
      GDCC::IRDump::IRDumpOpt_Labels     = !args.optFalse;
      GDCC::IRDump::IRDumpOpt_Object     = !args.optFalse;
      GDCC::IRDump::IRDumpOpt_Origin     = !args.optFalse;
      GDCC::IRDump::IRDumpOpt_Space      = !args.optFalse;
      GDCC::IRDump::IRDumpOpt_Statistics = !args.optFalse;
      GDCC::IRDump::IRDumpOpt_StrEnt     = !args.optFalse;

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
      bool IRDumpOpt_Block      = false;
      bool IRDumpOpt_Function   = false;
      bool IRDumpOpt_Headers    = false;
      bool IRDumpOpt_Import     = false;
      bool IRDumpOpt_Labels     = false;
      bool IRDumpOpt_Object     = false;
      bool IRDumpOpt_Origin     = false;
      bool IRDumpOpt_Space      = false;
      bool IRDumpOpt_Statistics = false;
      bool IRDumpOpt_StrEnt     = false;
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
      // IRDump_AddrSpace
      //
      void IRDump_AddrSpace(std::ostream &out, IR::AddrSpace const &space)
      {
         out << space.base << ' '; IRDumpString(out, space.name);
      }

      //
      // IRDump_Import
      //
      void IRDump_Import(std::ostream &out, IR::Import const &imp)
      {
         out << "\nImport "; IRDumpString(out, imp.glyph);

         out << '\n';
      }

      //
      // IRDump_Object
      //
      void IRDump_Object(std::ostream &out, IR::Object const &obj)
      {
         out << "\nObject "; IRDumpString(out, obj.glyph);

         if(obj.alias) out << " \\\n   alias=" << obj.alias;
         if(obj.alloc) out << " \\\n   alloc=" << obj.alloc;
         if(obj.defin) out << " \\\n   defin=" << obj.defin;
         if(obj.initi){out << " \\\n   initi=";   IRDump_Exp      (out, obj.initi);}
                       out << " \\\n   linka=" << obj.linka;
                      {out << " \\\n   space=";   IRDump_AddrSpace(out, obj.space);}
         if(obj.value) out << " \\\n   value=" << obj.value;
         if(obj.words) out << " \\\n   words=" << obj.words;

         out << '\n';
      }

      //
      // IRDump_Space
      //
      void IRDump_Space(std::ostream &out, IR::Space const &sp)
      {
         out << "\nSpace "; IRDump_AddrSpace(out, sp.space);

         if(sp.alloc) out << " \\\n   alloc=" << sp.alloc;
         if(sp.defin) out << " \\\n   defin=" << sp.defin;
                      out << " \\\n   linka=" << sp.linka;
         if(sp.value) out << " \\\n   value=" << sp.value;
         if(sp.words) out << " \\\n   words=" << sp.words;

         out << '\n';
      }

      //
      // IRDump_StrEnt
      //
      void IRDump_StrEnt(std::ostream &out, IR::StrEnt const &str)
      {
         out << "\nStrEnt "; IRDumpString(out, str.glyph);

         if(str.alias)    out << " \\\n   alias="    << str.alias;
         if(str.alloc)    out << " \\\n   alloc="    << str.alloc;
         if(str.defin)    out << " \\\n   defin="    << str.defin;
         if(str.valueInt) out << " \\\n   valueInt=" << str.valueInt;
         if(str.valueStr){out << " \\\n   valueStr=";   IRDumpString(out, str.valueStr);}

         out << '\n';
      }

      //
      // IRDumpString
      //
      void IRDumpString(std::ostream &out, Core::String str)
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

