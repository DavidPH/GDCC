//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Deleters.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__Deleter_H__
#define GDCC__Core__Deleter_H__

#include "../Core/Types.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Core
{
   //
   // ConditionalDeleter
   //
   template<typename T>
   class ConditionalDeleter
   {
   public:
      ConditionalDeleter(bool free_) : free{free_} {}

      void operator () (T *p) const {if(free) delete p;}

   private:
      bool free;
   };

   //
   // ConditionalDeleter<T[]>
   //
   template<typename T>
   class ConditionalDeleter<T[]>
   {
   public:
      ConditionalDeleter(bool free_) : free{free_} {}

      void operator () (T *p) const {if(free) delete[] p;}

   private:
      bool free;
   };
}

#endif//GDCC__Core__Deleter_H__

