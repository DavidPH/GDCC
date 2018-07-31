//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Numeric typedefs and utilities.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Config_H__
#define GDCC__Config_H__


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#cmakedefine01 GDCC_BC_DGE
#cmakedefine01 GDCC_BC_ZDACS
#cmakedefine01 GDCC_Core_BigNum

#define GDCC_Core_SizeLong     ${GDCC_Core_SizeLong}
#define GDCC_Core_SizeLongLong ${GDCC_Core_SizeLongLong}

#endif//GDCC__Config_H__

