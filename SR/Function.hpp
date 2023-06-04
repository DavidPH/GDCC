//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2023 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation function handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__Function_H__
#define GDCC__SR__Function_H__

#include "../SR/Types.hpp"

#include "../Core/Array.hpp"
#include "../Core/Counter.hpp"
#include "../Core/Number.hpp"
#include "../Core/NumberAlloc.hpp"
#include "../Core/StringGen.hpp"

#include <unordered_map>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::SR
{
   //
   // Function
   //
   class Function : public Core::Counter
   {
      GDCC_Core_CounterPreambleNoClone(GDCC::SR::Function, GDCC::Core::Counter);

   protected:
      using IRExpCPtr     = Core::CounterPtr<IR::Exp   const>;
      using LocalArr      = std::unordered_map<Core::FastU, Core::FastU>;
      using ScriptType    = Core::Array<Core::String>;
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

      void setAllocAut(IR::Exp const *exp);

      // Generally not useful directly. Use AST::Temporary, instead.
      LocalTmp       localTmp;

      Core::FastU    allocAut;
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
      Core::FastU    paramOpt;
      TypeCPtr       retrn;
      StatementCPtr  stmnt;
      ScriptType     stype;
      TypeCPtr       type;
      IRExpCPtr      valueInt;
      IRExpCPtr      valueLit;
      Core::String   valueStr;
      Core::String   warnUse;

      bool           declAuto    : 1;
      bool           defin       : 1;
      bool           delay       : 1;
      bool           noInitDelay : 1;
      bool           used        : 1;
      bool           warnDone    : 1;


      static Ref Create(Core::String name, Core::String glyph);

   protected:
      Function(Core::String name, Core::String glyph);
      virtual ~Function();

      Core::StringGen labeller;
   };
}

#endif//GDCC__SR__Function_H__

