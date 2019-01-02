//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Bitwise statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/Info.hpp"

#include "BC/AddFunc.hpp"

#include "IR/Function.hpp"
#include "IR/Glyph.hpp"

#include <sstream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC
{
   //
   // Info::addFunc_Bclo_W
   //
   void Info::addFunc_Bclo_W(Core::FastU n)
   {
      addFunc_Bclz_W(n, IR::Code::Bclo, -1);
   }

   //
   // Info::addFunc_Bclz_W
   //
   void Info::addFunc_Bclz_W(Core::FastU n)
   {
      addFunc_Bclz_W(n, IR::Code::Bclz, 0);
   }

   //
   // Info::addFunc_Bclz_W
   //
   void Info::addFunc_Bclz_W(Core::FastU n, IR::Code code, Core::FastU skip)
   {
      GDCC_BC_AddFuncPre(code, n, 1, n, n, __FILE__);
      GDCC_BC_AddFuncObjUna(n);

      // Generate labels.
      Core::Array<IR::Glyph> labelSkip{n};
      for(Core::FastU i = n; i;)
      {
         std::ostringstream labelBuf; labelBuf << name << "$skip" << i;
         std::string const &label = labelBuf.str();
         labelSkip[--i] = {prog, {label.data(), label.size()}};
      }

      Core::FastU bits = Target::GetWordBits();

      Core::FastU i = n;

      GDCC_BC_AddStmnt(Code::Move,     1, stk, lop[--i]);
      GDCC_BC_AddStmnt(Code::Jcnd_Tab, 1, stk, skip, labelSkip[i]);

      GDCC_BC_AddStmnt(code,           1, stk, stk);
      GDCC_BC_AddStmnt(Code::Retn,     1, stk);

      while(i)
      {
         newFunc->block.addLabel(labelSkip[i]);
         GDCC_BC_AddStmnt(Code::Move,     1, stk, lop[--i]);
         GDCC_BC_AddStmnt(Code::Jcnd_Tab, 1, stk, skip, labelSkip[i]);

         GDCC_BC_AddStmnt(code,           1, stk, stk);
         GDCC_BC_AddStmnt(Code::AddU,     1, stk, stk, (n - 1 - i) * bits);
         GDCC_BC_AddStmnt(Code::Retn,     1, stk);
      }

      newFunc->block.addLabel(labelSkip[0]);
      GDCC_BC_AddStmnt(Code::Retn, 1, n * bits);

      GDCC_BC_AddFuncEnd();
   }
}

// EOF

