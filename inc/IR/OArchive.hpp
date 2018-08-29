//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation output.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__OArchive_H__
#define GDCC__IR__OArchive_H__

#include "../IR/Types.hpp"

#include "../Core/Array.hpp"
#include "../Core/Number.hpp"
#include "../Core/String.hpp"

#include <ostream>
#include <unordered_map>
#include <utility>
#include <vector>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::IR
{
   //
   // OArchive
   //
   class OArchive
   {
   public:
      explicit OArchive(std::ostream &out);

      OArchive &operator << (bool in) {return out.put(in), *this;}

      OArchive &operator << (signed           char in) {return putI(in), *this;}
      OArchive &operator << (signed     short int  in) {return putI(in), *this;}
      OArchive &operator << (signed           int  in) {return putI(in), *this;}
      OArchive &operator << (signed      long int  in) {return putI(in), *this;}
      OArchive &operator << (signed long long int  in) {return putI(in), *this;}

      OArchive &operator << (unsigned           char in) {return putU(in), *this;}
      OArchive &operator << (unsigned     short int  in) {return putU(in), *this;}
      OArchive &operator << (unsigned           int  in) {return putU(in), *this;}
      OArchive &operator << (unsigned      long int  in) {return putU(in), *this;}
      OArchive &operator << (unsigned long long int  in) {return putU(in), *this;}

      OArchive &operator << (     float  in) = delete;
      OArchive &operator << (     double in) = delete;
      OArchive &operator << (long double in) = delete;

      OArchive &operator << (Core::Float const &in) {return putRatio(Core::Ratio(in)), *this;}
      OArchive &operator << (Core::Integ const &in) {return putInteg(in), *this;}
      OArchive &operator << (Core::Ratio const &in) {return putRatio(in), *this;}

      OArchive &operator << (Core::String      in);
      OArchive &operator << (Core::StringIndex in);

      void putHead();
      void putTail();

   private:
      template<typename T>
      void putI(T in)
      {
         bool sign = in < 0;
         out.put(sign);
         putU(static_cast<std::make_unsigned_t<T>>(sign ? -in : in));
      }

      void putInteg(Core::Integ in);

      void putRatio(Core::Ratio const &in);

      void putStrTab();

      template<typename T>
      void putU(T in)
      {
         constexpr std::size_t len = (sizeof(T) * CHAR_BIT + 6) / 7;
         char buf[len], *ptr = buf + len;

         *--ptr = static_cast<char>(in & 0x7F);
         while((in >>= 7))
            *--ptr = static_cast<char>(in & 0x7F) | 0x80;

         out.write(ptr, (buf + len) - ptr);
      }

      Core::Array<bool> strUse;

      std::ostream &out;
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   template<typename T>
   OArchive &operator << (OArchive &out, Core::Array<T> const &in);

   OArchive &operator << (OArchive &out, Core::Origin const &in);

   template<typename T1, typename T2>
   OArchive &operator << (OArchive &out, std::pair<T1, T2> const &in);

   template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
   OArchive &operator << (OArchive &out,
      std::unordered_map<Key, T, Hash, KeyEqual, Allocator> const &in);

   template<typename T, typename Allocator>
   OArchive &operator << (OArchive &out, std::vector<T, Allocator> const &in);

   //
   // operator OArchive << Core::Array
   //
   template<typename T>
   OArchive &operator << (OArchive &out, Core::Array<T> const &in)
   {
      out << in.size();
      for(auto const &i : in)
         out << i;
      return out;
   }

   //
   // operator OArchive << std::pair
   //
   template<typename T1, typename T2>
   OArchive &operator << (OArchive &out, std::pair<T1, T2> const &in)
   {
      return out << in.first << in.second;
   }

   //
   // operator OArchive << std::unordered_map
   //
   template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
   OArchive &operator << (OArchive &out,
      std::unordered_map<Key, T, Hash, KeyEqual, Allocator> const &in)
   {
      out << in.size();
      for(auto const &i : in)
         out << i;
      return out;
   }

   //
   // operator OArchive << std::vector
   //
   template<typename T, typename Allocator>
   OArchive &operator << (OArchive &out, std::vector<T, Allocator> const &in)
   {
      out << in.size();
      for(auto const &i : in)
         out << i;
      return out;
   }
}

#endif//GDCC__IR__OArchive_H__

