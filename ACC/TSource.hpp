//-----------------------------------------------------------------------------
//
// Copyright (C) 2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS tokenizing.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__ACC__TSource_H__
#define GDCC__ACC__TSource_H__

#include "../ACC/Types.hpp"

#include "../CPP/TSource.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::ACC
{
   //
   // TSource
   //
   class TSource : public CPP::TSource
   {
   public:
      using CPP::TSource::TSource;

   protected:
      virtual Core::Token v_getToken();
   };
}

#endif//GDCC__ACC__TSource_H__

