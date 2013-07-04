//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Linked list handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__List_H__
#define GDCC__List_H__


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   //
   // ListItr
   //
   template<typename T, T *T::*P, T *T::*N> class ListItr
   {
   public:
      ListItr() = default;
      explicit ListItr(T *p_) : p{p_} {}

      explicit operator bool () const {return p;}
      explicit operator T * () const {return p;}

      T *operator -> () const {return p;}
      T &operator * () const {return *p;}

      ListItr<T, P, N> operator ++ (int) {auto i = *this; p = p->*N; return i;}
      ListItr<T, P, N> operator -- (int) {auto i = *this; p = p->*P; return i;}

      ListItr<T, P, N> &operator ++ () {p = p->*N; return *this;}
      ListItr<T, P, N> &operator -- () {p = p->*P; return *this;}

   private:
      T *p;
   };
}

#endif//GDCC__List_H__

