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
   // Info::addFunc_Neg_IW
   //
   void Info::addFunc_Neg_IW(Core::FastU n)
   {
      GDCC_BC_AddFuncPre((Code::Neg+I, n), n, n, n, __FILE__);
      GDCC_BC_AddFuncObjUna(n);

      IR::Glyph labelRetn{prog, name + "$retn"};

      GDCC_BC_AddStmnt(Code::BNot,  n, src, src);

      GDCC_BC_AddStmnt(Code::Add+U, 1, src[0], src[0], 1);

      for(Core::FastU i = 0; i != n - 1;)
      {
         GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, src[i++], labelRetn);
         GDCC_BC_AddStmnt(Code::Add+U,    1, src[i],   src[i], 1);
      }

      GDCC_BC_AddLabel(labelRetn);
      GDCC_BC_AddStmnt(Code::Retn, n, src);

      GDCC_BC_AddFuncEnd();
   }
}

// EOF

