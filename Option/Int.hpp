//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2022 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Integer option handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Option__Int_H__
#define GDCC__Option__Int_H__

#include "../Option/Base.hpp"

#include "../Option/Exception.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Option
{
   //
   // Int
   //
   template<typename T>
   class Int : public Base
   {
   public:
      // constructor
      Int(Program *program, Info const &optInfo) :
         Base{program, optInfo}, i{0} {}
      Int(Program *program, Info const &optInfo, T i_) :
         Base{program, optInfo}, i{i_} {}

      explicit operator bool () const {return static_cast<bool>(i);}
      operator T const & () const {return i;}

      Int &operator = (T const &i_) {i = i_; return *this;}

      T       &data()       {return i;}
      T const &data() const {return i;}


      //
      // ProcessInt
      //
      static T ProcessInt(Args const &args, char const *s)
      {
         if(s[0] == '-')
            return ProcessInt(args, s + 1, true);
         else
            return ProcessInt(args, s, false);
      }

      //
      // ProcessInt
      //
      static T ProcessInt(Args const &args, char const *s, bool sign)
      {
         if(s[0] == '0')
         {
            if(s[1] == 'X' || s[1] == 'x')
               return ProcessInt(args, s + 2, 16, sign);
            else
               return ProcessInt(args, s + 1, 8, sign);
         }
         else
            return ProcessInt(args, s, 10, sign);
      }

      //
      // ProcessInt
      //
      static T ProcessInt(Args const &args, char const *s, unsigned base)
      {
         if(s[0] == '-')
            return ProcessInt(args, s + 1, base, true);
         else
            return ProcessInt(args, s, base, false);
      }

      //
      // ProcessInt
      //
      static T ProcessInt(Args const &args, char const *s, unsigned base, bool sign)
      {
         T        const max    = std::numeric_limits<T>::max() / base;
         unsigned const digMax = std::numeric_limits<T>::max() % base;

         T i = 0;

         for(; *s; ++s)
         {
            unsigned dig;
            if(*s >= '0' && *s <= '9')
               dig = *s - '0';
            else if(*s >= 'A' && *s <= 'Z')
               dig = *s - 'A' + 10;
            else if(*s >= 'a' && *s <= 'z')
               dig = *s - 'a' + 10;
            else
               Exception::Error(args, "integer required");

            if(dig >= base)
               Exception::Error(args, "integer required");

            if(i > max || (i == max && dig > digMax))
               Exception::Error(args, "integer too large");

            i = i * base + dig;
         }

         return sign ? -i : i;
      }

   protected:
      //
      // v_process
      //
      virtual std::size_t v_process(Args const &args)
      {
         if(args.optFalse) {i = 0; return 0;}
         if(!args.argC) Exception::Error(args, "argument required");

         i = ProcessInt(args, args.argV[0]);

         return 1;
      }

   private:
      T i;
   };
}

#endif//GDCC__Option__Int_H__

