//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C statements.
//
//-----------------------------------------------------------------------------

#include "CC/Factory.hpp"

#include "SR/Statement.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Factory::stCreate_Decl
   //
   SR::Statement::CRef Factory::stCreate_Decl(Core::Origin pos, Scope &)
   {
      return SR::StatementCreate_Empty(pos);
   }
}

// EOF

