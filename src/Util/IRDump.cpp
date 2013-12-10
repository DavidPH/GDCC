//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation dump/disassembly functions.
//
//-----------------------------------------------------------------------------

#include "IRDump.hpp"

#include "GDCC/IR/Import.hpp"
#include "GDCC/IR/Linkage.hpp"
#include "GDCC/IR/Object.hpp"
#include "GDCC/IR/Space.hpp"
#include "GDCC/IR/StrEnt.hpp"

#include "Option/Option.hpp"


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace Option
{
   //
   // -a, --dump-all
   //
   OptionCall DumpAll{'a', "dump-all", "output",
      "Enables all --dump-* options.", nullptr,
      [](strp, uint optf, uint, strv) -> uint
      {
         Util::IRDumpOpt_Block      = !(optf & OPTF_FALSE);
         Util::IRDumpOpt_Function   = !(optf & OPTF_FALSE);
         Util::IRDumpOpt_Headers    = !(optf & OPTF_FALSE);
         Util::IRDumpOpt_Import     = !(optf & OPTF_FALSE);
         Util::IRDumpOpt_Labels     = !(optf & OPTF_FALSE);
         Util::IRDumpOpt_Object     = !(optf & OPTF_FALSE);
         Util::IRDumpOpt_Origin     = !(optf & OPTF_FALSE);
         Util::IRDumpOpt_Space      = !(optf & OPTF_FALSE);
         Util::IRDumpOpt_Statistics = !(optf & OPTF_FALSE);
         Util::IRDumpOpt_StrEnt     = !(optf & OPTF_FALSE);

         return 0;
      }};
}

namespace Util
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


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Util
{
   //
   // IRDump_AddrSpace
   //
   void IRDump_AddrSpace(std::ostream &out, GDCC::IR::AddrSpace const &space)
   {
      out << space.base << ' '; IRDumpString(out, space.name);
   }

   //
   // IRDump_Import
   //
   void IRDump_Import(std::ostream &out, GDCC::IR::Import const &imp)
   {
      out << "\nImport "; IRDumpString(out, imp.glyph);

      out << '\n';
   }

   //
   // IRDump_Object
   //
   void IRDump_Object(std::ostream &out, GDCC::IR::Object const &obj)
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
   void IRDump_Space(std::ostream &out, GDCC::IR::Space const &sp)
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
   void IRDump_StrEnt(std::ostream &out, GDCC::IR::StrEnt const &str)
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
   void IRDumpString(std::ostream &out, GDCC::String str)
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

