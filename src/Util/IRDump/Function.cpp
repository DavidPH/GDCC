//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// IR::Function dump/disassembly functions.
//
//-----------------------------------------------------------------------------

#include "../IRDump.hpp"

#include "GDCC/IR/Function.hpp"
#include "GDCC/IR/Linkage.hpp"

#include "Option/Option.hpp"


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace Option
{
   //
   // --dump-block
   //
   OptionDPtr<bool> DumpBlock{'\0', "dump-block", "output",
      "Dump IR::Block objects.", nullptr, &Util::IRDumpOpt_Block};

   //
   // --dump-labels
   //
   OptionDPtr<bool> DumpLabels{'\0', "dump-labels", "output",
      "Dump labels of statements.", nullptr, &Util::IRDumpOpt_Labels};

   //
   // --dump-origin
   //
   OptionDPtr<bool> DumpOrigin{'\0', "dump-origin", "output",
      "Dump origin of statements.", nullptr, &Util::IRDumpOpt_Origin};
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Util
{
   //
   // IRDump_Function
   //
   void IRDump_Function(std::ostream &out, GDCC::IR::Function const &fn)
   {
      out << "\nFunction "; IRDumpString(out, fn.glyph);

      if(fn.alloc)    out << " \\\n   alloc="    << fn.alloc;
                      out << " \\\n   ctype="    << fn.ctype;
      if(fn.defin)    out << " \\\n   defin="    << fn.defin;
      if(fn.label)   {out << " \\\n   label=";      IRDumpString(out, fn.label);}
                      out << " \\\n   linka="    << fn.linka;
      if(fn.localArs) out << " \\\n   localArs=" << fn.localArs;
      if(fn.localReg) out << " \\\n   localReg=" << fn.localReg;
      if(fn.param)    out << " \\\n   param="    << fn.param;
      if(fn.retrn)    out << " \\\n   retrn="    << fn.retrn;
      if(fn.sflagNet) out << " \\\n   sflagNet=" << fn.sflagNet;
      if(fn.sflagClS) out << " \\\n   sflagClS=" << fn.sflagClS;
                      out << " \\\n   stype="    << fn.stype;
      if(fn.valueInt) out << " \\\n   valueInt=" << fn.valueInt;
      if(fn.valueStr){out << " \\\n   valueStr=";   IRDumpString(out, fn.valueStr);}

      if(IRDumpOpt_Block && !fn.block.empty())
      {
         out << " \\\n   block\n{\n";

         GDCC::Origin pos{GDCC::STRNULL, 0};

         for(auto const &stmnt : fn.block)
         {
            // Dump origin, if different from previous.
            if(IRDumpOpt_Origin && stmnt.pos != pos)
            {
               if(pos.file) out << '\n';
               out << "   ; " << (pos = stmnt.pos) << '\n';
            }

            // Dump labels.
            if(IRDumpOpt_Labels) for(auto const &lab : stmnt.labs)
            {
               IRDumpString(out, lab);
               out << '\n';
            }

            // Dump instruction and arguments.
            out << "   " << stmnt.code;

            for(auto const &arg : stmnt.args)
            {
               out << ", ";
               IRDump_Arg(out, arg);
            }

            out << '\n';
         }

         out << "}";
      }

      out << '\n';
   }
}

// EOF

