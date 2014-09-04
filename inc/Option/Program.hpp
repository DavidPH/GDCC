//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Option lists.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Option__Program_H__
#define GDCC__Option__Program_H__

#include <ostream>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Option
   {
      class Args;
      class Base;

      /// List of options for a program.

      ///
      ///
      class Program
      {
      public:
         /// Default constructor.

         /// Constructs an empty list of options.
         ///
         Program() = default;

         Program(Program const &) = delete;
         Program(Program &&) = delete;

         /// Destructor.
         ~Program();

         Program &operator = (Program const &) = delete;
         Program &operator = (Program &&) = delete;

         /// Searches for an option.
         Base *find(Args const &args);

         /// Searches for an option with a long name.
         Base *find(char const *name);

         /// Searches for an option with a short name.
         Base *find(char name);

         /// Gets an option or throws.
         Base &get(Args const &args);

         /// Processes program arguments.
         void process(Args const &args);

         /// Generates AsciiDoc documentation.
         void putAsciiDoc(std::ostream &out, std::size_t width = 0);

         /// Generates basic usage documentation.
         void putHelp(std::ostream &out, std::size_t width = 0);

         /// Generates long usage documentation.
         void putHelpLong(std::ostream &out, std::size_t width = 0);

         /// Generates version documentation.
         void putVersion(std::ostream &out, std::size_t width = 0);

         /// Option invoked to handle loose arguments.
         Base *processLoose = nullptr;

         char const *descL    = nullptr; ///< Program's long description.
         char const *descS    = nullptr; ///< Program's short description.
         char const *name     = nullptr; ///< Program's name.
         char const *nameFull = nullptr; ///< Program's full name.
         char const *usage    = nullptr; ///< Program's usage overview.
         char const *version  = nullptr; ///< Program's version string.


         friend class Base;

         static void PutWrapped(std::ostream &out, std::size_t width,
            char const *str, std::size_t baselen, char const *prefix = nullptr);

         static std::size_t WordLen(char const *s);

      private:
         //
         // GroupInfo
         //
         class GroupInfo
         {
         public:
            Base *first, *last;
            std::size_t len, lenL, lenS;
         };


         void buildHashChain();

         void buildShrtChain();

         GroupInfo getGroupInfo(Base *opt);

         Base *&getShrtChain(char c)
            {return shrtChainV[static_cast<unsigned char>(c) % shrtChainC];}

         void insert(Base *opt);
         void insertHash(Base *opt);
         void insertList(Base *opt);
         void insertShrt(Base *opt);

         std::size_t processArgs(Args args);
         std::size_t processLong(Args args);
         std::size_t processShrt(Args args);

         void putHelpUsage(std::ostream &out, std::size_t width);

         void remove(Base *opt);
         void removeHash(Base *opt);
         void removeList(Base *opt);
         void removeShrt(Base *opt);

         Base      **hashChainV = nullptr;
         std::size_t hashChainC = 0;

         Base       *listHead  = nullptr;
         std::size_t listSize  = 0;
         std::size_t listSizeL = 0;
         std::size_t listSizeS = 0;

         Base      **shrtChainV = nullptr;
         std::size_t shrtChainC = 0;


         static std::size_t GetWidthDefault();

         static void PutHelpPre(std::ostream &out, GroupInfo const &info, Base *opt);
      };
   }
}

#endif//GDCC__Option__Program_H__

