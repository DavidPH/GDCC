//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2025 David Hill
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

#include "../IR/Exp.hpp"

#include "../Target/Addr.hpp"


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
      Arg_##name &operator ++ () {++off; return *this;} \
      Arg_##name &operator -- () {--off; return *this;} \
      \
      Arg_##name operator + (Core::FastU i) const \
         {return {size, *idx, off + i};} \
      Arg_##name operator - (Core::FastU i) const \
         {return {size, *idx, off - i};} \
      \
      bool operator == (Arg_##name const &arg) const \
         {return ArgPtr1::operator == (arg);} \
      \
      Arg_##name &operator = (Arg_##name const &) = default; \
      Arg_##name &operator = (Arg_##name &&) = default; \
      \
      Arg_##name &operator += (Core::FastU i) {return off += i, *this;} \
      Arg_##name &operator -= (Core::FastU i) {return off -= i, *this;} \
      \
      Arg_##name getOffset(Core::FastU w) const {return {size, *idx, off + w};} \
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
      Arg_##name &operator ++ () {++off; return *this;} \
      Arg_##name &operator -- () {++off; return *this;} \
      \
      Arg_##name operator + (Core::FastU i) const \
         {return {size, *arr, *idx, off + i};} \
      Arg_##name operator - (Core::FastU i) const \
         {return {size, *arr, *idx, off - i};} \
      \
      bool operator == (Arg_##name const &arg) const \
         {return ArgPtr2::operator == (arg);} \
      \
      Arg_##name &operator = (Arg_##name const &) = default; \
      Arg_##name &operator = (Arg_##name &&) = default; \
      \
      Arg_##name &operator += (Core::FastU i) {return off += i, *this;} \
      Arg_##name &operator -= (Core::FastU i) {return off -= i, *this;} \
      \
      Arg_##name getOffset(Core::FastU w) const {return {size, *arr, *idx, off + w};} \
   }


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::IR
{
   bool operator == (Arg const &l, Arg const &r);

   //
   // ArgPart
   //
   class ArgPart
   {
   public:
      explicit ArgPart(Core::FastU size_) : size{size_} {}
      explicit ArgPart(IArchive &in);

      IArchive &getIR(IArchive &in);

      OArchive &putIR(OArchive &out) const;

      Core::FastU size;

   protected:
      ArgPart(ArgPart const &) = default;
      ArgPart(ArgPart &&) = default;

      bool operator == (ArgPart const &arg) const
         {return size == arg.size;}

      ArgPart &operator = (ArgPart const &) = default;
      ArgPart &operator = (ArgPart &&) = default;
   };

   //
   // ArgPtr1
   //
   class ArgPtr1 : public ArgPart
   {
   public:
      ArgPtr1(Core::FastU size, Arg const &idx);
      ArgPtr1(Core::FastU size, Arg &&idx);
      ArgPtr1(Core::FastU size, Arg const &idx, Core::FastU off);
      ArgPtr1(Core::FastU size, Arg &&idx, Core::FastU off);
      explicit ArgPtr1(IArchive &in);

      IArchive &getIR(IArchive &in);

      OArchive &putIR(OArchive &out) const;

      Arg        *idx;
      Core::FastU off;

   protected:
      ArgPtr1(ArgPtr1 const &arg);
      ArgPtr1(ArgPtr1 &&arg);
      ~ArgPtr1();

      bool operator == (ArgPtr1 const &arg) const
         {return ArgPart::operator == (arg) &&
             off == arg.off && *idx == *arg.idx;}

      ArgPtr1 &operator = (ArgPtr1 const &arg);
      ArgPtr1 &operator = (ArgPtr1 &&arg);
   };

   //
   // ArgPtr2
   //
   class ArgPtr2 : public ArgPart
   {
   public:
      ArgPtr2(Core::FastU size, Arg const &arr, Arg const &idx);
      ArgPtr2(Core::FastU size, Arg const &arr, Arg &&idx);
      ArgPtr2(Core::FastU size, Arg &&arr, Arg const &idx);
      ArgPtr2(Core::FastU size, Arg &&arr, Arg &&idx);
      ArgPtr2(Core::FastU size, Arg const &arr, Arg const &idx, Core::FastU off);
      ArgPtr2(Core::FastU size, Arg const &arr, Arg &&idx, Core::FastU off);
      ArgPtr2(Core::FastU size, Arg &&arr, Arg const &idx, Core::FastU off);
      ArgPtr2(Core::FastU size, Arg &&arr, Arg &&idx, Core::FastU off);
      explicit ArgPtr2(IArchive &in);

      IArchive &getIR(IArchive &in);

      OArchive &putIR(OArchive &out) const;

      Arg        *arr;
      Arg        *idx;
      Core::FastU off;

   protected:
      ArgPtr2(ArgPtr2 const &arg);
      ArgPtr2(ArgPtr2 &&arg);
      ~ArgPtr2();

      bool operator == (ArgPtr2 const &arg) const
         {return ArgPart::operator == (arg) &&
             off == arg.off && *idx == *arg.idx && *arr == *arg.arr;}

      ArgPtr2 &operator = (ArgPtr2 const &arg);
      ArgPtr2 &operator = (ArgPtr2 &&arg);
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
   class Arg_Cpy : public ArgPart
   {
   public:
      Arg_Cpy() : ArgPart{0}, value{0} {}
      Arg_Cpy(Core::FastU size_, Core::FastU value_) : ArgPart{size_}, value{value_} {}
      explicit Arg_Cpy(IArchive &in);

      bool operator == (Arg_Cpy const &arg) const
         {return ArgPart::operator == (arg) && value == arg.value;}

      IArchive &getIR(IArchive &in);

      Arg_Cpy getOffset(Core::FastU) const {return *this;}

      OArchive &putIR(OArchive &out) const;

      Core::FastU value;
   };

   //
   // Arg_Lit
   //
   // Only valid as a source.
   //
   class Arg_Lit : public ArgPart
   {
   public:
      Arg_Lit(Core::FastU size_, Exp const *value_) :
         ArgPart{size_}, value{value_}, off{0} {}
      Arg_Lit(Core::FastU size_, Exp const *value_, Core::FastU off_) :
         ArgPart{size_}, value{value_}, off{off_} {}
      explicit Arg_Lit(IArchive &in);

      bool operator == (Arg_Lit const &arg) const
         {return ArgPart::operator == (arg) &&
             off == arg.off && *value == *arg.value;}

      IArchive &getIR(IArchive &in);

      Arg_Lit getOffset(Core::FastU w) const {return {size, value, off + w};}

      OArchive &putIR(OArchive &out) const;

      Exp::CRef   value;
      Core::FastU off;
   };

   //
   // Arg_Nul
   //
   // Only valid as a destination.
   //
   class Arg_Nul : public ArgPart
   {
   public:
      using ArgPart::ArgPart;

      bool operator == (Arg_Nul const &arg) const
         {return ArgPart::operator == (arg);}

      Arg_Nul getOffset(Core::FastU) const {return *this;}
   };

   //
   // Arg_Stk
   //
   class Arg_Stk : public ArgPart
   {
   public:
      using ArgPart::ArgPart;

      bool operator == (Arg_Stk const &arg) const
         {return ArgPart::operator == (arg);}

      Arg_Stk getOffset(Core::FastU) const {return *this;}
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
   // Arg_HubArs
   //
   GDCC_IR_MakeArgPtr1(HubArs);

   //
   // Arg_HubReg
   //
   GDCC_IR_MakeArgPtr1(HubReg);

   //
   // Arg_LocReg
   //
   GDCC_IR_MakeArgPtr1(LocReg);

   //
   // Arg_ModArs
   //
   GDCC_IR_MakeArgPtr1(ModArs);

   //
   // Arg_ModReg
   //
   GDCC_IR_MakeArgPtr1(ModReg);

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
   // Arg_GblArr
   //
   GDCC_IR_MakeArgPtr2(GblArr);

   //
   // Arg_HubArr
   //
   GDCC_IR_MakeArgPtr2(HubArr);

   //
   // Arg_LocArr
   //
   GDCC_IR_MakeArgPtr2(LocArr);

   //
   // Arg_ModArr
   //
   GDCC_IR_MakeArgPtr2(ModArr);

   //
   // Arg_StrArr
   //
   GDCC_IR_MakeArgPtr2(StrArr);

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
            #define GDCC_Target_AddrList(name) case ArgBase::name: \
               new(&a##name) Arg_##name(arg.a##name); break;
            #include "../Target/AddrList.hpp"
         }
      }

      //
      // move constructor
      //
      Arg(Arg &&arg) : a{arg.a}
      {
         switch(a)
         {
            #define GDCC_Target_AddrList(name) case ArgBase::name: \
               new(&a##name) Arg_##name(std::move(arg.a##name)); break;
            #include "../Target/AddrList.hpp"
         }
      }

      //
      // cast constructors
      //
      #define GDCC_Target_AddrList(name) \
         Arg(Arg_##name const &arg) : a{ArgBase::name}, a##name(          arg ) {} \
         Arg(Arg_##name      &&arg) : a{ArgBase::name}, a##name(std::move(arg)) {}
      #include "../Target/AddrList.hpp"

      explicit Arg(IArchive &in);

      //
      // destructor
      //
      ~Arg()
      {
         switch(a)
         {
            #define GDCC_Target_AddrList(name) \
               case ArgBase::name: a##name.~Arg_##name(); break;
            #include "../Target/AddrList.hpp"
         }
      }

      //
      // copy assignment
      //
      Arg &operator = (Arg const &arg)
      {
         if(arg.a == a) switch(a)
         {
            #define GDCC_Target_AddrList(name) \
               case ArgBase::name: a##name = arg.a##name; break;
            #include "../Target/AddrList.hpp"
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
            #define GDCC_Target_AddrList(name) \
               case ArgBase::name: a##name = std::move(arg.a##name); break;
            #include "../Target/AddrList.hpp"
         }
         else
            new((this->~Arg(), this)) Arg(std::move(arg));

         return *this;
      }

      //
      // cast assignments
      //
      #define GDCC_Target_AddrList(name) \
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
      #include "../Target/AddrList.hpp"

      //
      // getOffset
      //
      Arg getOffset(Core::FastU w) const
      {
         switch(a)
         {
            #define GDCC_Target_AddrList(name) \
               case ArgBase::name: return a##name.getOffset(w);
            #include "../Target/AddrList.hpp"
         }

         return *this;
      }

      //
      // getSize
      //
      Core::FastU getSize() const
      {
         switch(a)
         {
            #define GDCC_Target_AddrList(name) \
               case ArgBase::name: return a##name.size;
            #include "../Target/AddrList.hpp"
         }

         return 0;
      }

      ArgBase a;

      union
      {
         #define GDCC_Target_AddrList(name) Arg_##name a##name;
         #include "../Target/AddrList.hpp"
      };
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   bool operator == (Arg const &l, Arg const &r);
   bool operator != (Arg const &l, Arg const &r);

   #define GDCC_Target_AddrList(name) \
      inline OArchive &operator << (OArchive &out, Arg_##name const &in) \
         {return in.putIR(out);}
   #include "../Target/AddrList.hpp"

   OArchive &operator << (OArchive &out, Arg const &in);

   #define GDCC_Target_AddrList(name) \
      inline IArchive &operator >> (IArchive &in, Arg_##name &out) \
         {return out.getIR(in);}
   #include "../Target/AddrList.hpp"

   IArchive &operator >> (IArchive &in, Arg &out);

   template<typename T>
   constexpr ArgBase GetArgBase();

   #define GDCC_Target_AddrList(name) \
      template<> \
      constexpr ArgBase GetArgBase<Arg_##name>() {return ArgBase::name;}
   #include "../Target/AddrList.hpp"
}

#endif//GDCC__IR__Arg_H__

