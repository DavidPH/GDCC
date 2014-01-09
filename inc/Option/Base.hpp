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

#ifndef GDCC__Option__Base_H__
#define GDCC__Option__Base_H__

#include <cstddef>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Option
   {
      class Program;

      //
      // Args
      //
      struct Args
      {
         Args() :
            argV{nullptr},
            argC{0},

            nameL{nullptr},
            nameS{'\0'},

            optFalse{false},
            optFinal{false},
            optKeepA{false}
         {
         }

         Args &drop(std::size_t n = 1) {argC -= n; argV += n; return *this;}

         Args &setArgC(std::size_t argC_) {argC = argC_; return *this;}

         Args &setArgs(char const *const *argV_, std::size_t argC_)
            {argV = argV_; argC = argC_; return *this;}

         Args &setName (char const *name) {nameL = name; return *this;}
         Args &setName (char        name) {nameS = name; return *this;}

         Args &setOptKeepA(bool opt = true) {optFinal = opt; return *this;}
         Args &setOptFinal(bool opt = true) {optFinal = opt; return *this;}

         char const *const *argV;
         std::size_t        argC;

         char const *nameL;
         char        nameS;

         bool optFalse : 1; // The option is negated.
         bool optFinal : 1; // There are no more args. (Hint only.)
         bool optKeepA : 1; // Argument strings may be kept.
      };

      //
      // Base
      //
      class Base
      {
      public:
         //
         // Info
         //
         struct Info
         {
         public:
            Info &setDescL(char const *descL_) {descL = descL_; return *this;}
            Info &setDescS(char const *descS_) {descS = descS_; return *this;}
            Info &setGroup(char const *group_) {group = group_; return *this;}
            Info &setName (char const *name)   {nameL = name;   return *this;}
            Info &setName (char        name)   {nameS = name;   return *this;}

            char const *descL = nullptr;
            char const *descS = nullptr;
            char const *group = nullptr;
            char const *nameL = nullptr;
            char        nameS = '\0';
         };


         Base(Base const &) = delete;
         Base(Base &&) = delete;

         Base &operator = (Base const &) = delete;
         Base &operator = (Base &&) = delete;

         void insert(Program *list);

         std::size_t process(Args const &args);

         void remove();

         Info const info;

         Program *list;

         bool processed : 1;


         friend class Program;

      protected:
         Base(Program *list, Info const &info);
         virtual ~Base();

         virtual std::size_t v_process(Args const &args) = 0;

      private:
         Base *hashNext, *hashPrev;
         Base *listNext, *listPrev;
         Base *shrtNext, *shrtPrev;

         std::size_t const hash;
         std::size_t const lenL;
      };
   }
}

#endif//GDCC__Option__Base_H__

