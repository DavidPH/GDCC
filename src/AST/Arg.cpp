//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree storage arguments.
//
//-----------------------------------------------------------------------------

#include "AST/Arg.hpp"

#include "AST/Exp.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/Arg.hpp"
#include "IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GenGetIRArg_ArgPtr2
//
#define GenGetIRArg_ArgPtr2(ArgT) \
   template<> \
   inline ArgT GetIRArg<ArgT>(AST::Arg const &arg, IR::Program &prog) \
   { \
      IR::Glyph glyph{prog, arg.type->getQualAddr().name}; \
      auto arr = IR::ExpCreate_Glyph(glyph, arg.data->pos); \
      \
      return ArgT(IR::Arg_Lit(arr), arg.data->getArg().getIRArg(prog)); \
   }

//
// GenIsIRArg_ArgPtr2
//
#define GenIsIRArg_ArgPtr2(ArgT) \
   template<> \
   inline bool IsIRArg<ArgT>(AST::Arg const &arg) \
   { \
      return arg.data->getArg().isIRArg(); \
   }


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // GetIRArg
      //
      template<typename ArgT>
      static ArgT GetIRArg(AST::Arg const &arg, IR::Program &prog)
      {
         return ArgT(arg.data->getArg().getIRArg(prog));
      }
      GenGetIRArg_ArgPtr2(IR::Arg_GblArr)
      GenGetIRArg_ArgPtr2(IR::Arg_LocArr)
      GenGetIRArg_ArgPtr2(IR::Arg_MapArr)
      GenGetIRArg_ArgPtr2(IR::Arg_StrArr)
      GenGetIRArg_ArgPtr2(IR::Arg_WldArr)

      // GetIRArg<IR::Arg_Cpy>
      template<>
      IR::Arg_Cpy GetIRArg<IR::Arg_Cpy>(AST::Arg const &, IR::Program &)
         {return IR::Arg_Cpy();}

      // GetIRArg<IR::Arg_Lit>
      template<>
      IR::Arg_Lit GetIRArg<IR::Arg_Lit>(AST::Arg const &arg, IR::Program &)
         {return IR::Arg_Lit(arg.data->getIRExp());}

      // GetIRArg<IR::Arg_Nul>
      template<>
      IR::Arg_Nul GetIRArg<IR::Arg_Nul>(AST::Arg const &, IR::Program &)
         {return IR::Arg_Nul();}

      // GetIRArg<IR::Arg_Stk>
      template<>
      IR::Arg_Stk GetIRArg<IR::Arg_Stk>(AST::Arg const &, IR::Program &)
         {return IR::Arg_Stk();}

      //
      // IsIRArg
      //
      template<typename ArgT> static bool IsIRArg(AST::Arg const &arg)
      {
         return arg.data->getArg().isIRArg();
      }
      GenIsIRArg_ArgPtr2(IR::Arg_GblArr)
      GenIsIRArg_ArgPtr2(IR::Arg_LocArr)
      GenIsIRArg_ArgPtr2(IR::Arg_MapArr)
      GenIsIRArg_ArgPtr2(IR::Arg_StrArr)
      GenIsIRArg_ArgPtr2(IR::Arg_WldArr)

      // IsIRArg<IR::Arg_Cpy>
      template<>
      bool IsIRArg<IR::Arg_Cpy>(AST::Arg const &)
         {return false;}

      // IsIRArg<IR::Arg_Lit>
      template<>
      bool IsIRArg<IR::Arg_Lit>(AST::Arg const &arg)
         {return arg.data->isIRExp();}

      // IsIRArg<IR::Arg_Nul>
      template<>
      bool IsIRArg<IR::Arg_Nul>(AST::Arg const &)
         {return false;}

      // IsIRArg<IR::Arg_Stk>
      template<>
      bool IsIRArg<IR::Arg_Stk>(AST::Arg const &)
         {return false;}
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Arg copy constructor
      //
      Arg::Arg(Arg const &arg) : type{arg.type}, data{arg.data}
      {
      }

      //
      // Arg constructor
      //
      Arg::Arg(Type const *type_) : type{type_}, data{nullptr}
      {
      }

      //
      // Arg constructor
      //
      Arg::Arg(Type const *type_, Exp const *data_) : type{type_}, data{data_}
      {
      }

      //
      // Arg constructor
      //
      Arg::Arg(Type const *type_, IR::AddrBase base, Exp const *data_) :
         type{type_->getTypeQual({{base, Core::STR_}})},
         data{data_}
      {
      }

      //
      // Arg destructor
      //
      Arg::~Arg()
      {
      }

      //
      // Arg::getIRArg
      //
      IR::Arg Arg::getIRArg(IR::Program &prog) const
      {
         auto addr = type->getQualAddr();
         if(addr.base == IR::AddrBase::Gen)
            addr = IR::GetAddrGen();

         switch(addr.base)
         {
            #define GDCC_IR_AddrList(addr) \
            case IR::AddrBase::addr: \
               return GetIRArg<IR::Arg_##addr>(*this, prog);
            #include "IR/AddrList.hpp"
         }

         // Callers should always check isIRArg, so this should never get
         // invoked. But if it does, any possible source position could help.
         if(data)
            throw Core::ExceptStr(data->pos, "bad Arg::getIRArg");
         else
            throw Core::ExceptStr({nullptr, 0}, "bad Arg::getIRArg");
      }

      //
      // Arg::isIRArg
      //
      bool Arg::isIRArg() const
      {
         // Using an IR Arg may bypass strict interpretation.
         if(type->getQualVola())
            return false;

         // Using an IR Arg needs to imply a lack of side-effects.
         if(data && data->isEffect())
            return false;

         auto addr = type->getQualAddr();
         if(addr.base == IR::AddrBase::Gen)
            addr = IR::GetAddrGen();

         switch(addr.base)
         {
            #define GDCC_IR_AddrList(addr) \
            case IR::AddrBase::addr: \
               return IsIRArg<IR::Arg_##addr>(*this);
            #include "IR/AddrList.hpp"
         }

         return false;
      }
   }
}

// EOF

