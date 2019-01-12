//-----------------------------------------------------------------------------
//
// Copyright (C) 2018-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Common typedefs and class forward declarations.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__ACC__Types_H__
#define GDCC__ACC__Types_H__

#include "../CC/Types.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::ACC
{
   class CreateTransDecl;
   class CreateTransProp;
   class CreateTransPropName;
   class DefineDTBuf;
   class Factory;
   class IgnoreDTBuf;
   class ImportDTBuf;
   class ImportStream;
   class IncStream;
   class IncludeDTBuf;
   class LibraryDTBuf;
   class MacroMap;
   class PPStream;
   class PPTokenTBuf;
   class Parser;
   class PragmaData;
   class PragmaParserBare;
   class PrintDecl;
   class PrintProp;
   class Scope_Global;
   class TSource;
   class TStream;

   using PragmaParser = CPP::PragmaParserVA<
      PragmaParserBare>;
}

#endif//GDCC__ACC__Types_H__

