//-----------------------------------------------------------------------------
//
// Copyright(C) 2014-2023 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Memory allocation routines.
//
//-----------------------------------------------------------------------------

#include <GDCC.h>

#if __GDCC_Family__ZDACS__
#include <ACS_ZDoom.h>
#endif

#if __GDCC_Engine__Doominati__
#include <Doominati.h>
#endif


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// __GDCC__AllocAlign
//
// Controls the minimum alignment requirement of allocations.
//
#ifndef __GDCC__AllocAlign
#define __GDCC__AllocAlign (_Alignof(MemBlock))
#endif

//
// __GDCC__AllocSize
//
// Controls the total size of the allocation heap, in bytes.
//
#ifndef __GDCC__AllocSize
#define __GDCC__AllocSize (2*1024*1024*1024u)
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

   __size_t size;
   __size_t flag;

   char data[];
};


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

#if __GDCC_Family__ZDACS__
//_Alignas(MemBlock)
[[no_init]]
static char AllocHeapRaw[__GDCC__AllocSize];
#endif

static MemBlockPtr AllocBase, AllocIter;

#if __GDCC_Family__ZDACS__
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
   __size_t allocSize;

   #if __GDCC_Family__ZDACS__
   allocSize = __GDCC__AllocSize;
   AllocBase = AllocIter = (MemBlockPtr)AllocHeapRaw;
   #elif __GDCC_Engine__Doominati__
   allocSize = (char *)DGE_FreestoreEnd() - (char *)DGE_FreestoreBegin();
   AllocBase = AllocIter = (MemBlockPtr)DGE_FreestoreBegin();
   #endif

   AllocBase->next = AllocBase->prev = AllocBase;
   AllocBase->size = allocSize - sizeof(MemBlock);
   AllocBase->flag = 0;
}

//
// AllocMerge
//
[[call("StkCall")]]
static _Bool AllocMerge(register MemBlockPtr block, register __size_t size)
{
   // Check if next block is free and contains enough space.
   register MemBlockPtr blockNext = block->next;
   if(blockNext == AllocBase || MemBlock_IsUsed(blockNext))
      return 0;

   // Would the combined block have enough space?
   register __size_t sizeFull = block->size + blockNext->size + sizeof(MemBlock);
   if(sizeFull < size)
      return 0;

   register MemBlockPtr blockNextNext = blockNext->next;
   register __size_t    sizeDiff      = sizeFull - size;

   if(AllocIter == blockNext)
      AllocIter = block;

   // Big enough to split.
   if(sizeDiff > __GDCC__MinSplit)
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
static VoidPtr AllocNew(register __size_t size)
{
   // Round size up to alignment of MemBlock.
   size = (size + (__GDCC__AllocAlign - 1)) & ~(__GDCC__AllocAlign - 1);

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
      if(iter->size > size)
      {
         register __size_t sizeDiff = iter->size - size;

         // Big enough to split.
         if(sizeDiff > __GDCC__MinSplit)
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
#if __GDCC_Family__ZDACS__
[[call("ScriptS"), script("open")]]
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
VoidPtr __GDCC__alloc(register VoidPtr ptrOld, register __size_t size)
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
   if(block->size >= size)
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

   #if __GDCC_Family__ZDACS__
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
[[call("StkCall")]]
VoidPtr __GDCC__Plsa(unsigned int size)
{
   #if __GDCC_Family__ZDACS__
   // Check if a new hub was entered. If so, free automatic storage.
   if(AllocTime > ACS_Timer())
      AllocDelAuto();
   AllocTime = ACS_Timer();
   #endif

   MemBlockPtr block = PtrToBlock(__GDCC__alloc(0, size));

   block->flag |= MemBlockFlag_Auto;

   return block->data;
}

//
// __GDCC__Plsf
//
[[call("StkCall")]]
void __GDCC__Plsf(VoidPtr ptr)
{
   AllocDel(PtrToBlock(ptr));
}

// EOF

