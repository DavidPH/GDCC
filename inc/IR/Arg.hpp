//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
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

#include "../IR/Addr.hpp"
#include "../IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_IR_MakeArgPtr1
//
#define GDCC_IR_MakeArgPtr1(name) \
   class Arg_##name : public ArgPtr1 \
   { \
   public: \
      using ArgPtr1::ArgPtr1; \
      \
      using ArgPtr1::getIR; \
      using ArgPtr1::putIR; \
      \
      using ArgPtr1::idx; \
      using ArgPtr1::off; \
      \
      Arg_##name(Arg_##name const &) = default; \
      Arg_##name(Arg_##name &&) = default; \
      \
      bool operator == (Arg_##name const &arg) const \
         {return *idx == *arg.idx && off == arg.off;} \
      \
      Arg_##name &operator = (Arg_##name const &) = default; \
      Arg_##name &operator = (Arg_##name &&) = default; \
      \
      Arg_##name getOffset(Core::FastU w) const {return {*idx, off + w};} \
   }

//
// GDCC_IR_MakeArgPtr2
//
#define GDCC_IR_MakeArgPtr2(name) \
   class Arg_##name : public ArgPtr2 \
   { \
   public: \
      using ArgPtr2::ArgPtr2; \
      \
      using ArgPtr2::getIR; \
      using ArgPtr2::putIR; \
      \
      using ArgPtr2::arr; \
      using ArgPtr2::idx; \
      using ArgPtr2::off; \
      \
      Arg_##name(Arg_##name const &) = default; \
      Arg_##name(Arg_##name &&) = default; \
      \
      bool operator == (Arg_##name const &arg) const \
         {return *arr == *arg.arr && *idx == *arg.idx && off == arg.off;} \
      \
      Arg_##name &operator = (Arg_##name const &) = default; \
      Arg_##name &operator = (Arg_##name &&) = default; \
      \
      Arg_##name getOffset(Core::FastU w) const {return {*arr, *idx, off + w};} \
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

      bool operator == (Arg const &l, Arg const &r);

      //
      // ArgPtr1
      //
      class ArgPtr1
      {
      public:
         ArgPtr1(ArgPtr1 const &arg);
         ArgPtr1(ArgPtr1 &&arg);
         ArgPtr1(Arg const &idx);
         ArgPtr1(Arg &&idx);
         ArgPtr1(Arg const &idx, Core::FastU off);
         ArgPtr1(Arg &&idx, Core::FastU off);
         explicit ArgPtr1(IArchive &in);
         ~ArgPtr1();

         ArgPtr1 &operator = (ArgPtr1 const &arg);
         ArgPtr1 &operator = (ArgPtr1 &&arg);

         IArchive &getIR(IArchive &in);

         OArchive &putIR(OArchive &out) const;

         Arg        *idx;
         Core::FastU off;
      };

      //
      // ArgPtr2
      //
      class ArgPtr2
      {
      public:
         ArgPtr2(ArgPtr2 const &arg);
         ArgPtr2(ArgPtr2 &&arg);
         ArgPtr2(Arg const &arr, Arg const &idx);
         ArgPtr2(Arg const &arr, Arg &&idx);
         ArgPtr2(Arg &&arr, Arg const &idx);
         ArgPtr2(Arg &&arr, Arg &&idx);
         ArgPtr2(Arg const &arr, Arg const &idx, Core::FastU off);
         ArgPtr2(Arg const &arr, Arg &&idx, Core::FastU off);
         ArgPtr2(Arg &&arr, Arg const &idx, Core::FastU off);
         ArgPtr2(Arg &&arr, Arg &&idx, Core::FastU off);
         explicit ArgPtr2(IArchive &in);
         ~ArgPtr2();

         ArgPtr2 &operator = (ArgPtr2 const &arg);
         ArgPtr2 &operator = (ArgPtr2 &&arg);

         IArchive &getIR(IArchive &in);

         OArchive &putIR(OArchive &out) const;

         Arg        *arr;
         Arg        *idx;
         Core::FastU off;
      };

      //
      // Arg_Gen
      //
      GDCC_IR_MakeArgPtr1(Gen);

      //
      // Arg_Cpy
      //
      // Not a valid arg type.
      // Used for argument placeholders in assembler macros.
      //
      class Arg_Cpy
      {
      public:
         Arg_Cpy() : value{0} {}
         explicit Arg_Cpy(Core::FastU value_) : value{value_} {}
         explicit Arg_Cpy(IArchive &in);

         bool operator == (Arg_Cpy const &arg) const
            {return value == arg.value;}

         IArchive &getIR(IArchive &in);

         Arg_Cpy getOffset(Core::FastU) const {return Arg_Cpy(value);}

         OArchive &putIR(OArchive &out) const;

         Core::FastU value;
      };

      //
      // Arg_Lit
      //
      // Only valid as a source.
      //
      class Arg_Lit
      {
      public:
         explicit Arg_Lit(Exp const *value_) : value{value_}, off{0} {}
         Arg_Lit(Exp const *value_, Core::FastU off_) : value{value_}, off{off_} {}
         explicit Arg_Lit(IArchive &in);

         bool operator == (Arg_Lit const &arg) const
            {return off == arg.off && *value == *arg.value;}

         IArchive &getIR(IArchive &in);

         Arg_Lit getOffset(Core::FastU w) const {return {value, off + w};}

         OArchive &putIR(OArchive &out) const;

         Exp::CRef   value;
         Core::FastU off;
      };

      //
      // Arg_Nul
      //
      // Only valid as a destination.
      //
      class Arg_Nul
      {
      public:
         Arg_Nul() = default;
         explicit Arg_Nul(IArchive &) {}

         bool operator == (Arg_Nul const &) const {return true;}

         IArchive &getIR(IArchive &in) {return in;}

         Arg_Nul getOffset(Core::FastU) const {return {};}

         OArchive &putIR(OArchive &out) const {return out;}
      };

      //
      // Arg_Stk
      //
      class Arg_Stk
      {
      public:
         Arg_Stk() = default;
         explicit Arg_Stk(IArchive &) {}

         bool operator == (Arg_Stk const &) const {return true;}

         IArchive &getIR(IArchive &in) {return in;}

         Arg_Stk getOffset(Core::FastU) const {return {};}

         OArchive &putIR(OArchive &out) const {return out;}
      };

      //
      // Arg_Aut
      //
      GDCC_IR_MakeArgPtr1(Aut);

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
      // Arg_Sta
      //
      GDCC_IR_MakeArgPtr1(Sta);

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
      // Arg_LocArr
      //
      GDCC_IR_MakeArgPtr2(LocArr);

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
               #include "../IR/AddrList.hpp"
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
               #include "../IR/AddrList.hpp"
            }
         }

         //
         // cast constructors
         //
         #define GDCC_IR_AddrList(name) \
            Arg(Arg_##name const &arg) : a{ArgBase::name}, a##name(          arg ) {} \
            Arg(Arg_##name      &&arg) : a{ArgBase::name}, a##name(std::move(arg)) {}
         #include "../IR/AddrList.hpp"

         explicit Arg(IArchive &in);

         //
         // destructor
         //
         ~Arg()
         {
            switch(a)
            {
               #define GDCC_IR_AddrList(name) \
                  case ArgBase::name: a##name.~Arg_##name(); break;
               #include "../IR/AddrList.hpp"
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
               #include "../IR/AddrList.hpp"
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
               #include "../IR/AddrList.hpp"
            }
            else
               new((this->~Arg(), this)) Arg(std::move(arg));

            return *this;
         }

         //
         // cast assignments
         //
         #define GDCC_IR_AddrList(name) \
            Arg &operator = (Arg_##name const &arg) \
            { \
               if(a == ArgBase::name) a##name = arg; \
               else new((this->~Arg(), this)) Arg(arg); \
               return *this; \
            } \
            Arg &operator = (Arg_##name &&arg) \
            { \
               if(a == ArgBase::name) a##name = std::move(arg); \
               else new((this->~Arg(), this)) Arg(std::move(arg)); \
               return *this; \
            }
         #include "../IR/AddrList.hpp"

         //
         // getOffset
         //
         Arg getOffset(Core::FastU w) const
         {
            switch(a)
            {
               #define GDCC_IR_AddrList(name) \
                  case ArgBase::name: return a##name.getOffset(w);
               #include "../IR/AddrList.hpp"
            }

            return *this;
         }

         ArgBase a;

         union
         {
            #define GDCC_IR_AddrList(name) Arg_##name a##name;
            #include "../IR/AddrList.hpp"
         };
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
      bool operator == (Arg const &l, Arg const &r);
      bool operator != (Arg const &l, Arg const &r);

      #define GDCC_IR_AddrList(name) \
         inline OArchive &operator << (OArchive &out, Arg_##name const &in) \
            {return in.putIR(out);}
      #include "../IR/AddrList.hpp"

      OArchive &operator << (OArchive &out, Arg const &in);

      #define GDCC_IR_AddrList(name) \
         inline IArchive &operator >> (IArchive &in, Arg_##name &out) \
            {return out.getIR(in);}
      #include "../IR/AddrList.hpp"

      IArchive &operator >> (IArchive &in, Arg &out);
   }
}

#endif//GDCC__IR__Arg_H__

