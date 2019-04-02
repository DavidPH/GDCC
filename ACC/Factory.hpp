//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS expression/statement creation.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__ACC__Factory_H__
#define GDCC__ACC__Factory_H__

#include "../ACC/Types.hpp"

#include "../CC/Factory.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::ACC
{
   //
   // Factory
   //
   class Factory : public CC::Factory
   {
   public:
      // Conversion as if by assignment.
      virtual ExpRef expPromo_Assign(Type const *t, Exp const *e, Origin pos);
   };
}

#endif//GDCC__ACC__Factory_H__

