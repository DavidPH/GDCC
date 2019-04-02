//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
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

#include "../Core/Types.hpp"

#include <functional>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Core
{
   //
   // LessMem
   //
   template<typename T, typename KT, KT T::*P, typename Cmp = std::less<KT>>
   class LessMem : private Cmp
   {
   public:
      using Cmp::Cmp;

      constexpr bool operator () (T const &l, T const &r) const
         {return Cmp::operator()(l.*P, r.*P);}
      constexpr bool operator () (T const *l, T const *r) const
         {return Cmp::operator()(l->*P, r->*P);}
   };
}

#endif//GDCC__Core__MemCmp_H__

