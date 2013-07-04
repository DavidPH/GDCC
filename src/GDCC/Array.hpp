//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Non-resizing array.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Array_H__
#define GDCC__Array_H__

#include "Compare.hpp"

#include <algorithm>
#include <iterator>
#include <memory>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   //
   // Array
   //
   template<typename T>
   class Array
   {
   public:
      using value_type             = T;
      using const_iterator         = value_type const *;
      using const_pointer          = value_type const *;
      using const_reference        = value_type const &;
      using const_reverse_iterator = std::reverse_iterator<const_iterator>;
      using difference_type        = std::ptrdiff_t;
      using iterator               = value_type *;
      using pointer                = value_type *;
      using reference              = value_type &;
      using reverse_iterator       = std::reverse_iterator<iterator>;
      using size_type              = std::size_t;


      Array() : p{nullptr}, s{0} {}
      Array(Array const &a) : p{Cpy(a.p, a.s)}, s{a.s} {}
      Array(Array &&a) : p{a.p}, s{a.s} {a.p = nullptr; a.s = 0;}
      explicit Array(size_type s_) : p{New(s_)}, s{s_} {}
      ~Array() {Del(p, s);}

      template<typename T2>
      Array(T2 const *p_, size_type s_) : p{Cpy(p_, s_)}, s{s_} {}

      // operator Array[]
            reference operator [] (size_type i)       {return p[i];}
      const_reference operator [] (size_type i) const {return p[i];}

      // copy assignment
      Array &operator = (Array const &a)
         {pointer n = Cpy(a.p, a.s); Del(p, s); p = n; s = a.s; return *this;}

      // move assignment
      Array &operator = (Array &&a) {swap(a); return *this;}

      // at
            reference at(size_type i)
         {if(i < s) return p[i]; throw std::out_of_range("GDCC::Array::at");}
      const_reference at(size_type i) const
         {if(i < s) return p[i]; throw std::out_of_range("GDCC::Array::at");}

      // back
            reference back()       {return p[s - 1];}
      const_reference back() const {return p[s - 1];}

      // begin
            iterator begin()       {return p;}
      const_iterator begin() const {return p;}

      // cbegin
      const_iterator cbegin() const {return p;}

      // cend
      const_iterator cend() const {return p + s;}

      // clear
      void clear() {Del(p, s); p = nullptr; s = 0;}

      // crbegin
      const_reverse_iterator crbegin() const
         {return static_cast<const_reverse_iterator>(p + s);}

      // crend
      const_reverse_iterator crend() const
         {return static_cast<const_reverse_iterator>(p);}

      // data
            pointer data()       {return p;}
      const_pointer data() const {return p;}

      // empty
      bool empty() const {return s != 0;}

      // end
            iterator end()       {return p + s;}
      const_iterator end() const {return p + s;}

      // fill
      void fill(const_reference v) {for(auto &i : *this) i = v;}

      // front
            reference front()       {return p[0];}
      const_reference front() const {return p[0];}

      // max_size
      size_type max_size() const {return std::numeric_limits<size_type>::max();}

      // rbegin
            reverse_iterator rbegin()
         {return static_cast<      reverse_iterator>(p + s);}
      const_reverse_iterator rbegin() const
         {return static_cast<const_reverse_iterator>(p + s);}

      // rend
            reverse_iterator rend()
         {return static_cast<      reverse_iterator>(p);}
      const_reverse_iterator rend() const
         {return static_cast<const_reverse_iterator>(p);}

      // size
      size_type size() const {return s;}

      // swap
      void swap(Array  &a) {std::swap(p, a.p); std::swap(s, a.s);}
      void swap(Array &&a) {std::swap(p, a.p); std::swap(s, a.s);}


      //
      // Cpy
      //
      template<typename T2>
      static pointer Cpy(T2 const *p, size_type s)
      {
         if(!s) return nullptr;

         pointer n = static_cast<pointer>(::operator new(s * sizeof(value_type))), i = n;

         try
         {
            while(s--) new(i++) value_type(*p++);
         }
         catch(...)
         {
            while(i != n) (--i)->~value_type();
            ::operator delete(n);
            throw;
         }

         return n;
      }

      //
      // Del
      //
      static void Del(pointer p, size_type s)
      {
         for(pointer i = p; s--;) i++->~value_type();

         ::operator delete(p);
      }

      //
      // Mov
      //
      template<typename T2>
      static pointer Mov(T2 const *p, size_type s)
      {
         if(!s) return nullptr;

         pointer n = static_cast<pointer>(::operator new(s * sizeof(value_type))), i = n;

         try
         {
            while(s--) new(i++) value_type(std::move(*p++));
         }
         catch(...)
         {
            while(i != n) (--i)->~value_type();
            ::operator delete(n);
            throw;
         }

         return n;
      }

      //
      // New
      //
      static pointer New(size_type s)
      {
         pointer n = static_cast<pointer>(::operator new(s * sizeof(value_type))), i = n;

         try
         {
            while(s--) new(i++) value_type;
         }
         catch(...)
         {
            while(i != n) (--i)->~value_type();
            ::operator delete(n);
            throw;
         }

         return n;
      }

      //
      // Pak
      //
      template<typename... Args>
      static pointer Pak(Args &&...args)
      {
         constexpr size_type s = sizeof...(Args);
         pointer n = static_cast<pointer>(::operator new(s * sizeof(value_type)));

         try {UnPak(n, std::forward<Args>(args)...);}
         catch(...) {::operator delete(n); throw;}

         return n;
      }

   private:
      pointer   p;
      size_type s;

      //
      // UnPak
      //
      template<typename Arg0>
      static void UnPak(pointer n, Arg0 &&arg0)
      {
         new(n) value_type(std::forward<Arg0>(arg0));
      }

      //
      // UnPak
      //
      template<typename Arg0, typename... Args>
      static void UnPak(pointer n, Arg0 &&arg0, Args &&...args)
      {
         new(n) value_type(std::forward<Arg0>(arg0));
         try {UnPak(n + 1, std::forward<Args>(args)...);}
         catch(...) {n->~value_type(); throw;}
      }
   };
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace std
{
   template<typename T>
   void swap(::GDCC::Array<T> &l, ::GDCC::Array<T> &r) {l.swap(r);}
}

