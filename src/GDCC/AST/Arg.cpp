//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree storage arguments.
//
//-----------------------------------------------------------------------------

#include "Arg.hpp"

#include "Exp.hpp"
#include "Type.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Arg copy constructor
      //
      Arg::Arg(Arg const &arg) : type{arg.type}, data{arg.data}
      {
      }

      //
      // Arg constructor
      //
      Arg::Arg(Type const *type_, Exp const *data_) : type{type_}, data{data_}
      {
      }

      //
      // Arg constructor
      //
      Arg::Arg(Type const *type_, IR::AddrBase base, Exp const *data_) :
         type{type_->getTypeQual(TypeQual(IR::AddrSpace(base, STR_)))},
         data{data_}
      {
      }

      //
      // Arg destructor
      //
      Arg::~Arg()
      {
      }
   }
}

// EOF

