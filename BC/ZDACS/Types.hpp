//-----------------------------------------------------------------------------
//
// Copyright (C) 2018-2024 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Common typedefs and class forward declarations.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__BC__ZDACS__Types_H__
#define GDCC__BC__ZDACS__Types_H__

#include "../../BC/Types.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::BC::ZDACS
{
   enum class Code;

   template<typename ElemT>
   class ChunkBase;
   class ChunkAIMP;
   class ChunkAINI;
   class ChunkARAY;
   class ChunkASTR;
   class ChunkATAG;
   class ChunkCODE;
   class ChunkFARY;
   class ChunkFNAM;
   class ChunkFUNC;
   class ChunkJUMP;
   class ChunkLOAD;
   class ChunkMEXP;
   class ChunkMIMP;
   class ChunkMINI;
   class ChunkMSTR;
   class ChunkSARY;
   class ChunkSFLG;
   class ChunkSNAM;
   class ChunkSPTR;
   class ChunkSTRL;
   class ChunkSVCT;
   class ElemArg;
   class ElemAIMP;
   class ElemAINI;
   class ElemARAY;
   class ElemASTR;
   class ElemATAG;
   class ElemCODE;
   class ElemFARY;
   class ElemFNAM;
   class ElemFUNC;
   class ElemJUMP;
   class ElemLOAD;
   class ElemMEXP;
   class ElemMIMP;
   class ElemMINI;
   class ElemMSTR;
   class ElemSARY;
   class ElemSFLG;
   class ElemSNAM;
   class ElemSPTR;
   class ElemSTRL;
   class ElemSVCT;
   class Info;
   class Module;

   using ElemArgs = Core::Array<ElemArg>;
}

#endif//GDCC__BC__ZDACS__Types_H__

