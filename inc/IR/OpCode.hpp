//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation operation-code handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__OpCode_H__
#define GDCC__IR__OpCode_H__

#include "../IR/Code.hpp"

#include "../Core/Number.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      //
      // OpCode
      //
      class OpCode
      {
      public:
         OpCode() = default;
         OpCode(Code code_, Core::FastU size_) : code{code_}, size{size_} {}

         Code        code;
         Core::FastU size;
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
      OArchive &operator << (OArchive &out, OpCode in);
      std::ostream &operator << (std::ostream &out, OpCode in);

      IArchive &operator >> (IArchive &in, OpCode &out);
   }
}

#endif//GDCC__IR__OpCode_H__

