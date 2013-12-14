//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Comparison templates.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__Compare_H__
#define GDCC__Core__Compare_H__


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Core
   {
      template<typename ItrL, typename ItrR>
      bool CompareGE(ItrL li, ItrL le, ItrR ri, ItrR re);

      template<typename ItrL, typename ItrR>
      bool CompareGT(ItrL li, ItrL le, ItrR ri, ItrR re);

      template<typename ItrL, typename ItrR>
      bool CompareLE(ItrL li, ItrL le, ItrR ri, ItrR re);

      template<typename ItrL, typename ItrR>
      bool CompareLT(ItrL li, ItrL le, ItrR ri, ItrR re);
   }
}

namespace GDCC
{
   namespace Core
   {
      //
      // CompareGE
      //
      template<typename ItrL, typename ItrR>
      bool CompareGE(ItrL li, ItrL le, ItrR ri, ItrR re)
      {
         for(;; ++li, ++ri)
         {
            if(ri == re) return true;
            if(li == le) return false;

            if(*li != *ri) return *li > *ri;
         }
      }

      //
      // CompareGT
      //
      template<typename ItrL, typename ItrR>
      bool CompareGT(ItrL li, ItrL le, ItrR ri, ItrR re)
      {
         for(;; ++li, ++ri)
         {
            if(li == le) return false;
            if(ri == re) return true;

            if(*li != *ri) return *li > *ri;
         }
      }

      //
      // CompareLE
      //
      template<typename ItrL, typename ItrR>
      bool CompareLE(ItrL li, ItrL le, ItrR ri, ItrR re)
      {
         for(;; ++li, ++ri)
         {
            if(li == le) return true;
            if(ri == re) return false;

            if(*li != *ri) return *li < *ri;
         }
      }

      //
      // CompareLT
      //
      // Like std::lexicographical_compare, but uses != for O(n) instead of O(2n).
      //
      template<typename ItrL, typename ItrR>
      bool CompareLT(ItrL li, ItrL le, ItrR ri, ItrR re)
      {
         for(;; ++li, ++ri)
         {
            if(ri == re) return false;
            if(li == le) return true;

            if(*li != *ri) return *li < *ri;
         }
      }
   }
}

#endif//GDCC__Core__Compare_H__

