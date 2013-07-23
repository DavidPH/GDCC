//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS target information and handling class.
//
//-----------------------------------------------------------------------------

#ifndef Bytecode__ZDACS__Info_H__
#define Bytecode__ZDACS__Info_H__

#include "../Info.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace Bytecode
{
   namespace ZDACS
   {
      //
      // Info
      //
      class Info : public InfoBase
      {
      public:
         virtual void put(std::ostream &out);

         virtual void translateBlock(GDCC::IR::Block &block);
      };
   }
}

#endif//Bytecode__MgC__Info_H__

