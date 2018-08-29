//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation input.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__IArchive_H__
#define GDCC__IR__IArchive_H__

#include "../IR/Types.hpp"

#include "../Core/Array.hpp"
#include "../Core/Number.hpp"
#include "../Core/StringBuf.hpp"

#include <istream>
#include <sstream>
#include <unordered_map>
#include <vector>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::IR
{
   //
   // IArchive
   //
   class IArchive
   {
   public:
      explicit IArchive(std::istream &in);

      IArchive &operator >> (bool &out) {out = getBool(); return *this;}

      IArchive &operator >> (signed           char &out) {return getI(out), *this;}
      IArchive &operator >> (signed     short int  &out) {return getI(out), *this;}
      IArchive &operator >> (signed           int  &out) {return getI(out), *this;}
      IArchive &operator >> (signed      long int  &out) {return getI(out), *this;}
      IArchive &operator >> (signed long long int  &out) {return getI(out), *this;}

      IArchive &operator >> (unsigned           char &out) {return getU(out), *this;}
      IArchive &operator >> (unsigned     short int  &out) {return getU(out), *this;}
      IArchive &operator >> (unsigned           int  &out) {return getU(out), *this;}
      IArchive &operator >> (unsigned      long int  &out) {return getU(out), *this;}
      IArchive &operator >> (unsigned long long int  &out) {return getU(out), *this;}

      IArchive &operator >> (     float  &out) = delete;
      IArchive &operator >> (     double &out) = delete;
      IArchive &operator >> (long double &out) = delete;

      IArchive &operator >> (Core::Float &out) {return out = getRatio(), *this;}
      IArchive &operator >> (Core::Integ &out) {return out = getInteg(), *this;}
      IArchive &operator >> (Core::Ratio &out) {return out = getRatio(), *this;}

      IArchive &operator >> (Core::String      &out);
      IArchive &operator >> (Core::StringIndex &out);

      bool getBool();

      Program *prog;

   private:
      template<typename T>
      T getI()
      {
         bool sign = getBool();
         T out = getU<std::make_unsigned_t<T>>();
         return sign ? -out : out;
      }

      template<typename T>
      T &getI(T &out) {return out = getI<T>();}

      Core::Integ getInteg();
      Core::Ratio getRatio();

      template<typename T>
      T getU()
      {
         T out{0};

         unsigned char c;
         while(((c = in.get()) & 0x80) && in)
            out <<= 7, out += c & 0x7F;
         out <<= 7, out += c;

         return out;
      }

      template<typename T>
      T &getU(T &out) {return out = getU<T>();}

      void getStrTab();

      Core::Array<Core::String> strTab;

      std::istream &in;
   };

   //
   // GetIR_T
   //
   // Used to implement GetIR. Can be specialized for special handling.
   //
   template<typename T>
   struct GetIR_T
   {
      static T GetIR_F(IArchive &in) {T out; in >> out; return out;}
   };

   //
   // GetIRCaller
   //
   template<typename T>
   class GetIRCaller
   {
   public:
      explicit GetIRCaller(IArchive &in_) : in(in_) {}

      explicit operator T () const {return GetIR_T<T>::GetIR_F(in);}

   private:
      IArchive &in;
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   template<typename T>
   IArchive &operator >> (IArchive &in, Core::Array<T> &out);

   IArchive &operator >> (IArchive &in, Core::Origin &out);

   template<typename T>
   IArchive &operator >> (IArchive &in, std::vector<T> &out);

   template<typename T>
   T GetIR(IArchive &in) {return GetIR_T<T>::GetIR_F(in);}

   template<typename T>
   T GetIR(IArchive &in, T const &) {return GetIR_T<T>::GetIR_F(in);}

   //
   // operator IArchive >> Core::Array
   //
   template<typename T>
   IArchive &operator >> (IArchive &in, Core::Array<T> &out)
   {
      typename Core::Array<T>::size_type s; in >> s;
      out = Core::Array<T>(s, GetIRCaller<T>(in));
      return in;
   }

   //
   // operator IArchive >> std::pair
   //
   template<typename T1, typename T2>
   IArchive &operator >> (IArchive &in, std::pair<T1, T2> &out)
   {
      return in >> out.first >> out.second;
   }

   //
   // operator IArchive >> std::unordered_map
   //
   template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
   IArchive &operator >> (IArchive &in,
      std::unordered_map<Key, T, Hash, KeyEqual, Allocator> &out)
   {
      typename std::unordered_map<Key, T, Hash, KeyEqual, Allocator>::size_type s;
      in >> s;
      out.clear(); out.reserve(s);
      while(s--)
         out.emplace(GetIR_T<std::pair<Key, T>>::GetIR_F(in));
      return in;
   }

   //
   // operator IArchive >> std::vector
   //
   template<typename T>
   IArchive &operator >> (IArchive &in, std::vector<T> &out)
   {
      typename std::vector<T>::size_type s; in >> s;
      out.clear(); out.reserve(s);
      while(s--)
         out.emplace_back(GetIR_T<T>::GetIR_F(in));
      return in;
   }
}

#endif//GDCC__IR__IArchive_H__

