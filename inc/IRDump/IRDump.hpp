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

#ifndef GDCC__IRDump__IRDump_H__
#define GDCC__IRDump__IRDump_H__

#include <ostream>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      class String;
   }

   namespace IR
   {
      class AddrSpace;
      class Arg;
      class Exp;
      class Function;
      class GlyphData;
      class Import;
      class Object;
      class Program;
      class Space;
      class Statement;
      class StrEnt;
      class Type;
      class Value;
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IRDump
   {
      void IRDump_AddrSpace(std::ostream &out, IR::AddrSpace const &space);

      void IRDump_Arg(std::ostream &out, IR::Arg const &arg);

      void IRDump_Exp(std::ostream &out, IR::Exp const *exp);

      void IRDump_Function(std::ostream &out, IR::Function const &fn);

      void IRDump_GlyphData(std::ostream &out, IR::GlyphData const &data);

      void IRDump_Import(std::ostream &out, IR::Import const &imp);

      void IRDump_Object(std::ostream &out, IR::Object const &obj);

      void IRDump_Program(std::ostream &out, IR::Program const &prog);

      void IRDump_Space(std::ostream &out, IR::Space const &sp);

      void IRDump_StrEnt(std::ostream &out, IR::StrEnt const &str);

      void IRDump_Type(std::ostream &out, IR::Type const &type);

      void IRDump_Value(std::ostream &out, IR::Value const &val);

      void IRDumpString(std::ostream &out, Core::String str);
   }
}


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace IRDump
   {
      extern bool IRDumpOpt_Block;
      extern bool IRDumpOpt_Function;
      extern bool IRDumpOpt_Glyph;
      extern bool IRDumpOpt_Headers;
      extern bool IRDumpOpt_Import;
      extern bool IRDumpOpt_Labels;
      extern bool IRDumpOpt_Object;
      extern bool IRDumpOpt_Origin;
      extern bool IRDumpOpt_Space;
      extern bool IRDumpOpt_Statistics;
      extern bool IRDumpOpt_StrEnt;
   }
}

#endif//GDCC__IRDump__IRDump_H__

