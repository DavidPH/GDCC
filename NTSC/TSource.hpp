//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// NTS tokenizing.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__NTSC__TSource_H__
#define GDCC__NTSC__TSource_H__

#include "../NTSC/Types.hpp"

#include "../Core/TokenSource.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::NTSC
{
   //
   // TSource
   //
   class TSource : public Core::TokenSource
   {
   public:
      TSource(std::istream &in_, Core::OriginSource &orig_) :
         in{in_}, orig{orig_}, tokHeader{false} {}


      static void SkipComment(std::istream &in);

   protected:
      virtual Core::Token v_getToken();

      std::istream       &in;
      Core::OriginSource &orig;

      bool tokHeader : 1;
   };
}

#endif//GDCC__NTSC__TSource_H__

