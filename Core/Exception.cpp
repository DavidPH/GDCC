//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2025 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Base GDCC exception classes.
//
//-----------------------------------------------------------------------------

#include "Core/Exception.hpp"

#include "Core/Token.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Core
{
   //
   // ExceptFile
   //
   class ExceptFile : public Exception
   {
   public:
      ExceptFile(String filename_, String mode_) noexcept :
         Exception{}, filename{filename_}, mode{mode_} {}

   protected:
      virtual char const *whatGen() const noexcept;

      String const filename;
      String const mode;
   };

   //
   // ExceptFileInc
   //
   class ExceptFileInc : public Exception
   {
   public:
      ExceptFileInc(Origin pos_, String filename_, char l_, char r_) noexcept :
         Exception{pos_}, filename{filename_}, l{l_}, r{r_} {}

   protected:
      virtual char const *whatGen() const noexcept;

      Core::String const filename;
      char         const l, r;
   };

   //
   // ExceptStringC
   //
   class ExceptStringC : public Exception
   {
   public:
      ExceptStringC(Origin pos_, char const *str_) noexcept :
         Exception{pos_}, str{str_} {}

   protected:
      virtual char const *whatGen() const noexcept;

      char const *const str;
   };

   //
   // ExceptStringS
   //
   class ExceptStringS : public Exception
   {
   public:
      ExceptStringS(Origin pos_, String str_) noexcept :
         Exception{pos_}, str{str_} {}

   protected:
      virtual char const *whatGen() const noexcept;

      String const str;
   };

   //
   // ExceptStringU
   //
   class ExceptStringU : public Exception
   {
   public:
      ExceptStringU(ExceptStringU const &e) :
         Exception{e}, str{StrDup(e.str.get())} {}
      ExceptStringU(ExceptStringU &&) = default;
      ExceptStringU(Origin pos_, std::unique_ptr<char[]> &&str_) noexcept :
         Exception{pos_}, str{std::move(str_)} {}

   protected:
      virtual char const *whatGen() const noexcept;

      std::unique_ptr<char[]> str;
   };

   //
   // ExceptStringZ
   //
   class ExceptStringZ : public Exception
   {
   public:
      ExceptStringZ(Origin pos_, std::string &&str_) noexcept :
         Exception{pos_}, str{std::move(str_)} {}

   protected:
      virtual char const *whatGen() const noexcept;

      std::string const str;
   };

   //
   // ExceptUndef
   //
   class ExceptUndef : public Exception
   {
   public:
      ExceptUndef(Origin pos_, String type_, String name_) noexcept :
         Exception{pos_}, type{type_}, name{name_} {}

   protected:
      virtual char const *whatGen() const noexcept;

      String const type;
      String const name;
   };

   //
   // ExceptExpect
   //
   class ExceptExpect : public Exception
   {
   public:
      ExceptExpect(Origin pos_, char const *exp_, String got_, bool expQ_, bool gotQ_) noexcept :
         Exception{pos_}, expC{exp_}, got{got_}, expQ{expQ_}, gotQ{gotQ_}, isExpC{true} {}
      ExceptExpect(Origin pos_, String exp_, String got_, bool expQ_, bool gotQ_) noexcept :
         Exception{pos_}, expS{exp_}, got{got_}, expQ{expQ_}, gotQ{gotQ_}, isExpC{false} {}

   protected:
      virtual char const *whatGen() const noexcept;

      union
      {
         Core::String const expS;
         char const  *const expC;
      };

      Core::String const got;
      bool         const expQ : 1;
      bool         const gotQ : 1;
      bool         const isExpC : 1;
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Core
{
   //
   // Exception copy constructor
   //
   Exception::Exception(Exception const &e) :
      pos{e.pos},
      whatCache{e.whatCache ? StrDup(e.whatCache.get()) : nullptr}
   {
   }

   //
   // Exception::putOrigin
   //
   std::ostream &Exception::putOrigin(std::ostream &out) const noexcept
   {
      if(pos.file) out << pos << ": ";
      return out;
   }

   //
   // Exception::what
   //
   char const *Exception::what() const noexcept
   {
      return whatCache ? whatCache.get() : whatGen();
   }

   //
   // Exception::whatSet
   //
   char const *Exception::whatSet(std::string const &str) const noexcept
   {
      try
      {
         return (whatCache = StrDup(str. data(), str.size())).get();
      }
      catch(std::bad_alloc const &)
      {
         return "";
      }
   }

   //
   // ExceptFile::whatGen
   //
   char const *ExceptFile::whatGen() const noexcept
   {
      std::ostringstream oss;
      putOrigin(oss) << "could not open '" << filename << "' for " << mode;
      return whatSet(oss.str());
   }

   //
   // ExceptFileInc::whatGen
   //
   char const *ExceptFileInc::whatGen() const noexcept
   {
      std::ostringstream oss;
      putOrigin(oss) << "could not include " << l << filename << r;
      return whatSet(oss.str());
   }

   //
   // ExceptStringC::whatGen
   //
   char const *ExceptStringC::whatGen() const noexcept
   {
      std::ostringstream oss;
      putOrigin(oss) << str;
      return whatSet(oss.str());
   }

   //
   // ExceptStringS::whatGen
   //
   char const *ExceptStringS::whatGen() const noexcept
   {
      std::ostringstream oss;
      putOrigin(oss) << str;
      return whatSet(oss.str());
   }

   //
   // ExceptStringU::whatGen
   //
   char const *ExceptStringU::whatGen() const noexcept
   {
      std::ostringstream oss;
      putOrigin(oss) << str.get();
      return whatSet(oss.str());
   }

   //
   // ExceptStringZ::whatGen
   //
   char const *ExceptStringZ::whatGen() const noexcept
   {
      std::ostringstream oss;
      putOrigin(oss) << str;
      return whatSet(oss.str());
   }

   //
   // ExceptUndef::whatGen
   //
   char const *ExceptUndef::whatGen() const noexcept
   {
      std::ostringstream oss;
      putOrigin(oss) << type << " undefined: '" << name << '\'';
      return whatSet(oss.str());
   }

   //
   // ExceptExpect::whatGen
   //
   char const *ExceptExpect::whatGen() const noexcept
   {
      std::ostringstream oss;

      putOrigin(oss);

      oss << "expected ";
      if(expQ) oss << '\'';
      if(isExpC) oss << expC; else oss << expS;
      if(expQ) oss << '\'';

      oss << " got ";
      if(gotQ) oss << '\'';
      oss << got;
      if(gotQ) oss << '\'';

      return whatSet(oss.str());
   }

   //
   // Error
   //
   void Error(Origin pos, String msg)
      {throw ExceptStringS(pos, msg);}
   void Error(Origin pos, char const *msg)
      {throw ExceptStringC(pos, msg);}
   void Error(Origin pos, std::string &&msg)
      {throw ExceptStringZ(pos, std::move(msg));}
   void Error(Origin pos, std::unique_ptr<char[]> &&msg)
      {throw ExceptStringU(pos, std::move(msg));}

   //
   // ErrorExpect
   //
   void ErrorExpect(Origin pos, String exp, String got, bool expQ, bool gotQ)
      {throw ExceptExpect(pos, exp, got, expQ, gotQ);}
   void ErrorExpect(Origin pos, char const *exp, String got, bool expQ, bool gotQ)
      {throw ExceptExpect(pos, exp, got, expQ, gotQ);}
   void ErrorExpect(String exp, Token const &got, bool expQ, bool gotQ)
      {throw ExceptExpect(got.pos, exp, got.str, expQ, gotQ);}
   void ErrorExpect(char const *exp, Token const &got, bool expQ, bool gotQ)
      {throw ExceptExpect(got.pos, exp, got.str, expQ, gotQ);}

   //
   // ErrorFile
   //
   void ErrorFile(String filename, char const *mode)
      {throw ExceptFile(filename, mode);}

   //
   // ErrorFileInc
   //
   void ErrorFileInc(Origin pos, String filename)
      {throw ExceptFileInc(pos, filename, '"', '"');}
   void ErrorFileInc(Origin pos, String filename, char l, char r)
      {throw ExceptFileInc(pos, filename, l, r);}

   //
   // ErrorUndef
   //
   void ErrorUndef(Origin pos, String type, String name)
      {throw ExceptUndef(pos, type, name);}
   void ErrorUndef(String type, String name)
      {throw ExceptUndef({}, type, name);}
}

// EOF

