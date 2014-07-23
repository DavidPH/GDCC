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

#include "AST/Arg.hpp"

#include "AST/Exp.hpp"
#include "AST/Type.hpp"


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
      Arg::Arg(Type const *type_) : type{type_}, data{nullptr}
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
         type{type_->getTypeQual(TypeQual(IR::AddrSpace(base, Core::STR_)))},
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

