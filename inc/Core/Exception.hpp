//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Base GDCC exception classes.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__Exception_H__
#define GDCC__Core__Exception_H__

#include "../Core/Origin.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Core
{
   //
   // Exception
   //
   class Exception : public std::exception
   {
   public:
      Exception(Exception const &e);
      Exception(Exception &&) = default;

      virtual char const *what() const noexcept;

      void setOrigin(Origin pos_) {pos = pos_;}

   protected:
      Exception() noexcept : pos{nullptr, 0} {}
      Exception(Origin pos_) noexcept : pos{pos_} {}

      std::ostream &putOrigin(std::ostream &out) const noexcept;

      virtual char const *whatGen() const noexcept = 0;

      char const *whatSet(std::string const &str) const noexcept;

      Origin pos;

      mutable std::unique_ptr<char[]> whatCache;
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Core
{
   [[noreturn]]
   void Error(Origin pos, char const *msg);
   [[noreturn]]
   void Error(Origin pos, String msg);

   [[noreturn]]
   void ErrorExpect(char const *exp, Token const &got, bool expQ = false, bool gotQ = true);
   [[noreturn]]
   void ErrorExpect(Origin pos, char const *exp, String got, bool expQ = false, bool gotQ = true);
   [[noreturn]]
   void ErrorExpect(Origin pos, String exp, String got, bool expQ = false, bool gotQ = true);
   [[noreturn]]
   void ErrorExpect(String exp, Token const &got, bool expQ = false, bool gotQ = true);

   [[noreturn]]
   void ErrorFile(String filename, char const *mode);

   [[noreturn]]
   void ErrorFileInc(Origin pos, String filename);
   [[noreturn]]
   void ErrorFileInc(Origin pos, String filename, char l, char r);

   [[noreturn]]
   void ErrorUndef(Origin pos, String type, String name);
   [[noreturn]]
   void ErrorUndef(String type, String name);
}

#endif//GDCC__Core__Exception_H__

