//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Linked list handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__List_H__
#define GDCC__Core__List_H__

#include <iterator>
#include <type_traits>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      template<typename T, typename std::remove_cv<T>::type *T::*P = &T::prev,
         typename std::remove_cv<T>::type *T::*N = &T::next>
      class ListItr
      {
      public:
         ListItr() = default;
         explicit ListItr(T *p_) : p{p_} {}

         explicit operator bool () const {return p;}
         explicit operator T * () const {return p;}

         T *operator -> () const {return p;}
         T &operator * () const {return *p;}

         ListItr operator ++ (int) {auto i = *this; p = p->*N; return i;}
         ListItr operator -- (int) {auto i = *this; p = p->*P; return i;}

         ListItr &operator ++ () {p = p->*N; return *this;}
         ListItr &operator -- () {p = p->*P; return *this;}

         bool operator == (ListItr const &i) {return p == i.p;}
         bool operator != (ListItr const &i) {return p != i.p;}

      private:
         T *p;
      };

      //
      // ListUtil
      //
      template<typename T, typename std::remove_cv<T>::type *T::*P = &T::prev,
         typename std::remove_cv<T>::type *T::*N = &T::next>
      class ListUtil
      {
      public:
         using Itr = ListItr<T, P, N>;
         using CItr = ListItr<T const, P, N>;


         //
         // Insert
         //
         static void Insert(T *node, T *next)
         {
            node->*N = next;
            node->*P = next->*P;

            node->*N->*P = node;
            node->*P->*N = node;
         }

         //
         // Relink
         //
         static void Relink(T *node, T *next)
         {
            (node->*N)->*P = node->*P;
            (node->*P)->*N = node->*N;

            node->*N = next;
            node->*P = next->*P;

            node->*N->*P = node;
            node->*P->*N = node;
         }

         //
         // Unlink
         //
         static void Unlink(T *node)
         {
            (node->*N)->*P = node->*P;
            (node->*P)->*N = node->*N;

            node->*N = node;
            node->*P = node;
         }
      };
   }
}

namespace std
{
   //
   // iterator_traits<::GDCC::Core::ListItr>
   //
   template<typename T, typename remove_cv<T>::type *T::*P,
      typename remove_cv<T>::type *T::*N>
   class iterator_traits<::GDCC::Core::ListItr<T, P, N>>
   {
   public:
      using difference_type   = ptrdiff_t;
      using value_type        = typename remove_cv<T>::type;
      using pointer           = T *;
      using reference         = T &;
      using iterator_category = bidirectional_iterator_tag;
   };
}

#endif//GDCC__Core__List_H__

