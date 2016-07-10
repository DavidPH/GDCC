//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
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

#include "IR/CallType.hpp"
#include "IR/Exp.hpp"
#include "IR/Linkage.hpp"
#include "IR/ScriptType.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace SR
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
         stype   {IR::ScriptType::None},
         type    {nullptr},
         warnUse {nullptr},

         declAuto{false},

         funcInline  {false},
         funcNoParam {false},
         funcNoReturn{false},

         isTypedef{false},

         objNoInit{false},

         sflagClS{false},
         sflagNet{false},

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
      // Attribute::setName
      //
      void Attribute::setName(Core::Token const &tok)
      {
         name    = tok.str;
         namePos = tok.pos;
      }
   }
}

// EOF

