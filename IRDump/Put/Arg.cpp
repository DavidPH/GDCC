//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
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

namespace GDCC::IRDump
{
   //
   // PutArgPart ArgPtr1
   //
   static void PutArgPart(std::ostream &out, IR::ArgPtr1 const &arg)
   {
      PutArg(out, *arg.idx);
      if(arg.off)
         out << ' ' << arg.off;
   }

   //
   // PutArgPart ArgPtr2
   //
   static void PutArgPart(std::ostream &out, IR::ArgPtr2 const &arg)
   {
      PutArg(out, *arg.arr);
      out << ' ';
      PutArg(out, *arg.idx);
      if(arg.off)
         out << ' ' << arg.off;
   }

   //
   // PutArgPart Cpy
   //
   static void PutArgPart(std::ostream &out, IR::Arg_Cpy const &arg)
   {
      if(arg.value) out << arg.value;
   }

   //
   // PutArgPart Lit
   //
   static void PutArgPart(std::ostream &out, IR::Arg_Lit const &arg)
   {
      PutExp(out, arg.value);
      if(arg.off)
         out << ' ' << arg.off;
   }

   //
   // PutArgPart Nul
   //
   static void PutArgPart(std::ostream &, IR::Arg_Nul const &)
   {
   }

   //
   // PutArgPart Stk
   //
   static void PutArgPart(std::ostream &, IR::Arg_Stk const &)
   {
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IRDump
{
   //
   // PutArg
   //
   void PutArg(std::ostream &out, IR::Arg const &arg)
   {
      out << arg.a << ' ' << arg.getSize() << '(';

      switch(arg.a)
      {
         #define GDCC_Target_AddrList(name) case IR::ArgBase::name: \
            PutArgPart(out, arg.a##name); break;
         #include "Target/AddrList.hpp"
      }

      out << ')';
   }
}

// EOF

