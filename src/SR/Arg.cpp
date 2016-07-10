//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation storage arguments.
//
//-----------------------------------------------------------------------------

#include "SR/Arg.hpp"

#include "SR/Exp.hpp"
#include "SR/Type.hpp"

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
   inline ArgT GetIRArg<ArgT>(SR::Arg const &arg, IR::Program &prog) \
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
   inline bool IsIRArg<ArgT>(SR::Arg const &arg) \
   { \
      return arg.data->getArg().isIRArg(); \
   }


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace SR
   {
      //
      // GetIRArg
      //
      template<typename ArgT>
      static ArgT GetIRArg(SR::Arg const &arg, IR::Program &prog)
      {
         return ArgT(arg.data->getArg().getIRArg(prog));
      }
      GenGetIRArg_ArgPtr2(IR::Arg_GblArr)
      GenGetIRArg_ArgPtr2(IR::Arg_HubArr)
      GenGetIRArg_ArgPtr2(IR::Arg_LocArr)
      GenGetIRArg_ArgPtr2(IR::Arg_ModArr)
      GenGetIRArg_ArgPtr2(IR::Arg_StrArr)

      // GetIRArg<IR::Arg_Cpy>
      template<>
      IR::Arg_Cpy GetIRArg<IR::Arg_Cpy>(SR::Arg const &, IR::Program &)
         {return IR::Arg_Cpy();}

      // GetIRArg<IR::Arg_Lit>
      template<>
      IR::Arg_Lit GetIRArg<IR::Arg_Lit>(SR::Arg const &arg, IR::Program &)
         {return IR::Arg_Lit(arg.data->getIRExp());}

      // GetIRArg<IR::Arg_Nul>
      template<>
      IR::Arg_Nul GetIRArg<IR::Arg_Nul>(SR::Arg const &, IR::Program &)
         {return IR::Arg_Nul();}

      // GetIRArg<IR::Arg_Stk>
      template<>
      IR::Arg_Stk GetIRArg<IR::Arg_Stk>(SR::Arg const &, IR::Program &)
         {return IR::Arg_Stk();}

      //
      // IsIRArg
      //
      template<typename ArgT> static bool IsIRArg(SR::Arg const &arg)
      {
         return arg.data->getArg().isIRArg();
      }
      GenIsIRArg_ArgPtr2(IR::Arg_GblArr)
      GenIsIRArg_ArgPtr2(IR::Arg_HubArr)
      GenIsIRArg_ArgPtr2(IR::Arg_LocArr)
      GenIsIRArg_ArgPtr2(IR::Arg_ModArr)
      GenIsIRArg_ArgPtr2(IR::Arg_StrArr)

      // IsIRArg<IR::Arg_Cpy>
      template<>
      bool IsIRArg<IR::Arg_Cpy>(SR::Arg const &)
         {return false;}

      // IsIRArg<IR::Arg_Lit>
      template<>
      bool IsIRArg<IR::Arg_Lit>(SR::Arg const &arg)
         {return arg.data->isIRExp();}

      // IsIRArg<IR::Arg_Nul>
      template<>
      bool IsIRArg<IR::Arg_Nul>(SR::Arg const &)
         {return false;}

      // IsIRArg<IR::Arg_Stk>
      template<>
      bool IsIRArg<IR::Arg_Stk>(SR::Arg const &)
         {return false;}
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace SR
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

