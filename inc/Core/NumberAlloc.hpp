//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Linear numeric range allocation.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__NumberAlloc_H__
#define GDCC__Core__NumberAlloc_H__

#include "../Core/List.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      //
      // NumberAlloc
      //
      template<typename T>
      class NumberAlloc
      {
      public:
         //
         // Block
         //
         class Block
         {
         public:
            T lo, hi;
            T size;


            friend class NumberAlloc;

         private:
            // default constructor
            Block() : next{this}, prev{this} {}

            //
            // constructor
            //
            explicit Block(Block *next_) :
               lo{0}, hi{0}, size{0},
               next{next_}, prev{next_->prev},
               used{false}
            {
               next->prev = prev->next = this;
            }

            //
            // constructor
            //
            Block(Block *next_, T const &lo_, T const &size_) :
               lo{lo_}, hi{lo_ + size_}, size{size_},
               next{next_}, prev{next_->prev},
               used{true}
            {
               next->prev = prev->next = this;
            }

            // destructor
            ~Block() {next->prev = prev; prev->next = next;}

            Block *next, *prev;

            bool used : 1;
         };

         using value_type       = Block;
         using const_value_type = value_type const;
         using const_iterator   = Core::ListItr<const_value_type,
            &const_value_type::prev, &const_value_type::next>;
         using iterator         = Core::ListItr<value_type, &value_type::prev,
            &value_type::next>;


         //
         // constructor
         //
         NumberAlloc()
         {
            new Block(&head);
         }

         //
         // destructor
         //
         ~NumberAlloc()
         {
            while(head.next != &head)
               delete head.next;
         }

         //
         // alloc
         //
         Block const *alloc(T const &size)
         {
            // Look for an unused allocation.
            for(auto &iter : *this)
            {
               if(!iter.used)
               {
                  // Exact size, use as-is.
                  if(iter.size == size)
                  {
                     iter.used = true;
                     return &iter;
                  }
                  // Bigger, so split the allocation.
                  else if(iter.size > size)
                  {
                     new Block(&iter, iter.lo, size);
                     iter.lo   += size;
                     iter.size -= size;
                     return iter.prev;
                  }
                  // Otherwise, continue searching.
               }
            }

            Block &last = back();

            // If last allocation is unused, extend it.
            if(!last.used)
            {
               last.hi   = last.lo + size;
               last.size = size;
               last.used = true;
               return &last;
            }

            // Otherwise, add a new allocation.
            return new Block(&head, last.hi, size);
         }

         // begin
         const_iterator begin() const {return static_cast<const_iterator>(head.next);}
               iterator begin()       {return static_cast<      iterator>(head.next);}

         // end
         const_iterator end() const {return static_cast<const_iterator>(&head);}
               iterator end()       {return static_cast<      iterator>(&head);}

         //
         // free
         //
         void free(Block const *iter_)
         {
            iterator iter{const_cast<Block *>(iter_)};
            auto prev = iter, next = iter;

            // Determine which neighbors are free.
            bool prevFree = prev != begin() && !(--prev)->used;
            bool nextFree = ++next != end() && !next->used;

            if(prevFree)
            {
               // Both neighbors free.
               if(nextFree)
               {
                  prev->hi    = next->hi;
                  prev->size += iter->size + next->size;

                  delete &*iter;
                  delete &*next;
               }

               // Only prev free.
               else
               {
                  prev->hi    = iter->hi;
                  prev->size += iter->size;

                  delete &*iter;
               }
            }
            else
            {
               // Only next free.
               if(nextFree)
               {
                  iter->hi    = next->hi;
                  iter->size += next->size;
                  iter->used  = false;

                  delete &*next;
               }

               // No neighbor free.
               else
               {
                  iter->used = false;
               }
            }
         }

         //
         // max
         //
         T max() const
         {
            return head.prev->hi;
         }

      private:
         Block &back() {return *head.prev;}

         Block head;
      };
   }
}

#endif//GDCC__Core__NumberAlloc_H__

