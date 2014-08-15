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
      struct Type_Empty;
      struct Type_Fixed;
      struct Type_Float;
      struct Type_Funct;
      struct Type_Multi;
      struct Type_Point;
      struct Type_StrEn;
      class Value;
      struct Value_Empty;
      struct Value_Fixed;
      struct Value_Float;
      struct Value_Funct;
      struct Value_Multi;
      struct Value_Point;
      struct Value_StrEn;
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IRDump
   {
      void PutAddrSpace(std::ostream &out, IR::AddrSpace const &space);

      void PutArg(std::ostream &out, IR::Arg const &arg);

      void PutExp(std::ostream &out, IR::Exp const *exp);

      void PutFunction(std::ostream &out, IR::Function const &fn);

      void PutGlyphData(std::ostream &out, IR::GlyphData const &data);

      void PutImport(std::ostream &out, IR::Import const &imp);

      void PutObject(std::ostream &out, IR::Object const &obj);

      void PutProgram(std::ostream &out, IR::Program const &prog);

      void PutSpace(std::ostream &out, IR::Space const &sp);

      void PutStrEnt(std::ostream &out, IR::StrEnt const &str);

      void PutType(std::ostream &out, IR::Type const &type);

      void PutType_Empty(std::ostream &out, IR::Type_Empty const &type);
      void PutType_Fixed(std::ostream &out, IR::Type_Fixed const &type);
      void PutType_Float(std::ostream &out, IR::Type_Float const &type);
      void PutType_Funct(std::ostream &out, IR::Type_Funct const &type);
      void PutType_Multi(std::ostream &out, IR::Type_Multi const &type);
      void PutType_Point(std::ostream &out, IR::Type_Point const &type);
      void PutType_StrEn(std::ostream &out, IR::Type_StrEn const &type);

      void PutValue(std::ostream &out, IR::Value const &val);

      void PutValue_Empty(std::ostream &out, IR::Value_Empty const &val);
      void PutValue_Fixed(std::ostream &out, IR::Value_Fixed const &val);
      void PutValue_Float(std::ostream &out, IR::Value_Float const &val);
      void PutValue_Funct(std::ostream &out, IR::Value_Funct const &val);
      void PutValue_Multi(std::ostream &out, IR::Value_Multi const &val);
      void PutValue_Point(std::ostream &out, IR::Value_Point const &val);
      void PutValue_StrEn(std::ostream &out, IR::Value_StrEn const &val);

      void PutString(std::ostream &out, Core::String str);
   }
}


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace IRDump
   {
      extern bool OptBlock;
      extern bool OptExpName;
      extern bool OptFunction;
      extern bool OptGlyph;
      extern bool OptHeaders;
      extern bool OptImport;
      extern bool OptLabels;
      extern bool OptObject;
      extern bool OptOrigin;
      extern bool OptSpace;
      extern bool OptStatistics;
      extern bool OptStrEnt;
   }
}

#endif//GDCC__IRDump__IRDump_H__

