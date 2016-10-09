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
// GDCC_BC_AddFuncPre
//
#define GDCC_BC_AddFuncPre(code, size, retrn, param, local, file) \
   Core::String name = getFuncName(code, size); \
   auto newFunc = getFuncDefn(name, retrn, param, local, file); \
   \
   if(!newFunc) \
      return

//
// GDCC_BC_AddStmnt
//
#define GDCC_BC_AddStmnt(...) \
   newFunc->block.setOrigin(__LINE__).addStatementArgs(__VA_ARGS__)

#endif//GDCC__BC__AddFunc_H__

