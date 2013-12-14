//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Assembly macros.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AS__Macro_H__
#define GDCC__AS__Macro_H__

#include "../IR/Block.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AS
   {
      //
      // Macro
      //
      class Macro
      {
      public:
         Macro(Core::String, IR::Block &&list_) : list{std::move(list_)} {}

         void expand(IR::Block &block, IR::Arg const *argv,
            std::size_t argc) const;


         static void Add(Core::String name, IR::Block &&list);

         static Macro const *Find(Core::String name);

      private:
         IR::Block list;
      };
   }
}

#endif//GDCC__AS__Macro_H__

