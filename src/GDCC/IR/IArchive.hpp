//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
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

#include "../Array.hpp"
#include "../Number.hpp"
#include "../String.hpp"

#include <istream>
#include <sstream>
#include <vector>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
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

         IArchive &operator >> (Float &out) {return getNumber(out);}
         IArchive &operator >> (Integ &out) {return getNumber(out);}

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

         //
         // getTables
         //
         IArchive &getTables()
         {
            getTablesString();
            getTablesGlyphs();
            getTablesFuncts();
            getTablesImport();
            getTablesSpaces();
            getTablesObject();
            getTablesStrEnt();

            return *this;
         }


         friend IArchive &::GDCC::operator >> (IArchive &in, String &out);

      private:
         IArchive &getTablesFuncts();
         IArchive &getTablesGlyphs();
         IArchive &getTablesImport();
         IArchive &getTablesObject();
         IArchive &getTablesSpaces();
         IArchive &getTablesStrEnt();
         IArchive &getTablesString();

         std::vector<char>   data;
         Array<char const *> str;
         Array<String>       stab;

         Array<char const *>::iterator itr;
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
      // operator IArchive >> std::vector
      //
      template<typename T>
      IArchive &operator >> (IArchive &in, std::vector<T> &out)
      {
         typename std::vector<T>::size_type s; in >> s;
         for(auto &o : (out = std::vector<T>(s)))
            in >> o;
         return in;
      }
   }
}

#endif//GDCC__IR__IArchive_H__

