//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Member-comparing classes.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__MemCmp_H__
#define GDCC__Core__MemCmp_H__

#include <functional>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      //
      // LessMem
      //
      template<typename T, typename KT, KT T::*P, typename Cmp = std::less<KT>>
      struct LessMem : private Cmp
      {
         using Cmp::Cmp;

         constexpr bool operator () (T const &l, T const &r) const
            {return Cmp::operator()(l.*P, r.*P);}
         constexpr bool operator () (T const *l, T const *r) const
            {return Cmp::operator()(l->*P, r->*P);}
      };
   }
}

#endif//GDCC__Core__MemCmp_H__

