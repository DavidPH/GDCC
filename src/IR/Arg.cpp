//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
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
#include "IR/OArchive.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   //
   // ArgPtr1 copy constructor
   //
   ArgPtr1::ArgPtr1(ArgPtr1 const &arg) :
      ArgPart{arg}, idx{new Arg(*arg.idx)}, off{arg.off}
   {
   }

   //
   // ArgPtr1 move constructor
   //
   ArgPtr1::ArgPtr1(ArgPtr1 &&arg) :
      ArgPart{std::move(arg)}, idx{arg.idx}, off{arg.off}
   {
      arg.idx = nullptr;
   }

   //
   // ArgPtr1 constructor
   //
   ArgPtr1::ArgPtr1(Core::FastU size_, Arg const &idx_) :
      ArgPart{size_}, idx{new Arg(idx_)}, off{0}
   {
   }

   //
   // ArgPtr1 constructor
   //
   ArgPtr1::ArgPtr1(Core::FastU size_, Arg &&idx_) :
      ArgPart{size_}, idx{new Arg(std::move(idx_))}, off{0}
   {
   }

   //
   // ArgPtr1 constructor
   //
   ArgPtr1::ArgPtr1(Core::FastU size_, Arg const &idx_, Core::FastU off_) :
      ArgPart{size_}, idx{new Arg(idx_)}, off{off_}
   {
   }

   //
   // ArgPtr1 constructor
   //
   ArgPtr1::ArgPtr1(Core::FastU size_, Arg &&idx_, Core::FastU off_) :
      ArgPart{size_}, idx{new Arg(std::move(idx_))}, off{off_}
   {
   }

   //
   // ArgPtr1 constructor
   //
   ArgPtr1::ArgPtr1(IArchive &in) :
      ArgPart{in}, off{GetIR(in, off)}
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
      ArgPart::operator = (arg);
      *idx = *arg.idx;
       off =  arg.off;
      return *this;
   }

   //
   // ArgPtr1 move assignment
   //
   ArgPtr1 &ArgPtr1::operator = (ArgPtr1 &&arg)
   {
      ArgPart::operator = (std::move(arg));
      idx = arg.idx; arg.idx = nullptr;
      off = arg.off;
      return *this;
   }

   //
   // ArgPtr1::putIR
   //
   OArchive &ArgPtr1::putIR(OArchive &out) const
   {
      return ArgPart::putIR(out) << off << *idx;
   }

   //
   // ArgPtr2 copy constructor
   //
   ArgPtr2::ArgPtr2(ArgPtr2 const &arg) :
      ArgPart{arg},
      arr{Core::Array<Arg>::Cpy(arg.arr, arg.arr + 2)}, idx{arr + 1},
      off{arg.off}
   {
   }

   //
   // ArgPtr2 move constructor
   //
   ArgPtr2::ArgPtr2(ArgPtr2 &&arg) :
      ArgPart{std::move(arg)}, arr{arg.arr}, idx{arr + 1}, off{arg.off}
   {
      arg.arr = nullptr;
      arg.idx = nullptr;
   }

   //
   // ArgPtr2 constructor
   //
   ArgPtr2::ArgPtr2(Core::FastU size_, Arg const &arr_, Arg const &idx_) :
      ArgPart{size_},
      arr{Core::Array<Arg>::Pak(arr_, idx_)}, idx{arr + 1}, off{0}
   {
   }

   //
   // ArgPtr2 constructor
   //
   ArgPtr2::ArgPtr2(Core::FastU size_, Arg const &arr_, Arg &&idx_) :
      ArgPart{size_},
      arr{Core::Array<Arg>::Pak(arr_, std::move(idx_))}, idx{arr + 1}, off{0}
   {
   }

   //
   // ArgPtr2 constructor
   //
   ArgPtr2::ArgPtr2(Core::FastU size_, Arg &&arr_, Arg const &idx_) :
      ArgPart{size_},
      arr{Core::Array<Arg>::Pak(std::move(arr_), idx_)}, idx{arr + 1}, off{0}
   {
   }

   //
   // ArgPtr2 constructor
   //
   ArgPtr2::ArgPtr2(Core::FastU size_, Arg &&arr_, Arg &&idx_) :
      ArgPart{size_},
      arr{Core::Array<Arg>::Pak(std::move(arr_), std::move(idx_))},
      idx{arr + 1}, off{0}
   {
   }

   //
   // ArgPtr2 constructor
   //
   ArgPtr2::ArgPtr2(Core::FastU size_, Arg const &arr_, Arg const &idx_, Core::FastU off_) :
      ArgPart{size_},
      arr{Core::Array<Arg>::Pak(arr_, idx_)}, idx{arr + 1}, off{off_}
   {
   }

   //
   // ArgPtr2 constructor
   //
   ArgPtr2::ArgPtr2(Core::FastU size_, Arg const &arr_, Arg &&idx_, Core::FastU off_) :
      ArgPart{size_},
      arr{Core::Array<Arg>::Pak(arr_, std::move(idx_))}, idx{arr + 1},
      off{off_}
   {
   }

   //
   // ArgPtr2 constructor
   //
   ArgPtr2::ArgPtr2(Core::FastU size_, Arg &&arr_, Arg const &idx_, Core::FastU off_) :
      ArgPart{size_},
      arr{Core::Array<Arg>::Pak(std::move(arr_), idx_)}, idx{arr + 1},
      off{off_}
   {
   }

   //
   // ArgPtr2 constructor
   //
   ArgPtr2::ArgPtr2(Core::FastU size_, Arg &&arr_, Arg &&idx_, Core::FastU off_) :
      ArgPart{size_},
      arr{Core::Array<Arg>::Pak(std::move(arr_), std::move(idx_))},
      idx{arr + 1}, off{off_}
   {
   }

   //
   // ArgPtr2 constructor
   //
   ArgPtr2::ArgPtr2(IArchive &in) : ArgPart{in}, off{GetIR(in, off)}
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
      ArgPart::operator = (arg);
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
      ArgPart::operator = (std::move(arg));
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
      return ArgPart::putIR(out) << off << *arr << *idx;
   }

   //
   // Arg_Cpy constructor
   //
   Arg_Cpy::Arg_Cpy(IArchive &in) : ArgPart{in}, value{GetIR(in, value)}
   {
   }

   //
   // Arg_Cpy::getIR
   //
   IArchive &Arg_Cpy::getIR(IArchive &in)
   {
      return ArgPart::getIR(in) >> value;
   }

   //
   // Arg_Cpy::putIR
   //
   OArchive &Arg_Cpy::putIR(OArchive &out) const
   {
      return ArgPart::putIR(out) << value;
   }

   //
   // Arg_Lit constructor
   //
   Arg_Lit::Arg_Lit(IArchive &in) :
      ArgPart{in}, value{GetIR(in, value)}, off{GetIR(in, off)}
   {
   }

   //
   // Arg_Lit::getIR
   //
   IArchive &Arg_Lit::getIR(IArchive &in)
   {
      return ArgPart::getIR(in) >> value >> off;
   }

   //
   // Arg_Lit::putIR
   //
   OArchive &Arg_Lit::putIR(OArchive &out) const
   {
      return ArgPart::putIR(out) << value << off;
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
   // operator Arg == Arg
   //
   bool operator == (Arg const &l, Arg const &r)
   {
      if(l.a == r.a) switch(l.a)
      {
         #define GDCC_IR_AddrList(name) \
            case ArgBase::name: return l.a##name == r.a##name;
         #include "IR/AddrList.hpp"
      }

      return false;
   }

   //
   // operator Arg != Arg
   //
   bool operator != (Arg const &l, Arg const &r)
   {
      return !(l == r);
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

      std::cerr << "invalid enum GDCC::IR::Arg\n";
      throw EXIT_FAILURE;
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

      std::cerr << "invalid enum GDCC::IR::Arg\n";
      throw EXIT_FAILURE;
   }
}

// EOF

