//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation space handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__Space_H__
#define GDCC__SR__Space_H__

#include "../Core/Counter.hpp"
#include "../Core/String.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      enum class AddrBase;
      enum class Linkage;

      class Exp;
      class Program;
   }

   namespace SR
   {
      //
      // Space
      //
      class Space final : public Core::CounterBase
      {
         GDCC_Core_CounterPreambleNoVirtual(
            GDCC::SR::Space, GDCC::Core::CounterBase);

      protected:
         using IRExpCPtr = Core::CounterPtr<IR::Exp const>;

      public:
         void genSpace(IR::Program &prog) const;

         Core::String glyph;
         IR::Linkage  linka;
         Core::String name;
         IR::AddrBase space;
         IRExpCPtr    value;

         bool         defin : 1; // Is defined?


         static Ref Create(Core::String name, Core::String glyph);

      protected:
         Space(Core::String name, Core::String glyph);
         ~Space();
      };
   }
}

#endif//GDCC__SR__Space_H__

