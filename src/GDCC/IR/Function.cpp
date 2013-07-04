//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation function handling.
//
//-----------------------------------------------------------------------------

#include "Function.hpp"

#include <unordered_map>


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

static std::unordered_map<GDCC::String, GDCC::IR::Function> FuncMap;


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Function constructor
      //
      Function::Function(String glyph_) :
         block   {},
         ctype   {CallType::None},
         glyph   {glyph_},
         label   {STRNULL},
         linka   {Linkage::None},
         localArs{},
         localReg{},
         param   {},
         retrn   {},
         stype   {ScriptType::None},
         valueInt{0},
         valueStr{STRNULL},

         exdef   {false},
         sflagNet{false},
         sflagClS{false}
      {
      }

      //
      // Function::Find
      //
      Function *Function::Find(String glyph)
      {
         auto itr = FuncMap.find(glyph);

         return itr == FuncMap.end() ? nullptr : &itr->second;
      }

      //
      // Function::Get
      //
      Function &Function::Get(String glyph)
      {
         auto itr = FuncMap.find(glyph);

         if(itr != FuncMap.end()) return itr->second;

         itr = FuncMap.emplace(std::piecewise_construct,
            std::forward_as_tuple(glyph), std::forward_as_tuple(glyph)).first;

         return itr->second;
      }
   }
}

// EOF

