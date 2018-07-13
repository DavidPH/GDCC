//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Template for holding off an IStream feature.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__FeatureHold_H__
#define GDCC__Core__FeatureHold_H__

#include "../Core/Types.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Core
{
   //
   // FeatureHold
   //
   template<typename T, void(T::*D)(), void(T::*E)()> class FeatureHold
   {
   public:
      FeatureHold(FeatureHold const &) = delete;
      FeatureHold(FeatureHold &&hold) : str{hold.str} {hold.str = nullptr;}
      explicit FeatureHold(T &str_) : str{&str_} {(str->*D)();}
      ~FeatureHold() {if(str) (str->*E)();}

   private:
      T *str;
   };
}

#endif//GDCC__Core__FeatureHold_H__

