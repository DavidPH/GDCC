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

#include "../Core/Number.hpp"

#include <ostream>
#include <unordered_map>
#include <utility>
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

         OArchive &operator << (bool in) {out << (in ? "1" : "0") << '\0'; return *this;}

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

         OArchive &operator << (Core::Float const &in) {return putNumber(in);}
         OArchive &operator << (Core::Integ const &in) {return putNumber(in);}

         //
         // putHeader
         //
         OArchive &putHeader()
         {
            out << "MgC_NTS" << '\0' << "GDCC::IR" << '\0' << '\0';
            putTablesString();
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

      private:
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
      template<typename T1, typename T2>
      OArchive &operator << (OArchive &out, std::pair<T1, T2> const &in);

      template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
      OArchive &operator << (OArchive &out,
         std::unordered_map<Key, T, Hash, KeyEqual, Allocator> const &in);

      template<typename T, typename Allocator>
      OArchive &operator << (OArchive &out, std::vector<T, Allocator> const &in);
   }
}

namespace GDCC
{
   namespace IR
   {
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
}

#endif//GDCC__IR__OArchive_H__

