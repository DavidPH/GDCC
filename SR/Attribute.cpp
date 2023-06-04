//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2023 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation generalized attribute handling.
//
//-----------------------------------------------------------------------------

#include "SR/Attribute.hpp"

#include "SR/Type.hpp"

#include "Core/Token.hpp"

#include "IR/Exp.hpp"
#include "IR/Linkage.hpp"

#include "Target/CallType.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::SR
{
   //
   // Attribute constructor
   //
   Attribute::Attribute() :
      addrI   {nullptr},
      addrS   {nullptr},
      allocAut{nullptr},
      callt   {IR::CallType::None},
      linka   {IR::Linkage::None},
      name    {nullptr},
      namePos {nullptr, 0},
      param   {},
      paramOpt{0},
      space   {IR::AddrBase::Gen, Core::STR_},
      stype   {},
      type    {nullptr},
      warnUse {nullptr},

      declAuto{false},

      funcDelay      {false},
      funcInline     {false},
      funcNoInitDelay{false},
      funcNoParam    {false},
      funcNoReturn   {false},

      isTypedef{false},

      objAnonymous{false},
      objNoInit   {false},

      stReturn{false},

      storeAuto  {false},
      storeExt   {false},
      storeGbl   {false},
      storeHub   {false},
      storeInt   {false},
      storeReg   {false},
      storeThread{false}
   {
   }

   //
   // Attribute copy constructor
   //
   Attribute::Attribute(Attribute const &) = default;

   //
   // Attribute move constructor
   //
   Attribute::Attribute(Attribute &&) = default;

   //
   // Attribute destructor
   //
   Attribute::~Attribute()
   {
   }

   //
   // Attribute::clear
   //
   void Attribute::clear()
   {
      this->~Attribute();
      new(this) Attribute;
   }

   //
   // Attribute::setName
   //
   void Attribute::setName(Core::Token const &tok)
   {
      name    = tok.str;
      namePos = tok.pos;
   }
}

// EOF

