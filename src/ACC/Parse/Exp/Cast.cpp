//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS cast-expression parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "AST/Attribute.hpp"
#include "AST/Exp.hpp"
#include "AST/Object.hpp"
#include "AST/Type.hpp"

#include "CC/Exp/Assign.hpp"
#include "CC/Exp/Init.hpp"
#include "CC/Init.hpp"
#include "CC/Scope/Global.hpp"
#include "CC/Scope/Local.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // GetExp_Cast
      //
      AST::Exp::CRef GetExp_Cast(ParserCtx const &ctx, CC::Scope &scope)
      {
         if(IsExp_Cast(ctx, scope))
         {
            // (
            auto pos = ctx.in.get().pos;

            // type-name
            auto type = GetType(ctx, scope);

            // )
            if(!ctx.in.drop(Core::TOK_ParenC))
               throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);

            return CC::ExpCreate_Cst(type, GetExp_Cast(ctx, scope), pos);
         }

         return GetExp_Unar(ctx, scope);
      }

      //
      // IsExp_Cast
      //
      bool IsExp_Cast(ParserCtx const &ctx, CC::Scope &scope)
      {
         if(ctx.in.drop(Core::TOK_ParenO))
         {
            bool res = IsType(ctx, scope);
            ctx.in.unget();
            return res;
         }

         return false;
      }
   }
}

// EOF

