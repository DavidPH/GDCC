//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation statement argument handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Arg_H__
#define GDCC__IR__Arg_H__

#include "Addr.hpp"
#include "Exp.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_IR_MakeArgPtr1
//
#define GDCC_IR_MakeArgPtr1(name) \
   struct Arg_##name : private ArgPtr1 \
   { \
      using ArgPtr1::ArgPtr1; \
      \
      using ArgPtr1::idx; \
      using ArgPtr1::off; \
      \
      Arg_##name(Arg_##name const &) = default; \
      Arg_##name(Arg_##name &&) = default; \
      \
      Arg_##name &operator = (Arg_##name const &) = default; \
      Arg_##name &operator = (Arg_##name &&) = default; \
   }

//
// GDCC_IR_MakeArgPtr2
//
#define GDCC_IR_MakeArgPtr2(name) \
   struct Arg_##name : private ArgPtr2 \
   { \
      using ArgPtr2::ArgPtr2; \
      \
      using ArgPtr2::arr; \
      using ArgPtr2::idx; \
      using ArgPtr2::off; \
      \
      Arg_##name(Arg_##name const &) = default; \
      Arg_##name(Arg_##name &&) = default; \
      \
      Arg_##name &operator = (Arg_##name const &) = default; \
      Arg_##name &operator = (Arg_##name &&) = default; \
   }


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      class Arg;

      typedef AddrBase ArgBase;

      //
      // ArgPtr1
      //
      struct ArgPtr1
      {
         ArgPtr1(ArgPtr1 const &arg);
         ArgPtr1(ArgPtr1 &&arg);
         ArgPtr1(Arg const &idx, Exp *off);
         ArgPtr1(Arg &&idx, Exp *off);
         ~ArgPtr1();

         ArgPtr1 &operator = (ArgPtr1 const &arg);
         ArgPtr1 &operator = (ArgPtr1 &&arg);

         Arg     *idx;
         Exp::Ref off;
      };

      //
      // ArgPtr2
      //
      struct ArgPtr2
      {
         ArgPtr2(ArgPtr2 const &arg);
         ArgPtr2(ArgPtr2 &&arg);
         ArgPtr2(Arg const &arr, Arg const &idx, Exp *off);
         ArgPtr2(Arg const &arr, Arg &&idx, Exp *off);
         ArgPtr2(Arg &&arr, Arg const &idx, Exp *off);
         ArgPtr2(Arg &&arr, Arg &&idx, Exp *off);
         ~ArgPtr2();

         ArgPtr2 &operator = (ArgPtr2 const &arg);
         ArgPtr2 &operator = (ArgPtr2 &&arg);

         Arg     *arr;
         Arg     *idx;
         Exp::Ref off;
      };

      //
      // Arg_Gen
      //
      GDCC_IR_MakeArgPtr1(Gen);

      //
      // Arg_Cpy
      //
      // Not a valid arg type.
      //
      struct Arg_Cpy
      {
      };

      //
      // Arg_Lit
      //
      // Only valid as a source.
      //
      struct Arg_Lit
      {
         explicit Arg_Lit(Exp *value_) : value{value_} {}

         Exp::Ref value;
      };

      //
      // Arg_Nul
      //
      // Only valid as a destination.
      //
      struct Arg_Nul
      {
      };

      //
      // Arg_Stk
      //
      struct Arg_Stk
      {
      };

      //
      // Arg_Far
      //
      GDCC_IR_MakeArgPtr1(Far);

      //
      // Arg_GblArs
      //
      GDCC_IR_MakeArgPtr1(GblArs);

      //
      // Arg_GblReg
      //
      GDCC_IR_MakeArgPtr1(GblReg);

      //
      // Arg_Loc
      //
      GDCC_IR_MakeArgPtr1(Loc);

      //
      // Arg_LocArs
      //
      GDCC_IR_MakeArgPtr1(LocArs);

      //
      // Arg_LocReg
      //
      GDCC_IR_MakeArgPtr1(LocReg);

      //
      // Arg_MapArs
      //
      GDCC_IR_MakeArgPtr1(MapArs);

      //
      // Arg_MapReg
      //
      GDCC_IR_MakeArgPtr1(MapReg);

      //
      // Arg_StrArs
      //
      GDCC_IR_MakeArgPtr1(StrArs);

      //
      // Arg_Vaa
      //
      GDCC_IR_MakeArgPtr1(Vaa);

      //
      // Arg_WldArs
      //
      GDCC_IR_MakeArgPtr1(WldArs);

      //
      // Arg_WldReg
      //
      GDCC_IR_MakeArgPtr1(WldReg);

      //
      // Arg_GblArr
      //
      GDCC_IR_MakeArgPtr2(GblArr);

      //
      // Arg_MapArr
      //
      GDCC_IR_MakeArgPtr2(MapArr);

      //
      // Arg_StrArr
      //
      GDCC_IR_MakeArgPtr2(StrArr);

      //
      // Arg_WldArr
      //
      GDCC_IR_MakeArgPtr2(WldArr);

      //
      // Arg
      //
      class Arg
      {
      public:
         Arg() : a{ArgBase::Cpy}, aCpy{} {}

         //
         // copy constructor
         //
         Arg(Arg const &arg) : a{arg.a}
         {
            switch(a)
            {
               #define GDCC_IR_AddrList(name) case ArgBase::name: \
                     new(&a##name) Arg_##name(arg.a##name); break;
               #include "AddrList.hpp"
            }
         }

         //
         // move constructor
         //
         Arg(Arg &&arg) : a{arg.a}
         {
            switch(a)
            {
               #define GDCC_IR_AddrList(name) case ArgBase::name: \
                     new(&a##name) Arg_##name(std::move(arg.a##name)); break;
               #include "AddrList.hpp"
            }
         }

         //
         // cast constructors
         //
         #define GDCC_IR_AddrList(name) \
            Arg(Arg_##name const &arg) : a{ArgBase::name}, a##name(          arg ) {} \
            Arg(Arg_##name      &&arg) : a{ArgBase::name}, a##name(std::move(arg)) {}
         #include "AddrList.hpp"

         //
         // destructor
         //
         ~Arg()
         {
            switch(a)
            {
               #define GDCC_IR_AddrList(name) \
                  case ArgBase::name: a##name.~Arg_##name(); break;
               #include "AddrList.hpp"
            }
         }

         //
         // copy assignment
         //
         Arg &operator = (Arg const &arg)
         {
            if(arg.a == a) switch(a)
            {
               #define GDCC_IR_AddrList(name) \
                  case ArgBase::name: a##name = arg.a##name; break;
               #include "AddrList.hpp"
            }
            else
               new((this->~Arg(), this)) Arg(arg);

            return *this;
         }

         //
         // move assignment
         //
         Arg &operator = (Arg &&arg)
         {
            if(arg.a == a) switch(a)
            {
               #define GDCC_IR_AddrList(name) \
                  case ArgBase::name: a##name = std::move(arg.a##name); break;
               #include "AddrList.hpp"
            }
            else
               new((this->~Arg(), this)) Arg(std::move(arg));

            return *this;
         }

         ArgBase a;

         union
         {
            #define GDCC_IR_AddrList(name) Arg_##name a##name;
            #include "AddrList.hpp"
         };
      };
   }
}

#endif//GDCC__IR__Arg_H__

