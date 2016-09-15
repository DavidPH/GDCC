//-----------------------------------------------------------------------------
//
// Copyright(C) 2016 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libdge.
//
// Doominati confirmed.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__Doominati_h__
#define __GDCC_Header__C__Doominati_h__


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

[[call("Native")]] void DGE_FileClose(int fd);
[[call("Native")]] int DGE_FileGet(int fd, unsigned idx);
[[call("Native")]] int DGE_FileOpen(char const *name);
[[call("Native")]] int DGE_FileRead(int fd, unsigned idx, char *buf, unsigned len);
[[call("Native")]] unsigned DGE_FileSize(int fd);

[[call("Native")]] void *DGE_FreestoreBegin(void);
[[call("Native")]] void *DGE_FreestoreEnd(void);

[[call("Native")]] void DGE_PrintChar(unsigned c);
[[call("Native")]] void DGE_PrintWordD(unsigned d);

[[call("Native")]] void DGE_Delay(unsigned ticks);

#endif//__GDCC_Header__C__Doominati_h__

