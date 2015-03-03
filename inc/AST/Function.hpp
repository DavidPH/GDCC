//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
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
#include "../Core/NumberAlloc.hpp"
#include "../Core/StringGen.hpp"

#include <unordered_map>


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
      class Function;
      class Program;
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
         using LocalArr      = std::unordered_map<Core::FastU, Core::FastU>;
         using StatementCPtr = Core::CounterPtr<Statement const>;
         using TypeCPtr      = Core::CounterPtr<Type      const>;

      public:
         using LocalTmp = Core::NumberAlloc<Core::FastU>;


         virtual Core::String genLabel();

         // Generates an IR function with minimal data.
         void genFunctionDecl(IR::Program &prog);

         // Generates an IR function with all available data.
         void genFunctionDefn(IR::Program &prog);

         Core::String getLabelEnd();
         Core::String getLabelRes();
         Core::String getLabelTmp();

         void setAllocLoc(IR::Exp const *exp);

         // Generally not useful directly. Use AST::Temporary, instead.
         LocalTmp       localTmp;

         Core::FastU    allocLoc;
         IR::CallType   ctype;
         Core::String   glyph;
         Core::String   label;
         Core::String   labelEnd;
         Core::String   labelRes;
         Core::String   labelTmp;
         IR::Linkage    linka;
         LocalArr       localArr;
         Core::FastU    localAut;
         Core::FastU    localReg;
         Core::String   name;
         Core::FastU    param;
         Core::FastU    paramOpt;
         TypeCPtr       retrn;
         StatementCPtr  stmnt;
         IR::ScriptType stype;
         TypeCPtr       type;
         IRExpCPtr      valueInt;
         IRExpCPtr      valueLit;
         Core::String   valueStr;
         Core::String   warnUse;

         bool           defin    : 1;
         bool           sflagNet : 1;
         bool           sflagClS : 1;
         bool           used     : 1;
         bool           warnDone : 1;


         static Ref Create(Core::String name, Core::String glyph);

      protected:
         Function(Core::String name, Core::String glyph);
         virtual ~Function();

         Core::StringGen labeller;
      };
   }
}

#endif//GDCC__AST__Function_H__

