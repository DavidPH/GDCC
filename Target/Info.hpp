//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2025 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Target information/selection.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Target__Info_H__
#define GDCC__Target__Info_H__

#include "../Target/Types.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Target
{
   //
   // Engine
   //
   enum class Engine
   {
      None,
      Doominati,
      Eternity,
      Hexen,
      ZDoom,
      Zandronum,
   };

   //
   // Format
   //
   enum class Format
   {
      None,
      ACS0,
      ACSE,
      DGE_NTS,
   };
}


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

namespace GDCC::Target
{
   extern Engine EngineCur;
   extern Format FormatCur;
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Target
{
   unsigned GetByteBitsI();

   // How many extra auto words does this type of call require?
   unsigned GetCallAutoAdd(CallType call);

   unsigned GetWordAlign();
   unsigned GetWordBits();
   unsigned GetWordBytes();
   unsigned GetWordPoint();
   unsigned GetWordShift();

   bool IsAddrFirst(AddrBase addr);

   // Does this type of call require explicit auto pointer propagation?
   bool IsCallAutoProp(CallType call);

   // Do extra arguments to this type of call require variadic handling?
   bool IsCallVaria(CallType call);

   bool IsDelayFunction();

   bool IsFamily_ZDACS();

   // Does null have a zero representation?
   bool IsZeroNull_DJump();
   bool IsZeroNull_Funct(CallType call);
   bool IsZeroNull_Point(AddrBase addr);
   bool IsZeroNull_StrEn();
}

#endif//GDCC__Target__Info_H__

