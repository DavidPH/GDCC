//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
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
#define GDCC_BC_AddFuncPre(code, size, retrn, param, local, file) \
   Core::String name; \
   { \
      using IR::Code; \
      name = getFuncName(code, size); \
   } \
   auto newFunc = getFuncDefn(name, retrn, param, local, file); \
   \
   if(!newFunc) \
      return

//
// GDCC_BC_AddFuncObj
//
#define GDCC_BC_AddFuncObj() \
   IR::Arg_Nul nul{}; ((void)nul); \
   IR::Arg_Stk stk{}; ((void)stk)

//
// GDCC_BC_AddFuncObjBin
//
#define GDCC_BC_AddFuncObjBin(size) \
   GDCC_BC_AddFuncObj(); \
   \
   IR::Arg_LocReg lop{IR::Arg_Lit(newFunc->block.getExp(0))}; \
   IR::Arg_LocReg rop{IR::Arg_Lit(newFunc->block.getExp(size))}

//
// GDCC_BC_AddFuncObjUna
//
#define GDCC_BC_AddFuncObjUna(size) \
   GDCC_BC_AddFuncObj(); \
   \
   IR::Arg_LocReg lop{IR::Arg_Lit(newFunc->block.getExp(0))}

//
// GDCC_BC_AddLabel
//
#define GDCC_BC_AddLabel(lab) \
   newFunc->block.addLabel(lab)

//
// GDCC_BC_AddStmnt
//
#define GDCC_BC_AddStmnt(...) \
   do \
   { \
      using IR::Code; \
      newFunc->block.setOrigin(__LINE__).addStatementArgs(__VA_ARGS__); \
   } \
   while(false)

//
// GDCC_BC_ArgLit
//
#define GDCC_BC_ArgLit(val) (::GDCC::IR::Arg_Lit{newFunc->block.getExp(val)})

#endif//GDCC__BC__AddFunc_H__

