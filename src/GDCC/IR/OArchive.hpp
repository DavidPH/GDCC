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
         explicit OArchive(std::ostream &out_) : out(out_) {}

         OArchive &operator << (bool in) {return putString(in ? "1" : "0");}

         OArchive &operator << (signed           char in) {return putNumber(in);}
         OArchive &operator << (signed     short int  in) {return putNumber(in);}
         OArchive &operator << (signed           int  in) {return putNumber(in);}
         OArchive &operator << (signed      long int  in) {return putNumber(in);}
         OArchive &operator << (signed long long int  in) {return putNumber(in);}

         OArchive &operator << (unsigned           char in) {return putNumber(in);}
         OArchive &operator << (unsigned     short int  in) {return putNumber(in);}
         OArchive &operator << (unsigned           int  in) {return putNumber(in);}
         OArchive &operator << (unsigned      long int  in) {return putNumber(in);}
         OArchive &operator << (unsigned long long int  in) {return putNumber(in);}

         OArchive &operator << (     float  in) {return putNumber(in);}
         OArchive &operator << (     double in) {return putNumber(in);}
         OArchive &operator << (long double in) {return putNumber(in);}

         OArchive &operator << (Float const &in) {return putNumber(in);}
         OArchive &operator << (Integ const &in) {return putNumber(in);}

         //
         // putHeader
         //
         OArchive &putHeader()
         {
            return putString("MgC_NTS").putString("GDCC::IR").putString("");
            return *this;
         }

         //
         // putNumber
         //
         template<typename T>
         OArchive &putNumber(T const &n)
         {
            out << std::hex << n << '\0';
            return *this;
         }

         //
         // putString
         //
         OArchive &putString(char const *s)
         {
            out << s << '\0';
            return *this;
         }

         //
         // putTables
         //
         OArchive &putTables()
         {
            putTablesString();
            putTablesGlyphs();
            putTablesFuncts();
            putTablesImport();
            putTablesSpaces();
            putTablesObject();
            putTablesStrEnt();

            return *this;
         }

      private:
         OArchive &putTablesFuncts();
         OArchive &putTablesGlyphs();
         OArchive &putTablesImport();
         OArchive &putTablesObject();
         OArchive &putTablesSpaces();
         OArchive &putTablesStrEnt();
         OArchive &putTablesString();

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

