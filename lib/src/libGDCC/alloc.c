//-----------------------------------------------------------------------------
//
// Copyright(C) 2014 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Memory allocation routines.
//
//-----------------------------------------------------------------------------

#include <GDCC.h>

#if __GDCC_Target__ZDoom__
#include <ACS_ZDoom.h>
#endif


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// __GDCC__AllocSize
//
// Controls the total size of the allocation heap, in bytes.
//
#ifndef __GDCC__AllocSize
#define __GDCC__AllocSize (256*1024*1024)
#endif

//
// __GDCC__MinSplit
//
// Blocks will not be split if the remainder would be smaller than this.
//
#ifndef __GDCC__MinSplit
#define __GDCC__MinSplit (sizeof(MemBlock) * 2 + sizeof(int) * 4)
#endif

//
// MemBlockFlag_*
//
#define MemBlockFlag_Auto 0x00000001
#define MemBlockFlag_Used 0x00000002

//
// MemBlock_IsUsed
//
#define MemBlock_IsUsed(block) ((block)->flag & MemBlockFlag_Used)

//
// PtrToBlock
//
#define PtrToBlock(ptr) ((MemBlockPtr)((CharPtr)(ptr) - sizeof(MemBlock)))


//----------------------------------------------------------------------------|
// Types                                                                      |
//

struct MemBlock;

typedef struct MemBlock MemBlock;

typedef MemBlock __sta *MemBlockPtr;
typedef char     __sta *CharPtr;
typedef void     __sta *VoidPtr;

//
// MemBlock
//
struct MemBlock
{
   MemBlockPtr next, prev;

   size_t size;
   size_t flag;

   char data[];
};


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

//_Alignas(MemBlock)
[[no_init]]
static char AllocHeapRaw[__GDCC__AllocSize];

[[no_init]]
static MemBlockPtr AllocBase, AllocIter;

#if __GDCC_Target__ZDoom__
static int AllocTime;
#endif


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// AllocDel
//
[[call("StkCall")]]
static void AllocDel(register MemBlockPtr block)
{
   register MemBlockPtr next = block->next;
   register MemBlockPtr prev = block->prev;

   // Determine which neighbors are free and can be merged.
   register int nextFree = next  != AllocBase && !MemBlock_IsUsed(next);
   register int prevFree = block != AllocBase && !MemBlock_IsUsed(prev);

   if(prevFree)
   {
      // Both neighbors free.
      if(nextFree)
      {
         if(AllocIter == block || AllocIter == next)
            AllocIter = prev;

         (prev->next = next->next)->prev = prev;

         prev->size += block->size + next->size + sizeof(MemBlock) * 2;
      }

      // Only prev free.
      else
      {
         if(AllocIter == block)
            AllocIter = prev;

         prev->next = next;
         next->prev = prev;

         prev->size += block->size + sizeof(MemBlock);
      }
   }
   else
   {
      // Only next free.
      if(nextFree)
      {
         if(AllocIter == next)
            AllocIter = prev;

         (block->next = next->next)->prev = block;

         block->size += next->size + sizeof(MemBlock);

         block->flag = 0;
      }

      // No neighbor free.
      else
      {
         block->flag = 0;
      }
   }
}

//
// AllocDelAuto
//
[[call("StkCall")]]
static void AllocDelAuto(void)
{
   MemBlockPtr iter = AllocBase, next;

   do
   {
      next = iter->next;

      if(iter->flag & MemBlockFlag_Auto)
         AllocDel(iter);
   }
   while((iter = next) != AllocBase);
}

//
// AllocInit
//
[[call("StkCall")]]
static void AllocInit(void)
{
   AllocBase = AllocIter = (MemBlockPtr)AllocHeapRaw;

   AllocBase->next = AllocBase->prev = AllocBase;
   AllocBase->size = __GDCC__AllocSize - sizeof(MemBlock);
   AllocBase->flag = 0;
}

//
// AllocMerge
//
[[call("StkCall")]]
static _Bool AllocMerge(register MemBlockPtr block, register size_t size)
{
   // Check if next block is free and contains enough space.
   register MemBlockPtr blockNext = block->next;
   if(blockNext == AllocBase || MemBlock_IsUsed(blockNext))
      return 0;

   // Would the combined block have enough space?
   register size_t sizeFull = block->size + blockNext->size + sizeof(MemBlock);
   if((ssize_t)sizeFull < (ssize_t)size)
      return 0;

   register MemBlockPtr blockNextNext = blockNext->next;
   register size_t      sizeDiff      = sizeFull - size;

   if(AllocIter == blockNext)
      AllocIter = block;

   // Big enough to split.
   if((ssize_t)sizeDiff > (ssize_t)__GDCC__MinSplit)
   {
      // Place new block at end of current block.
      blockNext = (MemBlockPtr)(block->data + size);

      // Initialize new block.
      blockNext->prev = block;
      blockNext->next = blockNextNext;
      blockNext->size = sizeDiff - sizeof(MemBlock);
      blockNext->flag = 0;

      // Update neighboring blocks.
      block->size         = size;
      block->next         = blockNext;
      blockNextNext->prev = blockNext;
   }

   // No, just merge.
   else
   {
      block->size         = sizeFull;
      block->next         = blockNextNext;
      blockNextNext->prev = block;
   }

   return 1;
}

