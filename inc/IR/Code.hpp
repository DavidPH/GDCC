//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation code handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Code_H__
#define GDCC__IR__Code_H__

#include "../IR/Types.hpp"

#include <ostream>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::IR
{
   //
   // CodeBase
   //
   enum class CodeBase
   {
      #define GDCC_IR_CodeList(name) name,
      #include "../IR/CodeList.hpp"
      None
   };

   //
   // CodeType
   //
   // Types:
   //   A - unsigned fractional
   //   E - unsigned floating-point
   //   F - signed floating-point
   //   I - signed integral
   //   K - unsigned fixed-point
   //   R - signed fractional
   //   U - unsigned integral
   //   X - signed fixed-point
   //
   class CodeType
   {
   public:
      constexpr CodeType() : t{} {}
      constexpr CodeType(CodeType const &) = default;
      constexpr CodeType(CodeType &&) = default;
      constexpr CodeType(char t0) : t{t0} {}
      constexpr CodeType(char t0, char t1) : t{t0, t1} {}
      constexpr CodeType(char t0, char t1, char t2) : t{t0, t1, t2} {}
      constexpr CodeType(char t0, char t1, char t2, char t3) : t{t0, t1, t2, t3} {}
      constexpr CodeType(char const (&t_)[2]) : t{t_[0], t_[1]} {}
      constexpr CodeType(char const (&t_)[3]) : t{t_[0], t_[1], t_[2]} {}

      constexpr char const *begin() const {return t;}

      constexpr char const *end() const
         {return t[0] ? t[1] ? t[2] ? t[3] ? t+4 : t+3 : t+2 : t+1 : t+0;}

      constexpr explicit operator bool () const {return t[0];}

      constexpr char       &operator [] (std::size_t i)       {return t[i];}
      constexpr char const &operator [] (std::size_t i) const {return t[i];}

      constexpr bool operator == (CodeType const &r) const
         {return t[0] == r.t[0] && t[1] == r.t[1] && t[2] == r.t[2] && t[3] == r.t[3];}
      constexpr bool operator != (CodeType const &r) const
         {return t[0] != r.t[0] || t[1] != r.t[1] || t[2] != r.t[2] || t[3] != r.t[3];}

      constexpr CodeType &operator = (CodeType const &) = default;
      constexpr CodeType &operator = (CodeType &&) = default;

      constexpr std::size_t size() const
         {return t[0] ? t[1] ? t[2] ? t[3] ? 4 : 3 : 2 : 1 : 0;}

   private:
      alignas(CodeBase)
      char t[4];
   };

   //
   // Code
   //
   class Code
   {
   public:
      constexpr Code() : base{CodeBase::None}, type{} {}
      constexpr Code(Code const &) = default;
      constexpr Code(Code &&) = default;
      constexpr Code(CodeBase b) : base{b}, type{} {}
      constexpr Code(CodeBase b, CodeType t) : base{b}, type{t} {}

      constexpr bool operator == (Code const &r) const
         {return base == r.base && type == r.type;}
      constexpr bool operator != (Code const &r) const
         {return base != r.base || type != r.type;}

      constexpr Code &operator = (Code const &) = default;
      constexpr Code &operator = (Code &&) = default;

      CodeBase base;
      CodeType type;
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   constexpr Code operator + (CodeBase base, CodeType type) {return {base, type};}

   OArchive &operator << (OArchive &out, Code in);
   OArchive &operator << (OArchive &out, CodeBase in);
   OArchive &operator << (OArchive &out, CodeType in);
   std::ostream &operator << (std::ostream &out, Code in);
   std::ostream &operator << (std::ostream &out, CodeBase in);
   std::ostream &operator << (std::ostream &out, CodeType in);

   IArchive &operator >> (IArchive &in, Code &out);
   IArchive &operator >> (IArchive &in, CodeBase &out);
   IArchive &operator >> (IArchive &in, CodeType &out);
}

#endif//GDCC__IR__Code_H__

