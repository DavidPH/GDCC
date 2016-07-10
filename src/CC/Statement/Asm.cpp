//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C assembly statements.
//
//-----------------------------------------------------------------------------

#include "CC/Statement/Asm.hpp"

#include "AS/Macro.hpp"
#include "AS/Parse.hpp"

#include "Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Statement_Asm constructor
      //
      Statement_Asm::Statement_Asm(Labels &&labels_, Core::Origin pos_,
         Tokens &&tokens_) :
         Super{std::move(labels_), pos_},
         tokens{std::move(tokens_)}
      {
      }

      //
      // Statement_Asm::v_genStmnt
      //
      void Statement_Asm::v_genStmnt(SR::GenStmntCtx const &ctx) const
      {
         Core::ArrayTBuf   buf{tokens.data(), tokens.size()};
         Core::TokenStream in {&buf};
         AS::MacroMap      mac{};

         AS::ParseBlock({in, mac, ctx.prog}, ctx.block, Core::TOK_EOF);
      }

      //
      // StatementCreate_Asm
      //
      SR::Statement::CRef StatementCreate_Asm(
         SR::Statement::Labels &&labels, Core::Origin pos,
         Statement_Asm::Tokens &&tokens)
      {
         return Statement_Asm::Create(std::move(labels), pos, std::move(tokens));
      }
   }
}

// EOF

