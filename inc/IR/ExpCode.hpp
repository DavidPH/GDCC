//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Expression code functions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__ExpCode_H__
#define GDCC__IR__ExpCode_H__

#include "../IR/Code.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // ExpCode_A_W
      //
      template<typename Codes>
      Code ExpCode_A_W(Core::FastU words)
      {
         switch(words)
         {
         case  1: return Codes::CodeA_W;
         case  2: return Codes::CodeA_W2;
         default: return Code::None;
         }
      }

      //
      // ExpCode_F_W
      //
      template<typename Codes>
      Code ExpCode_F_W(Core::FastU words)
      {
         switch(words)
         {
         case  1: return Codes::CodeF_W;
         case  2: return Codes::CodeF_W2;
         default: return Code::None;
         }
      }

      //
      // ExpCode_I_W
      //
      template<typename Codes>
      Code ExpCode_I_W(Core::FastU words)
      {
         switch(words)
         {
         case  1: return Codes::CodeI_W;
         case  2: return Codes::CodeI_W2;
         case  3: return Codes::CodeI_W3;
         default: return Code::None;
         }
      }

      //
      // ExpCode_K_W
      //
      template<typename Codes>
      Code ExpCode_K_W(Core::FastU words)
      {
         switch(words)
         {
         case  1: return Codes::CodeK_W;
         case  2: return Codes::CodeK_W2;
         default: return Code::None;
         }
      }

      //
      // ExpCode_Move
      //
      inline Code ExpCode_Move(Core::FastU words)
      {
         switch(words)
         {
         case  1: return Code::Move_W;
         case  2: return Code::Move_W2;
         case  3: return Code::Move_W3;
         case  4: return Code::Move_W4;
         case  5: return Code::Move_W5;
         case  6: return Code::Move_W6;
         default: return Code::None;
         }
      }

      //
      // ExpCode_U_W
      //
      template<typename Codes>
      Code ExpCode_U_W(Core::FastU words)
      {
         switch(words)
         {
         case  1: return Codes::CodeU_W;
         case  2: return Codes::CodeU_W2;
         case  3: return Codes::CodeU_W3;
         default: return Code::None;
         }
      }

      //
      // ExpCode_R_W
      //
      template<typename Codes>
      Code ExpCode_R_W(Core::FastU words)
      {
         switch(words)
         {
         case  1: return Codes::CodeR_W;
         case  2: return Codes::CodeR_W2;
         default: return Code::None;
         }
      }

      //
      // ExpCode_X_W
      //
      template<typename Codes>
      Code ExpCode_X_W(Core::FastU words)
      {
         switch(words)
         {
         case  1: return Codes::CodeX_W;
         case  2: return Codes::CodeX_W2;
         default: return Code::None;
         }
      }
   }
}

#endif//GDCC__IR__ExpCode_H__

