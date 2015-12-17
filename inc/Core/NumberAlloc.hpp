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
      template<typename T>
      class NumberAllocMerge;

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

            bool used : 1;


            friend class NumberAlloc;
            friend class NumberAllocMerge<T>;

         private:
            // default constructor
            Block() : next{this}, prev{this} {}

            //
            // constructor
            //
            explicit Block(Block *next_) :
               lo{0}, hi{0}, size{0}, used{false},
               next{next_}, prev{next_->prev}
            {
               next->prev = prev->next = this;
            }

            //
            // constructor
            //
            Block(Block *next_, T const &lo_, T const &size_, bool used_) :
               lo{lo_}, hi{lo_ + size_}, size{size_}, used{used_},
               next{next_}, prev{next_->prev}
            {
               next->prev = prev->next = this;
            }

            // destructor
            ~Block() {next->prev = prev; prev->next = next;}

            Block *next, *prev;
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
                     new Block(&iter, iter.lo, size, true);
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
            return new Block(&head, last.hi, size, true);
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

      //
      // NumberAllocMerge
      //
      // Number allocator that merges neighboring ranges and can accept
      // explicit address allocations. However, allocations cannot be freed.
      //
      template<typename T>
      class NumberAllocMerge
      {
      public:
         using Block = typename NumberAlloc<T>::Block;

         using value_type       = Block;
         using const_value_type = value_type const;
         using const_iterator   = Core::ListItr<const_value_type,
            &const_value_type::prev, &const_value_type::next>;
         using iterator         = Core::ListItr<value_type, &value_type::prev,
            &value_type::next>;


         //
         // constructor
         //
         NumberAllocMerge()
         {
            new Block(&head);
         }

         //
         // destructor
         //
         ~NumberAllocMerge()
         {
            while(head.next != &head)
               delete head.next;
         }

         //
         // alloc
         //
         T alloc(T const &size)
         {
            alloc(size, 0);
         }

         //
         // alloc
         //
         T alloc(T const &size, T const &min)
         {
            // Look for an unused allocation.
            for(auto &block : *this)
            {
               if(block.used || block.size < size || block.lo < min)
                  continue;

               T addr = block.lo;
               allocAt(size, addr, &block);
               return addr;
            }

            Block &block = back();

            // If last allocation is unused, extend it.
            if(!block.used)
            {
               T addr = block.lo;
               if(addr < min) addr = min;
               allocAt(size, addr, &block);
               return addr;
            }

            if(block.hi < min)
            {
               allocAt(size, min, new Block(&head, block.hi, size, true));
               return min;
            }
            else
               return (new Block(&head, block.hi, size, true))->lo;
         }

         //
         // allocAt
         //
         void allocAt(T const &size, T const &addr)
         {
            for(auto &block : *this)
            {
               if(block.lo > addr || addr >= block.hi)
                  continue;

               allocAt(size, addr, &block);
               return;
            }

            if(back().used)
               allocAt(size, addr, new Block(&head, head.prev->hi, size, true));
            else
               allocAt(size, addr, &back());

         }

         // begin
         const_iterator begin() const {return static_cast<const_iterator>(head.next);}
               iterator begin()       {return static_cast<      iterator>(head.next);}

         // end
         const_iterator end() const {return static_cast<const_iterator>(&head);}
               iterator end()       {return static_cast<      iterator>(&head);}

      private:
         //
         // allocAt
         //
         void allocAt(T const &size, T const &lo, Block *block)
         {
            T hi = lo + size;

            // Possibly extend block forward.
            if(block->hi < hi)
            {
               block->hi   = hi;
               block->size = block->hi - block->lo;
            }

            // If not currently a used block, might need to split.
            if(!block->used)
            {
               // Check for splitting off high part.
               if(block->hi > hi)
               {
                  new Block(block, block->lo, hi - block->lo, false);

                  block->lo   = hi;
                  block->size = block->hi - block->lo;

                  block = block->prev;
               }

               // Check for splitting off low part.
               if(block->lo < lo)
               {
                  new Block(block, block->lo, lo - block->lo, false);

                  block->lo   = lo;
                  block->size = block->hi - block->lo;
               }

               block->used = true;
            }

            // If there is a gap between this and previous block, fill it.
            if(block->prev != &head && block->prev->hi < block->lo)
               new Block(block, block->prev->hi, block->lo - block->prev->hi, false);

            // If previous block is used, merge with it.
            if(block->prev != &head && block->prev->used)
            {
               Block *prev = block->prev;
               prev->hi    = block->hi;
               prev->size += block->size;
               delete block;
               block = prev;
            }

            // If block overlaps next block(s), merge with them.
            while(block->next != &head && block->hi > block->next->lo)
            {
               // Partial overlap?
               if(block->hi < block->next->hi)
               {
                  block->next->lo   = block->hi;
                  block->next->size = block->next->hi - block->next->lo;
               }
               else
                  delete block->next;
            }

            // If next block is used, merge with it.
            if(block->next != &head && block->next->used)
            {
               block->hi    = block->next->hi;
               block->size += block->next->size;
               delete block->next;
            }
         }

         Block &back() {return *head.prev;}

         Block head;
      };
   }
}

#endif//GDCC__Core__NumberAlloc_H__

