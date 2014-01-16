//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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

#include "../Core/Array.hpp"
#include "../Core/Number.hpp"
#include "../Core/String.hpp"

#include <istream>
#include <sstream>
#include <vector>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      struct Origin;
   }

   namespace IR
   {
      class Program;

      //
      // IArchive
      //
      class IArchive
      {
      public:
         explicit IArchive(std::istream &in);

         explicit operator bool () const {return itr != str.end();}

         IArchive &operator >> (bool &out) {out = getBool(); return *this;}

         IArchive &operator >> (signed           char &out) {return getNumber(out);}
         IArchive &operator >> (signed     short int  &out) {return getNumber(out);}
         IArchive &operator >> (signed           int  &out) {return getNumber(out);}
         IArchive &operator >> (signed      long int  &out) {return getNumber(out);}
         IArchive &operator >> (signed long long int  &out) {return getNumber(out);}

         IArchive &operator >> (unsigned           char &out) {return getNumber(out);}
         IArchive &operator >> (unsigned     short int  &out) {return getNumber(out);}
         IArchive &operator >> (unsigned           int  &out) {return getNumber(out);}
         IArchive &operator >> (unsigned      long int  &out) {return getNumber(out);}
         IArchive &operator >> (unsigned long long int  &out) {return getNumber(out);}

         IArchive &operator >> (     float  &out) {return getNumber(out);}
         IArchive &operator >> (     double &out) {return getNumber(out);}
         IArchive &operator >> (long double &out) {return getNumber(out);}

         IArchive &operator >> (Core::Float &out) {return getNumber(out);}
         IArchive &operator >> (Core::Integ &out) {return getNumber(out);}

         char const *get() {return itr != str.end() ? *itr++ : "";}

         //
         // getBool
         //
         bool getBool()
         {
            return !std::strcmp(get(), "1");
         }

         template<typename T> T getNumber() {T n; getNumber(n); return n;}

         //
         // getNumber
         //
         template<typename T>
         IArchive &getNumber(T &out)
         {
            auto s = get();

            if(*s)
            {
               std::istringstream in{s};
               in >> std::hex >> out;
            }
            else
               out = 0;

            return *this;
         }

         IArchive &getHeader();

         Program *prog;


         friend IArchive &operator >> (IArchive &in, Core::String      &out);
         friend IArchive &operator >> (IArchive &in, Core::StringIndex &out);

      private:
         IArchive &getTablesString();

         std::vector<char>         data;
         Core::Array<char const *> str;
         Core::Array<Core::String> stab;

         Core::Array<char const *>::iterator itr;
      };

      //
      // GetIR_T
      //
      // Used to implement GetIR. Can be specialized for special handling.
      //
      template<typename T> struct GetIR_T
      {
         static T GetIR_F(IArchive &in) {T out; in >> out; return out;}
      };

      //
      // GetIRCaller
      //
      template<typename T> class GetIRCaller
      {
      public:
         explicit GetIRCaller(IArchive &in_) : in(in_) {}

         explicit operator T () const {return GetIR_T<T>::GetIR_F(in);}

      private:
         IArchive &in;
      };
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      template<typename T>
      IArchive &operator >> (IArchive &in, Core::Array<T> &out);

      IArchive &operator >> (IArchive &in, Core::Origin &out);

      IArchive &operator >> (IArchive &in, Core::String      &out);
      IArchive &operator >> (IArchive &in, Core::StringIndex &out);

      template<typename T>
      IArchive &operator >> (IArchive &in, std::vector<T> &out);

      template<typename T>
      T GetIR(IArchive &in) {return GetIR_T<T>::GetIR_F(in);}

      template<typename T>
      T GetIR(IArchive &in, T const &) {return GetIR_T<T>::GetIR_F(in);}
   }
}

namespace GDCC
{
   namespace IR
   {
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
}

#endif//GDCC__IR__IArchive_H__

