//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// General parsing utilities.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__Parse_H__
#define GDCC__Core__Parse_H__

#include "Number.hpp"
#include "String.hpp"

#include <istream>
#include <string>
#include <tuple>
#include <utility>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Core
   {
      bool IsDigit(char c, unsigned base);

      Integ MergeNumberFixedC(Integ valI, Integ valF, FastI valE,
         std::size_t digF, unsigned base);

      char ParseEscapeC(std::istream &in);

      std::pair<char const */*end*/, unsigned /*base*/>
      ParseNumberBaseC(char const *in);

      std::pair<char const */*end*/, FastI /*val*/>
      ParseNumberExpC(char const *in);

      std::tuple<char const */*end*/, FastU /*val*/, std::size_t /*len*/>
      ParseNumberFastU(char const *in, unsigned base);

      std::tuple<char const */*end*/, Integ /*val*/, std::size_t /*len*/>
      ParseNumberInteg(char const *in, unsigned base);

      std::tuple<char const */*end*/, Ratio /*val*/, std::size_t /*len*/>
      ParseNumberRatioC(char const *in, unsigned base);

      String ParseStringC(std::string const &in);
      String ParseStringC(String in);
      String ParseStringC(String in, std::size_t offset);

      void ReadEscapeC(std::istream &in, std::string &out);

      std::string ReadNumberC(std::istream &in);

      std::string ReadStringC(std::istream &in, char term);

      unsigned ToDigit(char c);
   }
}

#endif//GDCC__Core__Parse_H__

