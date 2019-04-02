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

#include "SR/Exp.hpp"
#include "SR/Statement/Empty.hpp"
#include "SR/Statement/Exp.hpp"
#include "SR/Statement/Multi.hpp"


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
      return SR::Statement_Empty::Create({}, pos);
   }

   //
   // Factory::stCreate_Empty
   //
   SR::Statement::CRef Factory::stCreate_Empty(SR::Statement::Labels &&labels,
      Core::Origin pos)
   {
      return SR::Statement_Empty::Create(std::move(labels), pos);
   }

   //
   // Factory::stCreate_Exp
   //
   SR::Statement::CRef Factory::stCreate_Exp(SR::Statement::Labels &&labels,
      Exp const *exp)
   {
      return stCreate_Exp(std::move(labels), exp->pos, exp);
   }

   //
   // Factory::stCreate_Exp
   //
   SR::Statement::CRef Factory::stCreate_Exp(SR::Statement::Labels &&labels,
      Core::Origin pos, Exp const *exp)
   {
      return SR::Statement_Exp::Create(std::move(labels), pos, exp);
   }

   //
   // Factory::stCreate_Multi
   //
   SR::Statement::CRef Factory::stCreate_Multi(SR::Statement::Labels &&labels,
      Core::Origin pos, Core::Array<SR::Statement::CRef> &&sts)
   {
      return SR::Statement_Multi::Create(std::move(labels), pos, std::move(sts));
   }
}

// EOF

