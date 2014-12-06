//-----------------------------------------------------------------------------
//
// Copyright(C) 2014 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Character handling.
//
//-----------------------------------------------------------------------------

#include <ctype.h>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#define CharMax 0x7F

#define CT_alpha 0x00000001
#define CT_blank 0x00000002
#define CT_cntrl 0x00000004
#define CT_digit 0x00000008
#define CT_graph 0x00000010
#define CT_lower 0x00000020
#define CT_print 0x00000040
#define CT_punct 0x00000080
#define CT_space 0x00000100
#define CT_upper 0x00000200
#define CT_xdigi 0x00000400


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

//
// CharTab
//
static unsigned int const CharTab[] =
{
   /* 00 - NUL */ CT_cntrl,
   /* 01 - SOH */ CT_cntrl,
   /* 02 - STX */ CT_cntrl,
   /* 03 - ETX */ CT_cntrl,
   /* 04 - EOT */ CT_cntrl,
   /* 05 - ENQ */ CT_cntrl,
   /* 06 - ACK */ CT_cntrl,
   /* 07 - BEL */ CT_cntrl,
   /* 08 - BS  */ CT_cntrl,
   /* 09 - HT  */ CT_blank | CT_cntrl | CT_space,
   /* 0A - LF  */ CT_cntrl | CT_space,
   /* 0B - VT  */ CT_cntrl | CT_space,
   /* 0C - FF  */ CT_cntrl | CT_space,
   /* 0D - CR  */ CT_cntrl | CT_space,
   /* 0E - SO  */ CT_cntrl,
   /* 0F - SI  */ CT_cntrl,
   /* 10 - DLE */ CT_cntrl,
   /* 11 - DC1 */ CT_cntrl,
   /* 12 - DC2 */ CT_cntrl,
   /* 13 - DC3 */ CT_cntrl,
   /* 14 - DC4 */ CT_cntrl,
   /* 15 - NAK */ CT_cntrl,
   /* 16 - SYN */ CT_cntrl,
   /* 17 - ETB */ CT_cntrl,
   /* 18 - CAN */ CT_cntrl,
   /* 19 - EM  */ CT_cntrl,
   /* 1A - SUB */ CT_cntrl,
   /* 1B - ESC */ CT_cntrl,
   /* 1C - FS  */ CT_cntrl,
   /* 1D - GS  */ CT_cntrl,
   /* 1E - RS  */ CT_cntrl,
   /* 1F - US  */ CT_cntrl,
   /* 20 - ' ' */ CT_blank | CT_print | CT_space,
   /* 21 - '!' */ CT_graph | CT_print | CT_punct,
   /* 22 - '"' */ CT_graph | CT_print | CT_punct,
   /* 23 - '#' */ CT_graph | CT_print | CT_punct,
   /* 24 - '$' */ CT_graph | CT_print | CT_punct,
   /* 25 - '%' */ CT_graph | CT_print | CT_punct,
   /* 26 - '&' */ CT_graph | CT_print | CT_punct,
   /* 27 - ''' */ CT_graph | CT_print | CT_punct,
   /* 28 - '(' */ CT_graph | CT_print | CT_punct,
   /* 29 - ')' */ CT_graph | CT_print | CT_punct,
   /* 2A - '*' */ CT_graph | CT_print | CT_punct,
   /* 2B - '+' */ CT_graph | CT_print | CT_punct,
   /* 2C - ',' */ CT_graph | CT_print | CT_punct,
   /* 2D - '-' */ CT_graph | CT_print | CT_punct,
   /* 2E - '.' */ CT_graph | CT_print | CT_punct,
   /* 2F - '/' */ CT_graph | CT_print | CT_punct,
   /* 30 - '0' */ CT_digit | CT_graph | CT_print | CT_xdigi,
   /* 31 - '1' */ CT_digit | CT_graph | CT_print | CT_xdigi,
   /* 32 - '2' */ CT_digit | CT_graph | CT_print | CT_xdigi,
   /* 33 - '3' */ CT_digit | CT_graph | CT_print | CT_xdigi,
   /* 34 - '4' */ CT_digit | CT_graph | CT_print | CT_xdigi,
   /* 35 - '5' */ CT_digit | CT_graph | CT_print | CT_xdigi,
   /* 36 - '6' */ CT_digit | CT_graph | CT_print | CT_xdigi,
   /* 37 - '7' */ CT_digit | CT_graph | CT_print | CT_xdigi,
   /* 38 - '8' */ CT_digit | CT_graph | CT_print | CT_xdigi,
   /* 39 - '9' */ CT_digit | CT_graph | CT_print | CT_xdigi,
   /* 3A - ':' */ CT_graph | CT_print | CT_punct,
   /* 3B - ';' */ CT_graph | CT_print | CT_punct,
   /* 3C - '<' */ CT_graph | CT_print | CT_punct,
   /* 3D - '=' */ CT_graph | CT_print | CT_punct,
   /* 3E - '>' */ CT_graph | CT_print | CT_punct,
   /* 3F - '?' */ CT_graph | CT_print | CT_punct,
   /* 40 - '@' */ CT_graph | CT_print | CT_punct,
   /* 41 - 'A' */ CT_alpha | CT_graph | CT_print | CT_upper | CT_xdigi,
   /* 42 - 'B' */ CT_alpha | CT_graph | CT_print | CT_upper | CT_xdigi,
   /* 43 - 'C' */ CT_alpha | CT_graph | CT_print | CT_upper | CT_xdigi,
   /* 44 - 'D' */ CT_alpha | CT_graph | CT_print | CT_upper | CT_xdigi,
   /* 45 - 'E' */ CT_alpha | CT_graph | CT_print | CT_upper | CT_xdigi,
   /* 46 - 'F' */ CT_alpha | CT_graph | CT_print | CT_upper | CT_xdigi,
   /* 47 - 'G' */ CT_alpha | CT_graph | CT_print | CT_upper,
   /* 48 - 'H' */ CT_alpha | CT_graph | CT_print | CT_upper,
   /* 49 - 'I' */ CT_alpha | CT_graph | CT_print | CT_upper,
   /* 4A - 'J' */ CT_alpha | CT_graph | CT_print | CT_upper,
   /* 4B - 'K' */ CT_alpha | CT_graph | CT_print | CT_upper,
   /* 4C - 'L' */ CT_alpha | CT_graph | CT_print | CT_upper,
   /* 4D - 'M' */ CT_alpha | CT_graph | CT_print | CT_upper,
   /* 4E - 'N' */ CT_alpha | CT_graph | CT_print | CT_upper,
   /* 4F - 'O' */ CT_alpha | CT_graph | CT_print | CT_upper,
   /* 50 - 'P' */ CT_alpha | CT_graph | CT_print | CT_upper,
   /* 51 - 'Q' */ CT_alpha | CT_graph | CT_print | CT_upper,
   /* 52 - 'R' */ CT_alpha | CT_graph | CT_print | CT_upper,
   /* 53 - 'S' */ CT_alpha | CT_graph | CT_print | CT_upper,
   /* 54 - 'T' */ CT_alpha | CT_graph | CT_print | CT_upper,
   /* 55 - 'U' */ CT_alpha | CT_graph | CT_print | CT_upper,
   /* 56 - 'V' */ CT_alpha | CT_graph | CT_print | CT_upper,
   /* 57 - 'W' */ CT_alpha | CT_graph | CT_print | CT_upper,
   /* 58 - 'X' */ CT_alpha | CT_graph | CT_print | CT_upper,
   /* 59 - 'Y' */ CT_alpha | CT_graph | CT_print | CT_upper,
   /* 5A - 'Z' */ CT_alpha | CT_graph | CT_print | CT_upper,
   /* 5B - '[' */ CT_graph | CT_print | CT_punct,
   /* 5C - '\' */ CT_graph | CT_print | CT_punct,
   /* 5D - ']' */ CT_graph | CT_print | CT_punct,
   /* 5E - '^' */ CT_graph | CT_print | CT_punct,
   /* 5F - '_' */ CT_graph | CT_print | CT_punct,
   /* 60 - '`' */ CT_graph | CT_print | CT_punct,
   /* 61 - 'a' */ CT_alpha | CT_graph | CT_lower | CT_print | CT_xdigi,
   /* 62 - 'b' */ CT_alpha | CT_graph | CT_lower | CT_print | CT_xdigi,
   /* 63 - 'c' */ CT_alpha | CT_graph | CT_lower | CT_print | CT_xdigi,
   /* 64 - 'd' */ CT_alpha | CT_graph | CT_lower | CT_print | CT_xdigi,
   /* 65 - 'e' */ CT_alpha | CT_graph | CT_lower | CT_print | CT_xdigi,
   /* 66 - 'f' */ CT_alpha | CT_graph | CT_lower | CT_print | CT_xdigi,
   /* 67 - 'g' */ CT_alpha | CT_graph | CT_lower | CT_print,
   /* 68 - 'h' */ CT_alpha | CT_graph | CT_lower | CT_print,
   /* 69 - 'i' */ CT_alpha | CT_graph | CT_lower | CT_print,
   /* 6A - 'j' */ CT_alpha | CT_graph | CT_lower | CT_print,
   /* 6B - 'k' */ CT_alpha | CT_graph | CT_lower | CT_print,
   /* 6C - 'l' */ CT_alpha | CT_graph | CT_lower | CT_print,
   /* 6D - 'm' */ CT_alpha | CT_graph | CT_lower | CT_print,
   /* 6E - 'n' */ CT_alpha | CT_graph | CT_lower | CT_print,
   /* 6F - 'o' */ CT_alpha | CT_graph | CT_lower | CT_print,
   /* 70 - 'p' */ CT_alpha | CT_graph | CT_lower | CT_print,
   /* 71 - 'q' */ CT_alpha | CT_graph | CT_lower | CT_print,
   /* 72 - 'r' */ CT_alpha | CT_graph | CT_lower | CT_print,
   /* 73 - 's' */ CT_alpha | CT_graph | CT_lower | CT_print,
   /* 74 - 't' */ CT_alpha | CT_graph | CT_lower | CT_print,
   /* 75 - 'u' */ CT_alpha | CT_graph | CT_lower | CT_print,
   /* 76 - 'v' */ CT_alpha | CT_graph | CT_lower | CT_print,
   /* 77 - 'w' */ CT_alpha | CT_graph | CT_lower | CT_print,
   /* 78 - 'x' */ CT_alpha | CT_graph | CT_lower | CT_print,
   /* 79 - 'y' */ CT_alpha | CT_graph | CT_lower | CT_print,
   /* 7A - 'z' */ CT_alpha | CT_graph | CT_lower | CT_print,
   /* 7B - '{' */ CT_graph | CT_print | CT_punct,
   /* 7C - '|' */ CT_graph | CT_print | CT_punct,
   /* 7D - '}' */ CT_graph | CT_print | CT_punct,
   /* 7E - '~' */ CT_graph | CT_print | CT_punct,
   /* 7F - DEL */ CT_cntrl,
};


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//=========================================================
// Character classification functions.
//

