//-----------------------------------------------------------------------------
//
// Copyright(C) 2016-2017 David Hill
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
// Types                                                                      |
//

typedef void (*DGE_CallbackType)(int w, int h) [[call("StkCall")]];

enum // DGE_Align
{
   DGE_Align_Keep,

   DGE_Align_Left = 1,
   DGE_Align_Right,
   DGE_Align_Center,
   DGE_Align_RightBox,
   DGE_Align_CenterBox,

   DGE_Align_Top = 1,
   DGE_Align_Bottom,
 //DGE_Align_Center,
};


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

[[call("Native")]] void DGE_CallbackRegister(char const *name,
   void (*callback)(void) [[call("StkCall")]]);
[[call("Native")]] void DGE_CallbackUnregister(char const *name,
   void (*callback)(void) [[call("StkCall")]]);

[[call("Native")]] void DGE_DebugCallStk(void);
[[optional_args(1)]]
[[call("Native")]] void DGE_DebugDataStk(int dump);
[[call("Native")]] void DGE_DebugLocReg(void);

[[call("Native")]] void DGE_Delay(unsigned ticks);

[[optional_args(1)]]
[[call("Native")]] void DGE_DrawColor(unsigned long _Fract r,
   unsigned long _Fract g, unsigned long _Fract b, unsigned long _Fract a);
[[call("Native")]] void DGE_DrawCircle(int x, int y, int r);
[[call("Native")]] void DGE_DrawCircleLine(int x, int y, int r);
[[call("Native")]] void DGE_DrawEllipse(int x1, int y1, int x2, int y2);
[[call("Native")]] void DGE_DrawEllipseLine(int x1, int y1, int x2, int y2);
[[call("Native")]] void DGE_DrawRectangle(int x1, int y1, int x2, int y2);
[[call("Native")]] void DGE_DrawRectangleLine(int x1, int y1, int x2, int y2);
[[call("Native")]] void DGE_DrawLine(int x1, int y1, int x2, int y2);
[[call("Native")]] void DGE_DrawText(int x, int y, char const *str);
[[call("Native")]] void DGE_DrawTexture(unsigned tex);
[[call("Native")]] void DGE_DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
[[call("Native")]] void DGE_DrawTriangleLine(int x1, int y1, int x2, int y2, int x3, int y3);

[[call("Native")]] void DGE_FileClose(int fd);
[[call("Native")]] int DGE_FileGet(int fd, unsigned idx);
[[call("Native")]] int DGE_FileOpen(char const *name);
[[call("Native")]] int DGE_FileRead(int fd, unsigned idx, char *buf, unsigned len);
[[call("Native")]] unsigned DGE_FileSize(int fd);

[[call("Native")]] void *DGE_FreestoreBegin(void);
[[call("Native")]] void *DGE_FreestoreEnd(void);

[[call("Native")]] unsigned DGE_GetTexture(__str name);

[[call("Native")]] void DGE_PrintChar(unsigned c);
[[call("Native")]] void DGE_PrintWordD(unsigned d);

[[call("Native")]] void DGE_SetWindowTitle(char const *str);

[[call("Native")]] unsigned DGE_SysRead(char *buf, unsigned len);
[[call("Native")]] unsigned DGE_SysWrite(char const *buf, unsigned len);
[[call("Native")]] unsigned DGE_SysWriteErr(char const *buf, unsigned len);

[[optional_args(1)]]
[[call("Native")]] void DGE_TextAlignment(int h, int v);

#endif//__GDCC_Header__C__Doominati_h__

