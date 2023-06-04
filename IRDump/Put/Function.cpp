//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2023 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// IR::Function dump/disassembly functions.
//
//-----------------------------------------------------------------------------

#include "IRDump/Put.hpp"

#include "Core/Option.hpp"

#include "IR/Function.hpp"
#include "IR/Linkage.hpp"

#include "Option/Bool.hpp"

#include "Target/CallType.hpp"


//----------------------------------------------------------------------------|
// Options                                                                    |
//

namespace GDCC::IRDump
{
   //
   // --dump-block
   //
   Option::Bool DumpBlock
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("dump-block")
         .setGroup("output")
         .setDescS("Dump IR::Block objects."),

      false
   };

   //
   // --dump-labels
   //
   Option::Bool DumpLabels
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("dump-labels")
         .setGroup("output")
         .setDescS("Dump labels of statements."),

      false
   };

   //
   // --dump-origin
   //
   Option::Bool DumpOrigin
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("dump-origin")
         .setGroup("output")
         .setDescS("Dump origin of statements."),

      false
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IRDump
{
   //
   // PutFunction
   //
   void PutFunction(std::ostream &out, IR::Function const &fn)
   {
      //
      // putLocalArr
      //
      auto putLocalArr = [](std::ostream &out,
         std::unordered_map<Core::FastU, Core::FastU> const &localArr)
      {
         for(auto const &itr : localArr)
            out << "\n   localArr[" << itr.first << "]=" << itr.second;
      };

      //
      // putSType
      //
      auto putSType = [](std::ostream &out, Core::Array<Core::String> const &stype)
      {
         out << "\n   stype=(";
         auto itr = stype.begin(), end = stype.end();
         if   (itr != end) PutString(out,        *itr++);
         while(itr != end) PutString(out << ' ', *itr++);
         out << ')';
      };

      out << "\nFunction "; PutString(out, fn.glyph); out << "\n(";

      if(fn.allocAut) out << "\n   allocAut=" << fn.allocAut;
      if(fn.alloc)    out << "\n   alloc="    << fn.alloc;
                      out << "\n   ctype="    << fn.ctype;
      if(fn.defin)    out << "\n   defin="    << fn.defin;
      if(fn.label)   {out << "\n   label=";      PutString(out, fn.label);}
                      out << "\n   linka="    << fn.linka;
      if(!fn.localArr.empty())  putLocalArr(out, fn.localArr);
      if(fn.localAut) out << "\n   localAut=" << fn.localAut;
      if(fn.localReg) out << "\n   localReg=" << fn.localReg;
      if(fn.param)    out << "\n   param="    << fn.param;
      if(fn.retrn)    out << "\n   retrn="    << fn.retrn;
      if(!fn.stype.empty())     putSType(out,    fn.stype);
      if(fn.valueInt) out << "\n   valueInt=" << fn.valueInt;
      if(fn.valueStr){out << "\n   valueStr=";   PutString(out, fn.valueStr);}

      if(DumpBlock && !fn.block.empty())
      {
         out << "\n   block\n   (\n";

         Core::Origin pos{Core::STRNULL, 0};

         for(auto const &stmnt : fn.block)
         {
            // Dump origin, if different from previous.
            if(DumpOrigin && stmnt.pos != pos)
            {
               if(pos.file) out << '\n';
               out << "      ; " << (pos = stmnt.pos) << '\n';
            }

            // Dump labels.
            if(DumpLabels) for(auto const &lab : stmnt.labs)
            {
               out << "   ";
               PutString(out, lab);
               out << '\n';
            }

            // Dump instruction and arguments.
            out << "      " << stmnt.code << '(';

            if(auto i = stmnt.args.begin(), e = stmnt.args.end(); i != e) for(;;)
            {
               PutArg(out, *i);
               if(++i == e) break;
               out << ' ';
            }

            out << ")\n";
         }

         out << "   )";
      }

      out << "\n)\n";
   }
}

// EOF

