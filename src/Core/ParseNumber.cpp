//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Number parsing utilities.
//
//-----------------------------------------------------------------------------

#include "Core/Parse.hpp"

#include <climits>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// IsNumberCharC
//
static bool IsNumberCharC(int c, int buf)
{
   if(std::isalnum(c)) return true;
   if(c == '.' || c == '_') return true;

   if(c == '+' || c == '-')
      return buf == 'E' || buf == 'e' || buf == 'P' || buf == 'p';

   return false;
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Core
   {
      //
      // IsDigit
      //
      bool IsDigit(char c, unsigned base)
      {
         switch(c)
         {
         case '0': return base > 0;
         case '1': return base > 1;
         case '2': return base > 2;
         case '3': return base > 3;
         case '4': return base > 4;
         case '5': return base > 5;
         case '6': return base > 6;
         case '7': return base > 7;
         case '8': return base > 8;
         case '9': return base > 9;

         case 'A': case 'a': return base > 10;
         case 'B': case 'b': return base > 11;
         case 'C': case 'c': return base > 12;
         case 'D': case 'd': return base > 13;
         case 'E': case 'e': return base > 14;
         case 'F': case 'f': return base > 15;
         case 'G': case 'g': return base > 16;
         case 'H': case 'h': return base > 17;
         case 'I': case 'i': return base > 18;
         case 'J': case 'j': return base > 19;
         case 'K': case 'k': return base > 20;
         case 'L': case 'l': return base > 21;
         case 'M': case 'm': return base > 22;
         case 'N': case 'n': return base > 23;
         case 'O': case 'o': return base > 24;
         case 'P': case 'p': return base > 25;
         case 'Q': case 'q': return base > 26;
         case 'R': case 'r': return base > 27;
         case 'S': case 's': return base > 28;
         case 'T': case 't': return base > 29;
         case 'U': case 'u': return base > 30;
         case 'V': case 'v': return base > 31;
         case 'W': case 'w': return base > 32;
         case 'X': case 'x': return base > 33;
         case 'Y': case 'y': return base > 34;
         case 'Z': case 'z': return base > 35;

         default: return false;
         }
      }

      //
      // MergeNumberFixedC
      //
      Integ MergeNumberFixedC(Integ valI, Integ valF, FastI valE,
         std::size_t digF, unsigned base)
      {
         // If exponent is scaling up, do so now to avoid losing fractional bits.
         if(valE > 0)
         {
            if(base == 10)
               for(auto i = valE; i--;)
                  valI *= 10, valF *= 10;
            else
               valI <<= valE, valF <<= valE;
         }

         // Adjust valF for number of digits.
         switch(base)
         {
         case  2: valF >>= digF * 1; break;
         case  8: valF >>= digF * 3; break;
         case 16: valF >>= digF * 4; break;
         default:
            for(auto i = digF; i--;) valF /= base;
            break;
         }

         auto &val = valI += valF;

         // If exponent is scaling down, do so now to operate on val as a whole.
         if(valE < 0)
         {
            if(base == 10)
               for(auto i = -valE; i--;)
                  val /= 10;
            else
               val >>= -valE;
         }

         return std::move(val);
      }

      //
      // ParseNumberBaseC
      //
      std::pair<char const */*end*/, unsigned /*base*/>
      ParseNumberBaseC(char const *in)
      {
         if(in[0] == '0') switch(in[1])
         {
         case 'B': case 'b': return {in + 2,  2};
         case 'X': case 'x': return {in + 2, 16};
         default:
            for(auto s = in; *s && *s != '_'; ++s)
               if(*s == '.') return {in, 10};
            return {in + 1,  8};
         }
         else
            return {in, 10};
      }

      //
      // ParseNumberExpC
      //
      std::pair<char const */*end*/, FastI /*val*/>
      ParseNumberExpC(char const *s)
      {
         bool  signE;
         Integ rawE;

         switch(*++s)
         {
         case '-': signE = true;  ++s; break;
         case '+': signE = false; ++s; break;
         default:  signE = false;      break;
         }

         std::tie(s, rawE, std::ignore) = ParseNumberInteg(s, 10);

         auto valE = number_cast<FastI>(rawE);
         if(signE) valE = -valE;

         return {s, valE};
      }

      //
      // ParseNumberFastU
      //
      std::tuple<char const */*end*/, FastU /*val*/, std::size_t /*len*/>
      ParseNumberFastU(char const *in, unsigned base)
      {
         auto t = ParseNumberInteg(in, base);

         auto &end = std::get<0>(t);
         auto &val = std::get<1>(t);
         auto &len = std::get<2>(t);

         return std::make_tuple(end, number_cast<FastU>(val), len);
      }

      //
      // ParseNumberInteg
      //
      std::tuple<char const */*end*/, Integ /*val*/, std::size_t /*len*/>
      ParseNumberInteg(char const *in, unsigned base)
      {
         Integ val = 0;
         char const *s = in;

         while(IsDigit(*s, base))
            val *= base, val += ToDigit(*s++);

         return std::make_tuple(s, std::move(val), s - in);
      }

      //
      // ParseNumberRatioC
      //
      std::tuple<char const */*end*/, Ratio /*val*/, std::size_t /*len*/>
      ParseNumberRatioC(char const *in, unsigned base)
      {
         char const *first = in;

         // Read integral part.
         Ratio val;
         std::tie(in, val, std::ignore) = ParseNumberInteg(in, base);

         // Read fractional part.
         if(*in == '.')
         {
            Ratio valF;
            std::size_t digF;
            std::tie(in, valF, digF) = ParseNumberInteg(++in, base);

            for(auto i = digF; i--;)
               valF /= base;

            val += valF;
         }

         // Read exponent.
         if(*in == 'E' || *in == 'e' || *in == 'P' || *in == 'p')
         {
            FastI valE;
            std::tie(in, valE) = ParseNumberExpC(in);

            if(in < 0)
            {
               if(base == 10)
                  for(auto i = -valE; i--;)
                     val /= 10;
               else
                  val >>= -valE;
            }
            else if(in > 0)
            {
               if(base == 10)
                  for(auto i = valE; i--;)
                     val *= 10;
               else
                  val <<= valE;
            }
         }

         return std::make_tuple(in, std::move(val), in - first);
      }

      //
      // ReadNumberC
      //
      std::string ReadNumberC(std::istream &in)
      {
         std::string buf;

         for(int c = in.get();; c = in.get())
         {
            if(c == EOF) break;

            if(!IsNumberCharC(c, buf.empty() ? EOF : buf.back()))
            {
               in.putback(c);
               break;
            }

            buf += static_cast<char>(c);
         }

         return buf;
      }

      //
      // ToDigit
      //
      unsigned ToDigit(char c)
      {
         switch(c)
         {
         case '0': return  0;
         case '1': return  1;
         case '2': return  2;
         case '3': return  3;
         case '4': return  4;
         case '5': return  5;
         case '6': return  6;
         case '7': return  7;
         case '8': return  8;
         case '9': return  9;

         case 'A': case 'a': return 10;
         case 'B': case 'b': return 11;
         case 'C': case 'c': return 12;
         case 'D': case 'd': return 13;
         case 'E': case 'e': return 14;
         case 'F': case 'f': return 15;
         case 'G': case 'g': return 16;
         case 'H': case 'h': return 17;
         case 'I': case 'i': return 18;
         case 'J': case 'j': return 19;
         case 'K': case 'k': return 20;
         case 'L': case 'l': return 21;
         case 'M': case 'm': return 22;
         case 'N': case 'n': return 23;
         case 'O': case 'o': return 24;
         case 'P': case 'p': return 25;
         case 'Q': case 'q': return 26;
         case 'R': case 'r': return 27;
         case 'S': case 's': return 28;
         case 'T': case 't': return 29;
         case 'U': case 'u': return 30;
         case 'V': case 'v': return 31;
         case 'W': case 'w': return 32;
         case 'X': case 'x': return 33;
         case 'Y': case 'y': return 34;
         case 'Z': case 'z': return 35;

         default: return UINT_MAX;
         }
      }
   }
}

// EOF

