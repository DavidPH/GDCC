//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C case block scope handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Scope__Case_H__
#define GDCC__CC__Scope__Case_H__

#include "../../CC/Scope/Break.hpp"

#include "../../Core/StringGen.hpp"
#include "../../Core/Utility.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Scope_Case
      //
      // A scope that supports case labels. In C, switch scopes.
      //
      class Scope_Case : public Scope_Break
      {
      public:
         //
         // Case
         //
         class Case
         {
         public:
            Case(Core::Integ const &value_, Core::String label_) :
               value{value_}, label{label_}, defin{false} {}
            explicit Case(Core::String label_) :
               value{}, label{label_}, defin{false} {}

            Core::Integ  const value;
            Core::String const label;
            bool               defin : 1;
         };

         using CaseSet = std::unordered_map<Core::Integ, Case>;
         using CaseItr = Core::MemItr<typename CaseSet::const_iterator, Case const>;


         explicit Scope_Case(Scope_Local &parent);

         CaseItr begin() const {return CaseItr(caseSet.begin());}

         CaseItr end() const {return CaseItr(caseSet.end());}

         Case const &getDefault() const {return caseDef;}

         virtual Core::String getLabelCase(Core::Integ const &n, bool define);
         virtual Core::String getLabelDefault(bool define);

         CaseSet::size_type size() const {return caseSet.size();}

      protected:
         Case            caseDef;
         Core::StringGen caseGen;
         CaseSet         caseSet;
      };
   }
}

#endif//GDCC__CC__Scope__Case_H__