//
// AllocNew
//
[[call("StkCall")]]
static VoidPtr AllocNew(register size_t size)
{
   // TODO: Round size up to alignment of MemBlock.

   register MemBlockPtr iter = AllocIter;

   do
   {
      // Skip used blocks.
      if(MemBlock_IsUsed(iter)) continue;

      // Exact size match!
      if(iter->size == size)
      {
         iter->flag = MemBlockFlag_Used;

         AllocIter = iter->next;
         return iter->data;
      }

      // Bigger, possibly split?
      if((ssize_t)iter->size > (ssize_t)size)
      {
         register size_t sizeDiff = iter->size - size;

         // Big enough to split.
         if((ssize_t)sizeDiff > (ssize_t)__GDCC__MinSplit)
         {
            // Place new block at end of current block.
            MemBlockPtr newBlock = PtrToBlock(iter->data + sizeDiff);

            // Initialize new block.
            newBlock->prev = iter;
            newBlock->next = iter->next;
            newBlock->size = size;
            newBlock->flag = MemBlockFlag_Used;

            // Update neighboring blocks.
            iter->size       = sizeDiff - sizeof(MemBlock);
            iter->next->prev = newBlock;
            iter->next       = newBlock;

            // Return allocation.
            AllocIter = iter;
            return newBlock->data;
         }

         // No, just use it as-is.
         else
         {
            iter->flag = MemBlockFlag_Used;

            AllocIter = iter->next;
            return iter->data;
         }
      }

      // Not big enough, move along.
   }
   while((iter = iter->next) != AllocIter);

   // No space found, give up.
   return 0;
}

//
// AllocTimeSet
//
#if __GDCC_Target__ZDoom__
[[addres(999), call("ScriptI"), script("Open")]]
static void AllocTimeSet(void)
{
   if(!ACS_Timer())
      ACS_Delay(1);

   if(ACS_Timer() == 1)
      AllocTime = 1;
}
#endif


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//
// __GDCC__alloc
//
[[call("StkCall")]]
VoidPtr __GDCC__alloc(register VoidPtr ptrOld, register size_t size)
{
   if(!AllocIter) AllocInit();

   // No existing allocation.
   if(!ptrOld)
      return size ? AllocNew(size) : 0;

   register MemBlockPtr block = PtrToBlock(ptrOld);

   // Size 0 means free.
   if(!size)
      return AllocDel(block), 0;

   // If block already has enough space, then no need to realloc.
   if((ssize_t)block->size >= (ssize_t)size)
      return ptrOld;

   // Try a merging block expansion.
   if(AllocMerge(block, size))
      return ptrOld;

   // Fallback to simply allocate new block and memcpy.
   register CharPtr ptrNew = AllocNew(size);

   if(!ptrNew) return 0;

   for(register CharPtr itrNew = ptrNew, itrOld = ptrOld,
      end = itrOld + block->size; itrOld != end;)
      *itrNew++ = *itrOld++;

   AllocDel(block);
   return ptrNew;
}

//
// __GDCC__alloc_dump
//
[[call("StkCall")]]
void __GDCC__alloc_dump(void)
{
   if(!AllocIter) AllocInit();

   #if __GDCC_Target__ZDoom__
   MemBlockPtr iter = AllocBase;
   do
   {
      ACS_BeginPrint();

      ACS_PrintHex((unsigned)iter);
      ACS_PrintChar(' ');
      ACS_PrintHex((unsigned)iter->next);
      ACS_PrintChar(' ');
      ACS_PrintHex((unsigned)iter->prev);
      ACS_PrintChar(' ');
      ACS_PrintHex(iter->size);
      ACS_PrintChar(' ');
      ACS_PrintHex(iter->flag);

      ACS_EndLog();
   }
   while((iter = iter->next) != AllocBase);
   #endif
}

//
// __GDCC__Plsa
//
#if __GDCC_Target__ZDoom__
[[call("StkCall")]]
VoidPtr __GDCC__Plsa(unsigned int size)
{
   // Check if a new hub was entered. If so, free automatic storage.
   if(AllocTime > ACS_Timer())
      AllocDelAuto();
   AllocTime = ACS_Timer();

   MemBlockPtr block = PtrToBlock(__GDCC__alloc(0, size));

   block->flag |= MemBlockFlag_Auto;

   return block->data;
}
#endif

//
// __GDCC__Plsf
//
#if __GDCC_Target__ZDoom__
[[call("StkCall")]]
void __GDCC__Plsf(VoidPtr ptr)
{
   AllocDel(PtrToBlock(ptr));
}
#endif

// EOF

