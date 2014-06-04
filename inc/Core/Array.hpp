//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Non-resizing array.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__Array_H__
#define GDCC__Core__Array_H__

#include "Compare.hpp"

#include <algorithm>
#include <iterator>
#include <memory>
#include <stdexcept>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      struct MoveType {};
      struct PackType {};

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


         Array() : p{nullptr}, e{nullptr} {}
         Array(Array const &a) : p{Cpy(a.p, a.e)}, e{p + a.size()} {}
         Array(Array &&a) : p{a.p}, e{a.e} {a.p = a.e = nullptr;}
         explicit Array(size_type s) : p{New(s)}, e{p + s} {}
         ~Array() {Del(p, e);}

         // Sized constructor.
         template<typename... Args>
         Array(size_type s, Args const &...args) : p{New(s, args...)}, e{p + s} {}

         // Range-copy constructor.
         template<typename Itr>
         Array(Itr first, Itr last) : p{Cpy(first, last)},
            e{p + std::distance(first, last)} {}

         // Range-call constructor.
         template<typename Itr, typename Fn>
         Array(Itr first, Itr last, Fn const &fn) : p{Cpy(first, last, fn)},
            e{p + std::distance(first, last)} {}

         // Range-move contructor.
         template<typename Itr>
         Array(MoveType, Itr first, Itr last) :
            p{Cpy(first, last, std::move<typename std::iterator_traits<Itr>::reference>)},
            e{p + std::distance(first, last)} {}

         // Variadic constructor.
         template<typename... Args>
         Array(PackType, Args &&...args) : p{Pak(std::forward<Args>(args)...)},
            e{p + sizeof...(Args)} {}

         // operator Array[]
               reference operator [] (size_type i)       {return p[i];}
         const_reference operator [] (size_type i) const {return p[i];}

         //
         // copy assignment
         //
         Array &operator = (Array const &a)
         {
            pointer n = Cpy(a.p, a.e);
            Del(p, e);
            e = (p = n) + a.size();
            return *this;
         }

         // move assignment
         Array &operator = (Array &&a) {swap(a); return *this;}

         //
         // at
         //
         reference at(size_type i)
         {
            if(i < size()) return p[i];
            throw std::out_of_range("GDCC::Core::Array::at");
         }

         //
         // at
         //
         const_reference at(size_type i) const
         {
            if(i < size()) return p[i];
            throw std::out_of_range("GDCC::Core::Array::at");
         }

         // back
               reference back()       {return *(e - 1);}
         const_reference back() const {return *(e - 1);}

         // begin
               iterator begin()       {return p;}
         const_iterator begin() const {return p;}

         // cbegin
         const_iterator cbegin() const {return p;}

         // cend
         const_iterator cend() const {return e;}

         // clear
         void clear() {Del(p, e); p = e = nullptr;}

         // crbegin
         const_reverse_iterator crbegin() const
            {return static_cast<const_reverse_iterator>(e);}

         // crend
         const_reverse_iterator crend() const
            {return static_cast<const_reverse_iterator>(p);}

         // data
               pointer data()       {return p;}
         const_pointer data() const {return p;}

         // empty
         bool empty() const {return p == e;}

         // end
               iterator end()       {return e;}
         const_iterator end() const {return e;}

         // fill
         void fill(const_reference v) {for(auto &i : *this) i = v;}

         // front
               reference front()       {return *p;}
         const_reference front() const {return *p;}

         // max_size
         size_type max_size() const {return std::numeric_limits<size_type>::max();}

         // rbegin
               reverse_iterator rbegin()
            {return static_cast<      reverse_iterator>(e);}
         const_reverse_iterator rbegin() const
            {return static_cast<const_reverse_iterator>(e);}

         // rend
               reverse_iterator rend()
            {return static_cast<      reverse_iterator>(p);}
         const_reverse_iterator rend() const
            {return static_cast<const_reverse_iterator>(p);}

         // size
         size_type size() const {return e - p;}

         // swap
         void swap(Array  &a) {std::swap(p, a.p); std::swap(e, a.e);}
         void swap(Array &&a) {std::swap(p, a.p); std::swap(e, a.e);}


         //
         // Cpy
         //
         template<typename Itr>
         static pointer Cpy(Itr first, Itr last)
         {
            size_type s = std::distance(first, last);
            pointer   n = Raw(s), i = n;

            try
            {
               while(s--) new(i++) value_type(*first++);
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
         // Cpy
         //
         template<typename Itr, typename Fn>
         static pointer Cpy(Itr first, Itr last, Fn const &fn)
         {
            size_type s = std::distance(first, last);
            pointer   n = Raw(s), i = n;

            try
            {
               while(s--) new(i++) value_type(fn(*first++));
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
         static void Del(const_pointer first, const_pointer last)
         {
            while(last != first) (--last)->~value_type();
            ::operator delete(const_cast<pointer>(first));
         }

         //
         // New
         //
         static pointer New(size_type s)
         {
            pointer n = Raw(s), i = n;

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
         // New
         //
         template<typename... Args>
         static pointer New(size_type s, Args const &...args)
         {
            pointer n = Raw(s), i = n;

            try
            {
               while(s--) new(i++) value_type(args...);
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
            pointer n = Raw(s);

            try {UnPak(n, std::forward<Args>(args)...);}
            catch(...) {::operator delete(n); throw;}

            return n;
         }

         //
         // Raw
         //
         static pointer Raw(size_type s)
         {
            if(!s) return nullptr;

            return static_cast<pointer>(::operator new(s * sizeof(value_type)));
         }

      private:
         pointer p, e;

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
}


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace Core
   {
      constexpr MoveType Move = {};
      constexpr PackType Pack = {};
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace std
{
   template<typename T>
   void swap(::GDCC::Core::Array<T> &l, ::GDCC::Core::Array<T> &r) {l.swap(r);}
}

namespace GDCC
{
   namespace Core
   {
      template<typename T> bool operator == (Array<T> const &l, Array<T> const &r);
      template<typename T> bool operator != (Array<T> const &l, Array<T> const &r);
      template<typename T> bool operator <  (Array<T> const &l, Array<T> const &r);
      template<typename T> bool operator <= (Array<T> const &l, Array<T> const &r);
      template<typename T> bool operator >  (Array<T> const &l, Array<T> const &r);
      template<typename T> bool operator >= (Array<T> const &l, Array<T> const &r);
   }
}

namespace GDCC
{
   namespace Core
   {
      //
      // operator Array == Array
      //
      template<typename T> bool operator == (Array<T> const &l, Array<T> const &r)
      {
         return std::equal(l.begin(), l.end(), r.begin());
      }

      //
      // operator Array != Array
      //
      template<typename T> bool operator != (Array<T> const &l, Array<T> const &r)
      {
         return !std::equal(l.begin(), l.end(), r.begin());
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
}

#endif//GDCC__Core__Array_H__

