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

#include "Arg.hpp"


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
      ArgPtr1::ArgPtr1(Arg const &idx_, Exp *off_) : idx{new Arg(idx_)}, off{off_}
      {
      }

      //
      // ArgPtr1 constructor
      //
      ArgPtr1::ArgPtr1(Arg &&idx_, Exp *off_) : idx{new Arg(std::move(idx_))}, off{off_}
      {
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
      // ArgPtr2 copy constructor
      //
      ArgPtr2::ArgPtr2(ArgPtr2 const &arg) : arr{Array<Arg>::Cpy(arg.arr, 2)},
         idx{arr + 1}, off{arg.off}
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
      ArgPtr2::ArgPtr2(Arg const &arr_, Arg const &idx_, Exp *off_) :
         arr{Array<Arg>::Pak(arr_, idx_)}, idx{arr + 1}, off{off_}
      {
      }

      //
      // ArgPtr2 constructor
      //
      ArgPtr2::ArgPtr2(Arg const &arr_, Arg &&idx_, Exp *off_) :
         arr{Array<Arg>::Pak(arr_, std::move(idx_))}, idx{arr + 1}, off{off_}
      {
      }

      //
      // ArgPtr2 constructor
      //
      ArgPtr2::ArgPtr2(Arg &&arr_, Arg const &idx_, Exp *off_) :
         arr{Array<Arg>::Pak(std::move(arr_), idx_)}, idx{arr + 1}, off{off_}
      {
      }

      //
      // ArgPtr2 constructor
      //
      ArgPtr2::ArgPtr2(Arg &&arr_, Arg &&idx_, Exp *off_) :
         arr{Array<Arg>::Pak(std::move(arr_), std::move(idx_))}, idx{arr + 1}, off{off_}
      {
      }

      //
      // ArgPtr2 destructor
      //
      ArgPtr2::~ArgPtr2()
      {
         if(arr) Array<Arg>::Del(arr, 2);
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
   }
}

// EOF

