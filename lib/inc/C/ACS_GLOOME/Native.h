/*
The MIT License (MIT)

Copyright (c) 2015 Benjamin Moir

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// Header for target library libacs.
// "Native" functions from GLOOME's ACS.

#ifndef __GDCC_Header__C__ACS_GLOOME__Native_h__
#define __GDCC_Header__C__ACS_GLOOME__Native_h__

#include <ACS_Zandronum/Native.h>

// Global Functions
ACS_NativeDeclFull(11192, 3, int,    ChangeFlag, int, __str, int, int);
//ACS_NativeDeclFull(11193, 0, int,    SetTicrate, int);
ACS_NativeDeclFull(11194, 0, int,    GetTicrate, void);
ACS_NativeDeclFull(11195, 0, __str,  GetActorFloorTexture, int);
ACS_NativeDeclFull(11196, 0, __str,  GetActorCeilingTexture, int);
ACS_NativeDeclFull(11197, 0, int,    SetPlayerBobMul, _Accum, int);
ACS_NativeDeclFull(11198, 0, int,    GetPlayerBobMul, int);
ACS_NativeDeclFull(11199, 0, int,    SetGameProperty, int, int);
ACS_NativeDeclFull(11200, 0, int,    GetGameProperty, int);
ACS_NativeDeclFull(11201, 6, int,    Warp, int, _Accum, _Accum, _Accum, _Accum, int, __str, int);
ACS_NativeDeclFull(11202, 0, int,    SetInputLock, int);
ACS_NativeDeclFull(11203, 0, int,    GetInputLock, void);
ACS_NativeDeclFull(11204, 0, int,    KeyIsBoundSym, int, __str);
ACS_NativeDeclFull(11205, 0, __str,  ReadUserData, __str);
ACS_NativeDeclFull(11206, 0, int,    ReadUserDataChar, __str, int);
ACS_NativeDeclFull(11207, 0, int,    Squat, int, _Accum);
ACS_NativeDeclFull(11028, 0, int,    GetUserDataSize, __str);
ACS_NativeDeclFull(11029, 0, int,    GetThingSpecial, int, int);

#endif //__GDCC_Header__C__ACS_GLOOME__Native_h__

