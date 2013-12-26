//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree function handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Function_H__
#define GDCC__AST__Function_H__

#include "../Core/Counter.hpp"
#include "../Core/Number.hpp"
#include "../Core/String.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      enum class CallType;
      enum class Linkage;
      enum class ScriptType;

      class Exp;
   }

   namespace AST
   {
      class Statement;
      class Type;

      //
      // Function
      //
      class Function : public Core::Counter
      {
         GDCC_Core_CounterPreambleNoClone(
            GDCC::AST::Function, GDCC::Core::Counter);

      protected:
         using IRExpCPtr     = Core::CounterPtr<IR::Exp   const>;
         using StatementCPtr = Core::CounterPtr<Statement const>;
         using TypeCPtr      = Core::CounterPtr<Type      const>;

      public:
         virtual Core::String genLabel();

         IR::CallType   ctype;
         Core::String   glyph;
         Core::String   label;
         IR::Linkage    linka;
         Core::FastU    localArs;
         Core::FastU    localReg;
         Core::String   name;
         Core::FastU    param;
         TypeCPtr       retrn;
         StatementCPtr  stmnt;
         IR::ScriptType stype;
         IRExpCPtr      valueInt;
         Core::String   valueStr;

         bool           defin    : 1;
         bool           sflagNet : 1;
         bool           sflagClS : 1;


         static Ref Create(Core::String name, Core::String glyph);

      protected:
         Function(Core::String name, Core::String glyph);
         ~Function();
      };
   }
}

#endif//GDCC__AST__Function_H__

