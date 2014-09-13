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

      //
      // Program
      //
      class Program
      {
      public:
         Program() = default;

         Program(Program const &) = delete;
         Program(Program &&) = delete;

         ~Program();

         Program &operator = (Program const &) = delete;
         Program &operator = (Program &&) = delete;

         Base *find(Args const &args);
         Base *find(char const *name);
         Base *find(char name);

         Base &get(Args const &args);

         void process(Args const &args);

         void putAsciiDoc(std::ostream &out, std::size_t width = 0);
         void putHelp(std::ostream &out, std::size_t width = 0);
         void putHelpLong(std::ostream &out, std::size_t width = 0);
         void putVersion(std::ostream &out, std::size_t width = 0);

         Base *processLoose = nullptr;

         char const *descL    = nullptr;
         char const *descS    = nullptr;
         char const *name     = nullptr;
         char const *nameFull = nullptr;
         char const *usage    = nullptr;
         char const *version  = nullptr;


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
         bool        processFile(char const *file);
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

