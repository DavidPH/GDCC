//-----------------------------------------------------------------------------
//
// Example C code for GDCC.
//
//-----------------------------------------------------------------------------

// Any project targeting ZDoom or other ZDACS-enabled engines will almost
// certainly need to include this, as it includes declarations for functions
// that acc declares implicitly.
#include <ACS_ZDoom.h>

#include <limits.h>
#include <stddef.h>
#include <stdfix.h>
#include <stdio.h>


// This declaration makes the DoAction function visible before its definition.
// It can also be used in other source files (usually by including a header
// file) to access functions and objects in other files.
void DoAction(void);

// This forward declares an address space.
__addrdef extern __map_arr map_var;

// And this forward declares an object in that address space.
extern int map_var MapInt;


// This declares and defines a map array address space. The name map_var is
// used as a type qualifier and all objects declared with a type qualified with
// it will be stored in the same underlying map array.
__addrdef __map_arr map_var;

// Declares and defines an int object in the map_var address space.
int map_var MapInt;

// Similarly, but for an array.
unsigned int map_var MapIntArr[16];

// If no address space is specified, objects declared at file-scope use the
// generic address space, which is implemented as a global array.
int IntVar;

// The typedef declaration can be used to give a type a new name, either for
// abstraction or convenience of typing. The declared name is identical to the
// underlying type, and either can be substituted for the other.
typedef int map_var map_int;

// This forward declaration of MapInt is equivalent to and compatible with the
// previous. Also worth noting that forward declarations can appear after the
// definition without error.
extern map_int MapInt;

// [[call("ScriptS")]] makes this function a named script.
// [[script("Open")]] gives it the OPEN type, making it an entry-point.
// By default, the name used will be the "mangled" name of the function. If the
// script needs to be referred to from outside GDCC, then [[extern("ACS")]]
// will make the mangled name the same as the declared name. Alternatively,
// [[address("Name Goes Here")]] will explicitly specify a name.
[[call("ScriptS"), script("Open")]]
void MainFunc(void)
{
   while(1)
   {
      DoAction();

      // ACS function names are prefixed to avoid name conflicts.
      ACS_Delay(ACS_Random(35, 70));
   }
}

// [[call("ScriptI")]] makes this function a numbered script. Its number will
// be automatically allocated. If a specific number is desired, then
// [[address(42)]] can be used with 42 replaced by the desired number.
// Automatically allocated numbers will avoid such explicit allocations.
[[call("ScriptI")]]
int CalcValue(void)
{
   // This declares a local pointer object. The pointer itself is stored as a
   // local variable, but the unsigned int objects that it accesses are stored
   // as map_var. Pointer declarators are best read right-to-left.
   unsigned int map_var *mapIntPtr;

   // Store the address of an element of MapIntArr in mapIntPtr.
   mapIntPtr = &MapIntArr[ACS_Timer() & 15];

   // Do stuff with the referenced element.
   *mapIntPtr += ACS_Timer() * *mapIntPtr;

   // Being functions, scripts return values and are terminated using the
   // return statement.
   return *mapIntPtr & INT_MAX;
}

void DoAction(void)
{
   // Being functions, scripts can be called like functions.
   int val = CalcValue();

   // An ACS string variable.
   __str msg = NULL;

   if(val == 42)
   {
      // ACS string literals have an s prefixed to them. Otherwise, it is a C
      // string, which is addressed differently.
      msg = s"Enjoying yourself?";
   }

   if(msg)
   {
      // This is used to initialize the native print buffer for a single print.
      ACS_BeginPrint();

      // __nprintf performs formatting the same as printf, but the output is
      // sent to the native print buffer. The format string must be a C string.
      // %S is used to print ACS strings.
      __nprintf("George says, \"%S\"", msg);

      // This prints the native print buffer to the screen as in ACS's Print.
      ACS_EndPrint();
   }
   else
   {
      // This is equivalent to a Log call in ACS. Note the trailing linefeed.
      // The buffer used by printf is only written when a linefeed is used.
      printf("Nothing to say, move along.\n");
   }
}

// This function demonstrates basic usage of floating and fixed point types.
void FloatVsFixed(void)
{
   // GDCC supports single and double precision floating point.
   float f;
   double lf;

   // The name accum is the canonical name for fixed-point (from Embedded C,
   // short for accumulator), however stdfix.h also defines fixed.
   accum k;
   fixed x;

   // The long accum type is an extended type with 1 sign bit, 31 integer bits,
   // and 32 fractional bits. It can be used for calculations needing some
   // extra intermediary precision.
   long accum lk;

   // Unsuffixed floating-point literals have type double. To give them accum
   // type, use the k suffix.
   f  = 8.125f;
   lf = 8.125;
   k  = 8.125k;
   x  = 8.125k;
   lk = 8.125lk;

   // Conversions between types is automatic, following normal C promotion
   // rules.
   lf = f * lf * k * x * lk;
   lk = lf;

   // Casts can also be explicit.
   k = (accum)lf;

   // Or using casts to improve intermediary precision. Note that type
   // promotion causes both operations to be done as type double.
   k = (double)k * x / 10;
}

// EOF