//
// isalnum
//
int isalnum(int c)
{
   if(c & ~CharMax) return 0;

   return CharTab[c] & (CT_alpha | CT_digit);
}

//
// isalpha
//
int isalpha(int c)
{
   if(c & ~CharMax) return 0;

   return CharTab[c] & CT_alpha;
}

//
// isblank
//
int isblank(int c)
{
   if(c & ~CharMax) return 0;

   return CharTab[c] & CT_blank;
}

//
// iscntrl
//
int iscntrl(int c)
{
   if(c & ~CharMax) return 0;

   return CharTab[c] & CT_cntrl;
}

//
// isdigit
//
int isdigit(int c)
{
   if(c & ~CharMax) return 0;

   return CharTab[c] & CT_digit;
}

//
// isgraph
//
int isgraph(int c)
{
   if(c & ~CharMax) return 0;

   return CharTab[c] & CT_graph;
}

//
// islower
//
int islower(int c)
{
   if(c & ~CharMax) return 0;

   return CharTab[c] & CT_lower;
}

//
// isprint
//
int isprint(int c)
{
   if(c & ~CharMax) return 0;

   return CharTab[c] & CT_print;
}

//
// ispunct
//
int ispunct(int c)
{
   if(c & ~CharMax) return 0;

   return CharTab[c] & CT_punct;
}

//
// isspace
//
int isspace(int c)
{
   if(c & ~CharMax) return 0;

   return CharTab[c] & CT_space;
}

//
// isupper
//
int isupper(int c)
{
   if(c & ~CharMax) return 0;

   return CharTab[c] & CT_upper;
}

//
// isxdigit
//
int isxdigit(int c)
{
   if(c & ~CharMax) return 0;

   return CharTab[c] & CT_xdigi;
}

//=========================================================
// Character case mapping functions.
//

//
// tolower
//
int tolower(int c)
{
   if(c & ~CharMax) return c;

   return CharTab[c] & CT_upper ? c + ('a' - 'A') : c;
}

//
// toupper
//
int toupper(int c)
{
   if(c & ~CharMax) return c;

   return CharTab[c] & CT_lower ? c - ('a' - 'A') : c;
}

// EOF

