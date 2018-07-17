//-----------------------------------------------------------------------------
//
// Copyright (C) 2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Common typedefs and class forward declarations.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CPP__Types_H__
#define GDCC__CPP__Types_H__

#include "../IR/Types.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CPP
{
   class ConcatTBuf;
   class ConditionDTBuf;
   class DefineDTBuf;
   class DefinedTBuf;
   class DirectiveTBuf;
   class ErrorDTBuf;
   class IStream;
   class IStreamHeader;
   class IdentiTBuf;
   class IncStream;
   class IncludeDTBuf;
   class IncludeLang;
   class LineDTBuf;
   class Macro;
   class MacroMap;
   class MacroTBuf;
   class PPStream;
   class PPTokenTBuf;
   class PragmaDTBuf;
   class PragmaData;
   class PragmaDataBase;
   class PragmaParserACS;
   class PragmaParserBase;
   class PragmaParserGDCC;
   class PragmaParserSTDC;
   template<typename... Parsers>
   class PragmaParserVA;
   class PragmaPushTBuf;
   class PragmaTBuf;
   class StringTBuf;
   class TStream;
   class UndefDTBuf;

   using PragmaParser = PragmaParserVA<
      PragmaParserACS,
      PragmaParserGDCC,
      PragmaParserSTDC>;
}

#endif//GDCC__BC__Types_H__

