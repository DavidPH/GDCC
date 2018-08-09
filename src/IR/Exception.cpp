//-----------------------------------------------------------------------------
//
// Copyright (C) 2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// IR exceptions.
//
//-----------------------------------------------------------------------------

#include "IR/Exception.hpp"

#include "IR/Code.hpp"
#include "IR/Statement.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::IR
{
   //
   // ExceptCode
   //
   class ExceptCode : public Core::Exception
   {
   public:
      ExceptCode(Core::Origin pos_, Code code_, char const *msg_) noexcept :
         Exception{pos_}, msg{msg_}, code{code_} {}

   protected:
      virtual char const *whatGen() const noexcept;

      char const *const msg;
      Code        const code;
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   //
   // ExceptCode::whatGen
   //
   char const *ExceptCode::whatGen() const noexcept
   {
      std::ostringstream oss;
      putOrigin(oss) << code << ": " << msg;
      return whatSet(oss.str());
   }

   //
   // ErrorCode
   //
   void ErrorCode(Core::Origin pos, Code code, char const *msg)
      {throw ExceptCode(pos, code, msg);}
   void ErrorCode(Statement *stmnt, char const *msg)
      {throw ExceptCode(stmnt->pos, stmnt->code, msg);}
}

// EOF

