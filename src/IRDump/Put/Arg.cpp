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

#include "IRDump/Put.hpp"

#include "IR/Arg.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// PutArgPart ArgPtr1
//
static void PutArgPart(std::ostream &out, GDCC::IR::ArgPtr1 const &arg)
{
   GDCC::IRDump::PutArg(out, *arg.idx);
   out << ", ";
   GDCC::IRDump::PutExp(out, arg.off);
}

//
// PutArgPart ArgPtr2
//
static void PutArgPart(std::ostream &out, GDCC::IR::ArgPtr2 const &arg)
{
   GDCC::IRDump::PutArg(out, *arg.arr);
   out << ", ";
   GDCC::IRDump::PutArg(out, *arg.idx);
   out << ", ";
   GDCC::IRDump::PutExp(out, arg.off);
}

//
// PutArgPart Cpy
//
static void PutArgPart(std::ostream &out, GDCC::IR::Arg_Cpy const &arg)
{
   if(arg.value) out << arg.value;
}

//
// PutArgPart Lit
//
static void PutArgPart(std::ostream &out, GDCC::IR::Arg_Lit const &arg)
{
   GDCC::IRDump::PutExp(out, arg.value);
}

//
// PutArgPart Nul
//
static void PutArgPart(std::ostream &, GDCC::IR::Arg_Nul const &)
{
}

//
// PutArgPart Stk
//
static void PutArgPart(std::ostream &, GDCC::IR::Arg_Stk const &)
{
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IRDump
   {
      //
      // PutArg
      //
      void PutArg(std::ostream &out, IR::Arg const &arg)
      {
         out << arg.a << '(';

         switch(arg.a)
         {
            #define GDCC_IR_AddrList(name) case IR::ArgBase::name: \
               PutArgPart(out, arg.a##name); break;
            #include "IR/AddrList.hpp"
         }

         out << ')';
      }
   }
}

// EOF

