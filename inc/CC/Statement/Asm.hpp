//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C assembly statements.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Statement__Asm_H__
#define GDCC__CC__Statement__Asm_H__

#include "../../CC/Statement.hpp"

#include "../../SR/Statement.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // Statement_Asm
   //
   class Statement_Asm : public SR::Statement
   {
      GDCC_Core_CounterPreamble(
         GDCC::CC::Statement_Asm, GDCC::SR::Statement);

   public:
      using Tokens = Core::Array<Core::Token>;


      Tokens const tokens;


      // Create
      static CRef Create(Labels &&labels, Core::Origin pos, Tokens &&tokens)
         {return CRef(new This(std::move(labels), pos, std::move(tokens)));}

   protected:
      Statement_Asm(Labels &&labels, Core::Origin pos, Tokens &&tokens);

      virtual void v_genStmnt(SR::GenStmntCtx const &ctx) const;

      virtual bool v_isEffect() const {return true;}
      virtual bool v_isNoAuto() const {return true;}
   };
}

#endif//GDCC__CC__Statement__Asm_H__

