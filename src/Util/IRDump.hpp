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

#ifndef Util__IRDump_H__
#define Util__IRDump_H__

#include <ostream>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      class AddrSpace;
      class Arg;
      class Exp;
      class Function;
      class Import;
      class Object;
      class Program;
      class Space;
      class Statement;
      class StrEnt;
      class Type;
      class Value;
   }

   class String;
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Util
{
   void IRDump_AddrSpace(std::ostream &out, GDCC::IR::AddrSpace const &space);

   void IRDump_Arg(std::ostream &out, GDCC::IR::Arg const &arg);

   void IRDump_Exp(std::ostream &out, GDCC::IR::Exp const *exp);

   void IRDump_Function(std::ostream &out, GDCC::IR::Function const &fn);

   void IRDump_Import(std::ostream &out, GDCC::IR::Import const &imp);

   void IRDump_Object(std::ostream &out, GDCC::IR::Object const &obj);

   void IRDump_Program(std::ostream &out, GDCC::IR::Program const &prog);

   void IRDump_Space(std::ostream &out, GDCC::IR::Space const &sp);

   void IRDump_StrEnt(std::ostream &out, GDCC::IR::StrEnt const &str);

   void IRDump_Type(std::ostream &out, GDCC::IR::Type const &type);

   void IRDump_Value(std::ostream &out, GDCC::IR::Value const &val);

   void IRDumpString(std::ostream &out, GDCC::String str);
}


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace Util
{
   extern bool IRDumpOpt_Block;
   extern bool IRDumpOpt_Function;
   extern bool IRDumpOpt_Headers;
   extern bool IRDumpOpt_Import;
   extern bool IRDumpOpt_Labels;
   extern bool IRDumpOpt_Object;
   extern bool IRDumpOpt_Origin;
   extern bool IRDumpOpt_Space;
   extern bool IRDumpOpt_Statistics;
   extern bool IRDumpOpt_StrEnt;
}

#endif//Util__IRDump_H__

