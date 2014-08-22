//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
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
   }
}

#endif//GDCC__Core__Deleter_H__

