//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Function generation utilities.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__BC__AddFunc_H__
#define GDCC__BC__AddFunc_H__

#include "../BC/Types.hpp"

#include "../Core/Array.hpp"
#include "../Core/IntItr.hpp"
#include "../Core/Number.hpp"

#include "../IR/Block.hpp"

#include "../Target/Info.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_BC_AddFuncEnd
//
#define GDCC_BC_AddFuncEnd() \
   throw ResetFunc()

//
// GDCC_BC_AddFuncPre
//
#define GDCC_BC_AddFuncPre(code, retrn, param, local, file) \
   Core::String name; \
   { \
      [[maybe_unused]] typedef ::GDCC::IR::CodeBase Code; \
      \
      [[maybe_unused]] constexpr char A = 'A'; \
      [[maybe_unused]] constexpr char E = 'E'; \
      [[maybe_unused]] constexpr char F = 'F'; \
      [[maybe_unused]] constexpr char I = 'I'; \
      [[maybe_unused]] constexpr char K = 'K'; \
      [[maybe_unused]] constexpr char R = 'R'; \
      [[maybe_unused]] constexpr char U = 'U'; \
      [[maybe_unused]] constexpr char X = 'X'; \
      \
      name = getFuncName code; \
   } \
   auto newFunc = getFuncDefn(name, retrn, param, local, file); \
   \
   if(!newFunc) \
      return

//
// GDCC_BC_AddFuncObj
//
#define GDCC_BC_AddFuncObj() \
   [[maybe_unused]] Core::FastU w = Target::GetWordBytes(); \
   \
   [[maybe_unused]] ::GDCC::IR::Block::Nul nul{}; \
   [[maybe_unused]] ::GDCC::IR::Block::Stk stk{}

//
// GDCC_BC_AddFuncObjBin
//
#define GDCC_BC_AddFuncObjBin(sizeL, sizeR) \
   GDCC_BC_AddFuncObj(); \
   \
   GDCC_BC_AddFuncObjReg(lop, sizeL, 0); \
   GDCC_BC_AddFuncObjReg(rop, sizeR, sizeL)

//
// GDCC_BC_AddFuncObjReg
//
#define GDCC_BC_AddFuncObjReg(name, size, addr) \
   ::GDCC::BC::AddFuncObj<::GDCC::IR::Arg_LocReg> name{newFunc->block, w, size, addr}

//
// GDCC_BC_AddFuncObjUna
//
#define GDCC_BC_AddFuncObjUna(size) \
   GDCC_BC_AddFuncObj(); \
   \
   GDCC_BC_AddFuncObjReg(src, size, 0)

//
// GDCC_BC_AddLabel
//
#define GDCC_BC_AddLabel(lab) \
   newFunc->block.addLabel(lab)

//
// GDCC_BC_AddStmnt
//
#define GDCC_BC_AddStmnt(code, size, ...) \
   do \
   { \
      [[maybe_unused]] typedef ::GDCC::IR::Block::Stk Stk; \
      [[maybe_unused]] typedef ::GDCC::IR::Block::Nul Nul; \
      [[maybe_unused]] typedef ::GDCC::IR::CodeBase Code; \
      \
      [[maybe_unused]] constexpr char A = 'A'; \
      [[maybe_unused]] constexpr char E = 'E'; \
      [[maybe_unused]] constexpr char F = 'F'; \
      [[maybe_unused]] constexpr char I = 'I'; \
      [[maybe_unused]] constexpr char K = 'K'; \
      [[maybe_unused]] constexpr char R = 'R'; \
      [[maybe_unused]] constexpr char U = 'U'; \
      [[maybe_unused]] constexpr char X = 'X'; \
      \
      newFunc->block \
         .setArgSize((size) * w) \
         .setOrigin(__LINE__) \
         .addStmnt(code, __VA_ARGS__); \
   } \
   while(false)

//
// GDCC_BC_AddStmntArgs
//
#define GDCC_BC_AddStmntArgs(code, size, ...) \
   do \
   { \
      [[maybe_unused]] typedef ::GDCC::IR::Block::Stk Stk; \
      [[maybe_unused]] typedef ::GDCC::IR::Block::Nul Nul; \
      [[maybe_unused]] typedef ::GDCC::IR::CodeBase Code; \
      \
      [[maybe_unused]] constexpr char A = 'A'; \
      [[maybe_unused]] constexpr char E = 'E'; \
      [[maybe_unused]] constexpr char F = 'F'; \
      [[maybe_unused]] constexpr char I = 'I'; \
      [[maybe_unused]] constexpr char K = 'K'; \
      [[maybe_unused]] constexpr char R = 'R'; \
      [[maybe_unused]] constexpr char U = 'U'; \
      [[maybe_unused]] constexpr char X = 'X'; \
      \
      newFunc->block \
         .setArgSize((size) * w) \
         .setOrigin(__LINE__) \
         .addStmntArgs(code, __VA_ARGS__); \
   } \
   while(false)

//
// GDCC_BC_ArgLit
//
#define GDCC_BC_ArgLit(size, val) (::GDCC::IR::Arg_Lit((size) * w, newFunc->block.getExp(val)))


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::BC
{
   //
   // AddFuncObj
   //
   template<typename ArgT>
   class AddFuncObj
   {
   public:
      AddFuncObj(IR::Block &block, Core::FastU w, Core::FastU n, Core::FastU addr) :
         a{w * n, IR::Arg_Lit(w, block.getExp(w * addr))},
         v{Core::IntItr<Core::FastU>(0), Core::IntItr<Core::FastU>(n),
            [&](Core::FastU i)
               {return ArgT(w, IR::Arg_Lit(w, block.getExp(w * (addr + i))));}},
         hi{v[n-1]},
         lo{v[0]}
      {
      }

      operator ArgT & () {return a;}

      ArgT &operator [] (std::size_t i) {return v[i];}

   private:
      ArgT                    a;
      GDCC::Core::Array<ArgT> v;

   public:
      ArgT &hi, &lo;
   };
}

#endif//GDCC__BC__AddFunc_H__

