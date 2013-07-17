//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
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

#include "../Number.hpp"

#include <ostream>
#include <vector>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      //
      // OArchive
      //
      class OArchive
      {
      public:
         explicit OArchive(std::ostream &out_) : out{out_} {}

         OArchive &operator << (bool in) {return writeString(in ? "1" : "0");}

         OArchive &operator << (signed           char in) {return writeNumber(in);}
         OArchive &operator << (signed     short int  in) {return writeNumber(in);}
         OArchive &operator << (signed           int  in) {return writeNumber(in);}
         OArchive &operator << (signed      long int  in) {return writeNumber(in);}
         OArchive &operator << (signed long long int  in) {return writeNumber(in);}

         OArchive &operator << (unsigned           char in) {return writeNumber(in);}
         OArchive &operator << (unsigned     short int  in) {return writeNumber(in);}
         OArchive &operator << (unsigned           int  in) {return writeNumber(in);}
         OArchive &operator << (unsigned      long int  in) {return writeNumber(in);}
         OArchive &operator << (unsigned long long int  in) {return writeNumber(in);}

         OArchive &operator << (     float  in) {return writeNumber(in);}
         OArchive &operator << (     double in) {return writeNumber(in);}
         OArchive &operator << (long double in) {return writeNumber(in);}

         OArchive &operator << (Float const &in) {return writeNumber(in);}
         OArchive &operator << (Integ const &in) {return writeNumber(in);}

         //
         // writeHeader
         //
         OArchive &writeHeader()
         {
            return writeString("MgC_NTS").writeString("GDCC::IR").writeString("");
            return *this;
         }

         //
         // writeNumber
         //
         template<typename T>
         OArchive &writeNumber(T const &n)
         {
            out << std::hex << n << '\0';
            return *this;
         }

         //
         // writeString
         //
         OArchive &writeString(char const *s)
         {
            out << s << '\0';
            return *this;
         }

         //
         // writeTables
         //
         OArchive &writeTables()
         {
            writeTablesString();
            writeTablesGlyphs();
            writeTablesFuncts();

            return *this;
         }

      private:
         OArchive &writeTablesFuncts();
         OArchive &writeTablesGlyphs();
         OArchive &writeTablesString();

         std::ostream &out;
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
      OArchive &operator << (OArchive &out, std::vector<T> const &in);
   }
}

namespace GDCC
{
   namespace IR
   {
      //
      // operator OArchive << std::vector
      //
      template<typename T>
      OArchive &operator << (OArchive &out, std::vector<T> const &in)
      {
         out << in.size();
         for(auto const &i : in)
            out << i;
         return out;
      }
   }
}

#endif//GDCC__IR__OArchive_H__