namespace GDCC
{
   template<typename T> bool operator == (Array<T> const &l, Array<T> const &r);
   template<typename T> bool operator != (Array<T> const &l, Array<T> const &r);
   template<typename T> bool operator <  (Array<T> const &l, Array<T> const &r);
   template<typename T> bool operator <= (Array<T> const &l, Array<T> const &r);
   template<typename T> bool operator >  (Array<T> const &l, Array<T> const &r);
   template<typename T> bool operator >= (Array<T> const &l, Array<T> const &r);
}

namespace GDCC
{
   //
   // operator Array == Array
   //
   template<typename T> bool operator == (Array<T> const &l, Array<T> const &r)
   {
      return std::equal(l.begin(), l.end(), r.begin(), r.end());
   }

   //
   // operator Array != Array
   //
   template<typename T> bool operator != (Array<T> const &l, Array<T> const &r)
   {
      return !std::equal(l.begin(), l.end(), r.begin(), r.end());
   }

   //
   // operator Array < Array
   //
   template<typename T> bool operator < (Array<T> const &l, Array<T> const &r)
   {
      return CompareLT(l.begin(), l.end(), r.begin(), r.end());
   }

   //
   // operator Array <= Array
   //
   template<typename T> bool operator <= (Array<T> const &l, Array<T> const &r)
   {
      return CompareLE(l.begin(), l.end(), r.begin(), r.end());
   }

   //
   // operator Array > Array
   //
   template<typename T> bool operator > (Array<T> const &l, Array<T> const &r)
   {
      return CompareGT(l.begin(), l.end(), r.begin(), r.end());
   }

   //
   // operator Array >= Array
   //
   template<typename T> bool operator >= (Array<T> const &l, Array<T> const &r)
   {
      return CompareGE(l.begin(), l.end(), r.begin(), r.end());
   }
}

#endif//GDCC__Array_H__

