//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Negation statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/Info.hpp"

#include "BC/AddFunc.hpp"

#include "IR/Function.hpp"
#include "IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC
{
   //
   // Info::addFunc_NegI_W
   //
   void Info::addFunc_NegI_W(Core::FastU n)
   {
      GDCC_BC_AddFuncPre(Code::NegI, n, n, n, n, __FILE__);
      GDCC_BC_AddFuncObjUna(n);

      IR::Glyph labelRetn{prog, name + "$retn"};

      GDCC_BC_AddStmnt(Code::BNot, n, lop, lop);

      GDCC_BC_AddStmnt(Code::AddU, 1, lop[0], lop[0], 1);

      for(Core::FastU i = 0; i != n - 1;)
      {
         GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, lop[i++], labelRetn);
         GDCC_BC_AddStmnt(Code::AddU,     1, lop[i],   lop[i], 1);
      }

      GDCC_BC_AddLabel(labelRetn);
      GDCC_BC_AddStmnt(Code::Retn, n, lop);

      GDCC_BC_AddFuncEnd();
   }
}

// EOF

