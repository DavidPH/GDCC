//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// IR::Arg dump/disassembly functions.
//
//-----------------------------------------------------------------------------

#include "../IRDump.hpp"

#include "GDCC/IR/Arg.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// IRDump_ArgPart ArgPtr1
//
static void IRDump_ArgPart(std::ostream &out, GDCC::IR::ArgPtr1 const &arg)
{
   Util::IRDump_Arg(out, *arg.idx);
   out << ", ";
   Util::IRDump_Exp(out, arg.off);
}

//
// IRDump_ArgPart ArgPtr2
//
static void IRDump_ArgPart(std::ostream &out, GDCC::IR::ArgPtr2 const &arg)
{
   Util::IRDump_Arg(out, *arg.arr);
   out << ", ";
   Util::IRDump_Arg(out, *arg.idx);
   out << ", ";
   Util::IRDump_Exp(out, arg.off);
}

//
// IRDump_ArgPart Cpy
//
static void IRDump_ArgPart(std::ostream &out, GDCC::IR::Arg_Cpy const &arg)
{
   if(arg.value) out << arg.value;
}

//
// IRDump_ArgPart Lit
//
static void IRDump_ArgPart(std::ostream &out, GDCC::IR::Arg_Lit const &arg)
{
   Util::IRDump_Exp(out, arg.value);
}

//
// IRDump_ArgPart Nul
//
static void IRDump_ArgPart(std::ostream &, GDCC::IR::Arg_Nul const &)
{
}

//
// IRDump_ArgPart Stk
//
static void IRDump_ArgPart(std::ostream &, GDCC::IR::Arg_Stk const &)
{
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Util
{
   //
   // IRDump_Arg
   //
   void IRDump_Arg(std::ostream &out, GDCC::IR::Arg const &arg)
   {
      out << arg.a << '(';

      switch(arg.a)
      {
         #define GDCC_IR_AddrList(name) case GDCC::IR::ArgBase::name: \
            IRDump_ArgPart(out, arg.a##name); break;
         #include "GDCC/IR/AddrList.hpp"
      }

      out << ')';
   }
}

// EOF

