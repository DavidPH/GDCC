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

#include "IR/Arg.hpp"

#include "IR/IArchive.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // ArgPtr1 copy constructor
      //
      ArgPtr1::ArgPtr1(ArgPtr1 const &arg) : idx{new Arg(*arg.idx)}, off{arg.off}
      {
      }

      //
      // ArgPtr1 move constructor
      //
      ArgPtr1::ArgPtr1(ArgPtr1 &&arg) : idx{arg.idx}, off{arg.off}
      {
         arg.idx = nullptr;
      }

      //
      // ArgPtr1 constructor
      //
      ArgPtr1::ArgPtr1(Arg const &idx_, Exp const *off_) :
         idx{new Arg(idx_)}, off{off_}
      {
      }

      //
      // ArgPtr1 constructor
      //
      ArgPtr1::ArgPtr1(Arg &&idx_, Exp const *off_) :
         idx{new Arg(std::move(idx_))}, off{off_}
      {
      }

      //
      // ArgPtr1 constructor
      //
      ArgPtr1::ArgPtr1(IArchive &in) : off{GetIR(in, off)}
      {
         idx = new Arg(in);
      }

      //
      // ArgPtr1 destructor
      //
      ArgPtr1::~ArgPtr1()
      {
         delete idx;
      }

      //
      // ArgPtr1 copy assignment
      //
      ArgPtr1 &ArgPtr1::operator = (ArgPtr1 const &arg)
      {
         *idx = *arg.idx;
          off =  arg.off;
         return *this;
      }

      //
      // ArgPtr1 move assignment
      //
      ArgPtr1 &ArgPtr1::operator = (ArgPtr1 &&arg)
      {
         idx = arg.idx; arg.idx = nullptr;
         off = arg.off;
         return *this;
      }

      //
      // ArgPtr1::putIR
      //
      OArchive &ArgPtr1::putIR(OArchive &out) const
      {
         return out << off << *idx;
      }

      //
      // ArgPtr2 copy constructor
      //
      ArgPtr2::ArgPtr2(ArgPtr2 const &arg) :
         arr{Core::Array<Arg>::Cpy(arg.arr, arg.arr + 2)}, idx{arr + 1},
         off{arg.off}
      {
      }

      //
      // ArgPtr2 move constructor
      //
      ArgPtr2::ArgPtr2(ArgPtr2 &&arg) : arr{arg.arr}, idx{arr + 1}, off{arg.off}
      {
         arg.arr = nullptr;
         arg.idx = nullptr;
      }

      //
      // ArgPtr2 constructor
      //
      ArgPtr2::ArgPtr2(Arg const &arr_, Arg const &idx_, Exp const *off_) :
         arr{Core::Array<Arg>::Pak(arr_, idx_)}, idx{arr + 1}, off{off_}
      {
      }

      //
      // ArgPtr2 constructor
      //
      ArgPtr2::ArgPtr2(Arg const &arr_, Arg &&idx_, Exp const *off_) :
         arr{Core::Array<Arg>::Pak(arr_, std::move(idx_))}, idx{arr + 1},
         off{off_}
      {
      }

      //
      // ArgPtr2 constructor
      //
      ArgPtr2::ArgPtr2(Arg &&arr_, Arg const &idx_, Exp const *off_) :
         arr{Core::Array<Arg>::Pak(std::move(arr_), idx_)}, idx{arr + 1},
         off{off_}
      {
      }

      //
      // ArgPtr2 constructor
      //
      ArgPtr2::ArgPtr2(Arg &&arr_, Arg &&idx_, Exp const *off_) :
         arr{Core::Array<Arg>::Pak(std::move(arr_), std::move(idx_))},
         idx{arr + 1}, off{off_}
      {
      }

      //
      // ArgPtr2 constructor
      //
      ArgPtr2::ArgPtr2(IArchive &in) : off{GetIR(in, off)}
      {
         idx = (arr = Core::Array<Arg>::Pak(in, in)) + 1;
      }

      //
      // ArgPtr2 destructor
      //
      ArgPtr2::~ArgPtr2()
      {
         if(arr) Core::Array<Arg>::Del(arr, arr + 2);
      }

      //
      // ArgPtr2 copy assignment
      //
      ArgPtr2 &ArgPtr2::operator = (ArgPtr2 const &arg)
      {
         *arr = *arg.arr;
         *idx = *arg.idx;
          off =  arg.off;
         return *this;
      }

      //
      // ArgPtr2 move assignment
      //
      ArgPtr2 &ArgPtr2::operator = (ArgPtr2 &&arg)
      {
         arr = arg.arr; arg.arr = nullptr;
         idx = arg.idx; arg.idx = nullptr;
         off = arg.off;
         return *this;
      }

      //
      // ArgPtr2::putIR
      //
      OArchive &ArgPtr2::putIR(OArchive &out) const
      {
         return out << off << *arr << *idx;
      }

      //
      // Arg_Cpy constructor
      //
      Arg_Cpy::Arg_Cpy(IArchive &in) : value{GetIR(in, value)}
      {
      }

      //
      // Arg_Cpy::getIR
      //
      IArchive &Arg_Cpy::getIR(IArchive &in)
      {
         return in >> value;
      }

      //
      // Arg_Cpy::putIR
      //
      OArchive &Arg_Cpy::putIR(OArchive &out) const
      {
         return out << value;
      }

      //
      // Arg_Lit constructor
      //
      Arg_Lit::Arg_Lit(IArchive &in) : value{GetIR(in, value)}
      {
      }

      //
      // Arg constructor
      //
      Arg::Arg(IArchive &in)
      {
         switch(in >> a, a)
         {
            #define GDCC_IR_AddrList(name) \
               case ArgBase::name: new(&a##name) Arg_##name(in); return;
            #include "IR/AddrList.hpp"
         }
      }

      //
      // operator OArchive << Arg
      //
      OArchive &operator << (OArchive &out, Arg const &in)
      {
         out << in.a;
         switch(in.a)
         {
            #define GDCC_IR_AddrList(name) \
               case ArgBase::name: return in.a##name.putIR(out);
            #include "IR/AddrList.hpp"
         }
      }

      //
      // operator IArchive >> Arg
      //
      IArchive &operator >> (IArchive &in, Arg &out)
      {
         switch(GetIR<ArgBase>(in))
         {
            #define GDCC_IR_AddrList(name) \
               case ArgBase::name: out = Arg_##name(in); return in;
            #include "IR/AddrList.hpp"
         }
      }
   }
}

// EOF

