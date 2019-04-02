//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
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

#include "../IRDump/Types.hpp"

#include "../Option/Bool.hpp"

#include <ostream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IRDump
{
   void PutAddrSpace(std::ostream &out, IR::AddrSpace const &space);

   void PutArg(std::ostream &out, IR::Arg const &arg);

   void PutDJump(std::ostream &out, IR::DJump const &jump);

   void PutExp(std::ostream &out, IR::Exp const *exp);

   void PutFunction(std::ostream &out, IR::Function const &fn);

   void PutGlyphData(std::ostream &out, IR::GlyphData const &data);

   void PutImport(std::ostream &out, IR::Import const &imp);

   void PutObject(std::ostream &out, IR::Object const &obj);

   void PutProgram(std::ostream &out, IR::Program const &prog);

   void PutSpace(std::ostream &out, IR::Space const &sp);

   void PutStrEnt(std::ostream &out, IR::StrEnt const &str);

   void PutType(std::ostream &out, IR::Type const &type);

   void PutValue(std::ostream &out, IR::Value const &val);

   void PutString(std::ostream &out, Core::String str);

   #define GDCC_IR_TypeList(t) \
      void PutType_##t(std::ostream &out, IR::Type_##t const &type); \
      void PutValue_##t(std::ostream &out, IR::Value_##t const &val);
   #include "../IR/TypeList.hpp"
}


//----------------------------------------------------------------------------|
// Extern Objects                                                           |
//

namespace GDCC::IRDump
{
   extern Option::Bool DumpBlock;
   extern Option::Bool DumpDJump;
   extern Option::Bool DumpExpName;
   extern Option::Bool DumpFunction;
   extern Option::Bool DumpGlyph;
   extern Option::Bool DumpHeaders;
   extern Option::Bool DumpImport;
   extern Option::Bool DumpLabels;
   extern Option::Bool DumpObject;
   extern Option::Bool DumpOrigin;
   extern Option::Bool DumpSpace;
   extern Option::Bool DumpStatistics;
   extern Option::Bool DumpStrEnt;
}

#endif//GDCC__IRDump__IRDump_H__

