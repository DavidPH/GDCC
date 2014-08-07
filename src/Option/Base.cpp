//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Option processing base class.
//
//-----------------------------------------------------------------------------

#include "Option/Base.hpp"

#include "Option/Program.hpp"
#include "Option/StrUtil.hpp"

#include <cstring>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Option
   {
      //
      // Base constructor
      //
      Base::Base(Program *list_, Info const &info_) :
         info{info_},

         processed{false},

         hashNext{this}, hashPrev{this},
         shrtNext{this}, shrtPrev{this},

         hash{StrHash(info_.nameL)},
         lenL{info_.nameL ? std::strlen(info_.nameL) : 0}
      {
         if(list_) list_->insert(this);
      }

      //
      // Base destructor
      //
      Base::~Base()
      {
         if(list) list->remove(this);
      }

      //
      // Base::insert
      //
      void Base::insert(Program *list_)
      {
         if(list ) list ->remove(this);
         if(list_) list_->insert(this);
      }

      //
      // Base::process
      //
      std::size_t Base::process(Args const &args)
      {
         processed = true;
         return v_process(args);
      }

      //
      // Base::remove
      //
      void Base::remove()
      {
         if(list) list->remove(this);
      }
   }
}

// EOF

