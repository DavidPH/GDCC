//-----------------------------------------------------------------------------
//
// Copyright(C) 2015 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// General utilities.
//
// Searching and sorting utilities.
//
//-----------------------------------------------------------------------------

#include <stdlib.h>

#include <GDCC.h>
#include <string.h>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// SortGnome
//
static void SortGnome(char *base, char *end, size_t size,
   int (*compar)(void const *, void const *))
{
   char *itr = base;

   // Treat end as the actual last element for looptermination purposes.
   end -= size;

   // Gnome sort!
   while(itr != end)
   {
      // If itr > itr+1...
      if(compar(itr, itr + size) > 0)
      {
         // Swap the two elements.
         char *swpL = itr, *swpR = itr + size;
         for(size_t n = size; n--; ++swpL, ++swpR)
         {
            char swpT = *swpL;
            *swpL = *swpR;
            *swpR =  swpT;
         }

         // Reset the iterator.
         if(itr != base) itr -= size;
      }
      else
         itr += size;
   }
}

//
// SortQuick
//
static void SortQuick(char *begin, char *end, char *pivot, size_t size,
   int (*compar)(void const *, void const *))
{
   // Copy pivot value.
   memcpy(pivot, begin + (((end - begin) / (ptrdiff_t)size) >> 1) * size, size);

   // Do partitioning.
   char *itrL = begin, *itrR = end - size;
   for(;;)
   {
      while(compar(itrL, pivot) < 0) itrL += size;
      while(compar(pivot, itrR) < 0) itrR -= size;

      if(itrL > itrR) break;

      {
         // Swap the two elements.
         char *swpL = itrL, *swpR = itrR;
         for(size_t n = size; n--; ++swpL, ++swpR)
         {
            char swpT = *swpL;
            *swpL = *swpR;
            *swpR =  swpT;
         }
      }

      itrL += size;
      itrR -= size;
   }

   // Recurse.
   ssize_t len;

   // begin..itrL
   len = (itrL - begin) / size;

   if(len > 5)
      SortQuick(begin, itrL, pivot, size, compar);
   else if(len > 1)
      SortGnome(begin, itrL, size, compar);

   // itrL..end
   len = (end - itrL) / size;

   if(len > 5)
      SortQuick(itrL, end, pivot, size, compar);
   else if(len > 1)
      SortGnome(itrL, end, size, compar);
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//=========================================================
// Searching and sorting utilities.
//

//
// bsearch
//
void *bsearch(void const *key, void const *base, size_t nmemb, size_t size,
   int (*compar)(void const *, void const *))
{
   int cmp;
   char const *begin = base, *end = begin + nmemb*size, *itr;

   while(begin != end)
   {
      // itr = begin + (end - begin) / 2
      itr = begin + ((((end - begin) / size) / 2) * size);

      if(!(cmp = compar(key, itr))) return (void *)itr;

      if(cmp < 0) end = itr; else begin = itr;
   }

   return NULL;
}

//
// qsort
//
void qsort(void *base, size_t nmemb, size_t size,
   int (*compar)(void const *, void const *))
{
   if(!nmemb || nmemb == 1) return;

   // Use gnome sort for small arrays.
   if(nmemb < 5)
   {
      SortGnome(base, (char *)base + nmemb * size, size, compar);
      return;
   }

   char pivotBuf[64], *pivot;

   // Set up pivot storage for quicksort.
   if(size <= sizeof(pivotBuf))
      pivot = pivotBuf;
   else
      pivot = malloc(size);

   SortQuick(base, (char *)base + nmemb * size, pivot, size, compar);

   if(size > sizeof(pivotBuf))
      free(pivot);
}

// EOF

