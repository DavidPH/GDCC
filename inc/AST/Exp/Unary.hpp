//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree unary expression base.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Exp__Unary_H__
#define GDCC__AST__Exp__Unary_H__

#include "../../AST/Exp.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_AST_Exp_UnaryCreator
//
// Defines a basic constructor and static Create function for unary expression
// classes.
//
#define GDCC_AST_Exp_UnaryCreator(class) \
public: \
   static ::GDCC::AST::Exp::CRef Create(::GDCC::AST::Type const *t, \
      ::GDCC::AST::Exp const *e, ::GDCC::Core::Origin pos) \
   { \
      return static_cast<AST::Exp::CRef>(new class(t, e, pos)); \
   } \
   \
protected: \
   class(::GDCC::AST::Type const *t, ::GDCC::AST::Exp const *e, \
      ::GDCC::Core::Origin pos_) : Super{t, e, pos_} {}


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp_Unary
      //
      class Exp_Unary : public Exp
      {
         GDCC_Core_CounterPreambleAbstract(
            GDCC::AST::Exp_Unary, GDCC::AST::Exp);

      public:
         Exp::CRef const exp;
         TypeCRef  const type;

      protected:
         Exp_Unary(Type const *t, Exp const *e, Core::Origin pos);
         virtual ~Exp_Unary();

         virtual TypeCRef v_getType() const;

         virtual bool v_isEffect() const;

         virtual bool v_isIRExp() const;
      };
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      // Returns true if only evaluating for side effects.
      bool GenStmntNul(Exp_Unary const *exp, IR::Block &block, Function *fn,
         Arg const &dst);
   }
}

#endif//GDCC__AST__Exp__Binary_H__

