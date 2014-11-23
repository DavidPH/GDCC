;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Unsigned division routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;;
;; ___GDCC__DiXU_W
;;
Function "___GDCC__DiXU_W" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__DiXU_W$label" \
   linka    = ExtC \
   localReg = 3 \
   param    = 2 \
   retrn    = 2 \
   block
{
   AndU_W,   Stk(), LocReg(Lit(0), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__DiXU_W$l0")

   ; l has high bit set.
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__DiXU_W$l1r0")

   ; l and r have high bit set
   ; Therefore, if l < r, result is {0, l}, otherwise result is {1, l - r}.
   CmpI_LT_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__DiXU_W$l1r1_lt")

   Move_W, Stk(), Lit(1)
   SubU_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   Retn,   Stk(), Stk()

"___GDCC__DiXU_W$l1r1_lt"
   Move_W, Stk(), Lit(0)
   Move_W, Stk(), LocReg(Lit(0), 0)
   Retn,   Stk(), Stk()

   ; l has high bit set, r does not.
"___GDCC__DiXU_W$l1r0"
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(1)
   Cjmp_Nil, Stk(), Lit("___GDCC__DiXU_W$l1r00")

   ; r has low bit set.

   ; Halve l and divide by r, then double result.
   ; If the result of the division is 0, then immediately return {1, l - r}.
   ShRU_W, Stk(), LocReg(Lit(0), 0), Lit(1)
   DivI_W, Stk(), Stk(), LocReg(Lit(1), 0)
   Casm,   Lit(84), Lit(0), Lit("___GDCC__DiXU_W$l1r01_1") ; Cjmp_Lit
   ShLU_W, Stk(), Stk(), Lit(1)
   Move_W, LocReg(Lit(2), 0), Stk()

   ; If l - res * r >= r, add one to result.
   Move_W,    Stk(), LocReg(Lit(0), 0)
   MulU_W,    Stk(), LocReg(Lit(1), 0), LocReg(Lit(2), 0)
   SubU_W,    Stk(), Stk(), Stk()
   CmpI_GE_W, Stk(), Stk(), LocReg(Lit(1), 0)
   Cjmp_Nil,  Stk(), Lit("___GDCC__DiXU_W$l1r01_r")
   AddU_W,    LocReg(Lit(2), 0), LocReg(Lit(2), 0), Lit(1)

"___GDCC__DiXU_W$l1r01_r"
   Move_W, Stk(), LocReg(Lit(2), 0)

   Move_W, Stk(), LocReg(Lit(0), 0)
   MulU_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(1), 0)
   SubU_W, Stk(), Stk(), Stk()

   Retn,   Stk(), Stk()

"___GDCC__DiXU_W$l1r01_1"
   Move_W, Stk(), Lit(1)
   SubU_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   Retn,   Stk(), Stk()

   ; r has low bit unset.
   ; Therefore, div result is (l >> 1) / (r >> 1).
   ; Therefore, mod result is l - quot * r.
"___GDCC__DiXU_W$l1r00"
   ShRU_W, Stk(), LocReg(Lit(0), 0), Lit(1)
   ShRI_W, Stk(), LocReg(Lit(1), 0), Lit(1)
   DivI_W, LocReg(Lit(2), 0), Stk(), Stk()
   Move_W, Stk(), LocReg(Lit(2), 0)

   Move_W, Stk(), LocReg(Lit(0), 0)
   MulU_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(1), 0)
   SubU_W, Stk(), Stk(), Stk()

   Retn,   Stk(), Stk()

   ; l does not have high bit set.
"___GDCC__DiXU_W$l0"
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__DiXU_W$l0r0")

   ; l does not have high bit set, r does.
   ; Therefore, l < r, result is {0, l}.
   Move_W, Stk(), Lit(0)
   Move_W, Stk(), LocReg(Lit(0), 0)
   Retn,   Stk(), Stk()

   ; l and r do not have high bit set.
   ; Therefore, signed division will work.
"___GDCC__DiXU_W$l0r0"
   DivI_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   ModI_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   Retn,   Stk(), Stk()
}

;;
;; ___GDCC__DiXU_W2
;;
;; Registers:
;;    0 l/dividend
;;    2 r/divisor
;;    4 quotient
;;    6 remainder
;;    8 mask
;;
Function "___GDCC__DiXU_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__DiXU_W2$label" \
   linka    = ExtC \
   localReg = 10 \
   param    = 4 \
   retrn    = 4 \
   block
{
   ; If high word unset for both operands, use 1-word division.
   OrIU_W,   Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Cjmp_Tru, Stk(), Lit("___GDCC__DiXU_W2$full")

   DiXU_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(2), 0)
   Move_W, LocReg(Lit(2), 0), Stk()
   Move_W, LocReg(Lit(0), 0), Stk()

   Move_W2, Stk(), LocReg(Lit(0), 0)
   Move_W2, Stk(), LocReg(Lit(2), 0)

   Retn, Stk(), Stk(), Stk(), Stk()

"___GDCC__DiXU_W2$full"
   Move_W2, LocReg(Lit(4), 0), Lit(0_64.0)
   Move_W2, LocReg(Lit(6), 0), LocReg(Lit(0), 0)

   ; If divisor > dividend, return now.
   CmpU_GT_W2, Stk(), LocReg(Lit(2), 0), LocReg(Lit(0), 0)
   Cjmp_Tru,   Stk(), Lit("___GDCC__DiXU_W2$ret")


   ;;
   ;; Adjust divisor and calculate mask.
   ;;

   ; To minimize the number of shifts, test half the remaining size at a time.

   Move_W,   Stk(), LocReg(Lit(3), 0)
   Cjmp_Tru, Stk(), Lit("___GDCC__DiXU_W2$adjust32_1")

   Move_W,   LocReg(Lit(3), 0), LocReg(Lit(2), 0)
   Move_W,   LocReg(Lit(2), 0), Lit(0)

   Move_W2,  LocReg(Lit(8), 0), Lit(0x100000000_64.0)

   Jump,     Lit("___GDCC__DiXU_W2$adjust16")

"___GDCC__DiXU_W2$adjust32_1"
   Move_W2,  LocReg(Lit(8), 0), Lit(0x1_64.0)

"___GDCC__DiXU_W2$adjust16"
   AndU_W,   Stk(), LocReg(Lit(3), 0), Lit(0xFFFF0000)
   Cjmp_Tru, Stk(), Lit("___GDCC__DiXU_W2$adjust8")

   ShLU_W2,  LocReg(Lit(2), 0), LocReg(Lit(2), 0), Lit(16)
   ShLU_W2,  LocReg(Lit(8), 0), LocReg(Lit(8), 0), Lit(16)

"___GDCC__DiXU_W2$adjust8"
   AndU_W,   Stk(), LocReg(Lit(3), 0), Lit(0xFF000000)
   Cjmp_Tru, Stk(), Lit("___GDCC__DiXU_W2$adjust4")

   ShLU_W2,  LocReg(Lit(2), 0), LocReg(Lit(2), 0), Lit(8)
   ShLU_W2,  LocReg(Lit(8), 0), LocReg(Lit(8), 0), Lit(8)

"___GDCC__DiXU_W2$adjust4"
   AndU_W,   Stk(), LocReg(Lit(3), 0), Lit(0xF0000000)
   Cjmp_Tru, Stk(), Lit("___GDCC__DiXU_W2$adjust2")

   ShLU_W2,  LocReg(Lit(2), 0), LocReg(Lit(2), 0), Lit(4)
   ShLU_W2,  LocReg(Lit(8), 0), LocReg(Lit(8), 0), Lit(4)

"___GDCC__DiXU_W2$adjust2"
   AndU_W,   Stk(), LocReg(Lit(3), 0), Lit(0xC0000000)
   Cjmp_Tru, Stk(), Lit("___GDCC__DiXU_W2$adjust1")

   ShLU_W2,  LocReg(Lit(2), 0), LocReg(Lit(2), 0), Lit(2)
   ShLU_W2,  LocReg(Lit(8), 0), LocReg(Lit(8), 0), Lit(2)

"___GDCC__DiXU_W2$adjust1"
   AndU_W,   Stk(), LocReg(Lit(3), 0), Lit(0x80000000)
   Cjmp_Tru, Stk(), Lit("___GDCC__DiXU_W2$adjust0")

   ShLU_W2,  LocReg(Lit(2), 0), LocReg(Lit(2), 0), Lit(1)
   ShLU_W2,  LocReg(Lit(8), 0), LocReg(Lit(8), 0), Lit(1)

"___GDCC__DiXU_W2$adjust0"


   ;;
   ;; Division loop, 2-word.
   ;;

   ; If high word of mask unset, skip to next loop.
   Move_W,     Stk(), LocReg(Lit(9), 0)
   Cjmp_Nil,   Stk(), Lit("___GDCC__DiXU_W2$loop1cond")

   Jump,       Lit("___GDCC__DiXU_W2$loop2cond")

"___GDCC__DiXU_W2$loop2body"
   ; Check for division at current bit.
   CmpU_GE_W2, Stk(), LocReg(Lit(6), 0), LocReg(Lit(2), 0)
   Cjmp_Nil,   Stk(), Lit("___GDCC__DiXU_W2$loop2divchk")

   ; quotient |= mask; remainder -= divisor;
   OrIU_W,     LocReg(Lit(5), 0), LocReg(Lit(5), 0), LocReg(Lit(9), 0)
   SubU_W2,    LocReg(Lit(6), 0), LocReg(Lit(6), 0), LocReg(Lit(2), 0)
"___GDCC__DiXU_W2$loop2divchk"

   ; divisor >>= 1; mask >>= 1;
   ShRU_W2,    LocReg(Lit(2), 0), LocReg(Lit(2), 0), Lit(1)
   ShRU_W,     LocReg(Lit(9), 0), LocReg(Lit(9), 0), Lit(1)

   ; while(mask && remainder)
"___GDCC__DiXU_W2$loop2cond"
   Move_W,     Stk(), LocReg(Lit(9), 0)
   OrIU_W,     Stk(), LocReg(Lit(6), 0), LocReg(Lit(7), 0)
   Casm,       Lit(70) ; LAnd
   Cjmp_Tru,   Stk(), Lit("___GDCC__DiXU_W2$loop2body")

   Move_W,     LocReg(Lit(8), 0), Lit(0x80000000)
   Jump,       Lit("___GDCC__DiXU_W2$loop1cond")


   ;;
   ;; Division loop, 1-word.
   ;;

"___GDCC__DiXU_W2$loop1body"
   ; Check for division at current bit.
   CmpU_GE_W2, Stk(), LocReg(Lit(6), 0), LocReg(Lit(2), 0)
   Cjmp_Nil,   Stk(), Lit("___GDCC__DiXU_W2$loop1divchk")

   ; quotient |= mask; remainder -= divisor;
   OrIU_W,     LocReg(Lit(4), 0), LocReg(Lit(4), 0), LocReg(Lit(8), 0)
   SubU_W2,    LocReg(Lit(6), 0), LocReg(Lit(6), 0), LocReg(Lit(2), 0)
"___GDCC__DiXU_W2$loop1divchk"

   ; divisor >>= 1; mask >>= 1;
   ShRU_W2,    LocReg(Lit(2), 0), LocReg(Lit(2), 0), Lit(1)
   ShRU_W,     LocReg(Lit(8), 0), LocReg(Lit(8), 0), Lit(1)

   ; while(mask && remainder)
"___GDCC__DiXU_W2$loop1cond"
   Move_W,     Stk(), LocReg(Lit(8), 0)
   OrIU_W,     Stk(), LocReg(Lit(6), 0), LocReg(Lit(7), 0)
   Casm,       Lit(70) ; LAnd
   Cjmp_Tru,   Stk(), Lit("___GDCC__DiXU_W2$loop1body")


   ;;
   ;; Return quotient and remainder.
   ;;

"___GDCC__DiXU_W2$ret"
   Move_W2, Stk(), LocReg(Lit(4), 0)
   Move_W2, Stk(), LocReg(Lit(6), 0)
   Retn,    Stk(), Stk(), Stk(), Stk()
}

;;
;; ___GDCC__DiXU_W3
;;
;; Registers:
;;     0 l/dividend
;;     3 r/divisor
;;     6 quotient
;;     9 remainder
;;    12 mask
;;
Function "___GDCC__DiXU_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__DiXU_W3$label" \
   linka    = ExtC \
   localReg = 15 \
   param    = 6 \
   retrn    = 6 \
   block
{
   ; If high word unset for both operands, use 2-word division.
   OrIU_W,   Stk(), LocReg(Lit(2), 0), LocReg(Lit(5), 0)
   Cjmp_Tru, Stk(), Lit("___GDCC__DiXU_W3$full")

   ; If mid word unset for both operands, use 1-word division.
   OrIU_W,   Stk(), LocReg(Lit(1), 0), LocReg(Lit(4), 0)
   Cjmp_Tru, Stk(), Lit("___GDCC__DiXU_W3$part")

   DiXU_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(3), 0)
   Move_W, LocReg(Lit(3), 0), Stk()
   Move_W, LocReg(Lit(0), 0), Stk()

   Move_W3, Stk(), LocReg(Lit(0), 0)
   Move_W3, Stk(), LocReg(Lit(3), 0)

   Retn, Stk(), Stk(), Stk(), Stk(), Stk(), Stk()

"___GDCC__DiXU_W3$part"
   DiXU_W2, Stk(), LocReg(Lit(0), 0), LocReg(Lit(3), 0)
   Move_W2, LocReg(Lit(3), 0), Stk()
   Move_W2, LocReg(Lit(0), 0), Stk()

   Move_W3, Stk(), LocReg(Lit(0), 0)
   Move_W3, Stk(), LocReg(Lit(3), 0)

   Retn, Stk(), Stk(), Stk(), Stk(), Stk(), Stk()

"___GDCC__DiXU_W3$full"
   Move_W3, LocReg(Lit(6), 0), Lit(0_96.0)
   Move_W3, LocReg(Lit(9), 0), LocReg(Lit(0), 0)

   ; If divisor > dividend, return now.
   CmpU_GT_W3, Stk(), LocReg(Lit(3), 0), LocReg(Lit(0), 0)
   Cjmp_Tru,   Stk(), Lit("___GDCC__DiXU_W3$ret")


   ;;
   ;; Adjust divisor and calculate mask.
   ;;

   ; To minimize the number of shifts, test half the remaining size at a time.

   Move_W,   Stk(), LocReg(Lit(5), 0)
   Cjmp_Tru, Stk(), Lit("___GDCC__DiXU_W3$adjust64_1")

   Move_W,   Stk(), LocReg(Lit(4), 0)
   Cjmp_Tru, Stk(), Lit("___GDCC__DiXU_W3$adjust64_01")

   Move_W,   LocReg(Lit(5), 0), LocReg(Lit(3), 0)
   Move_W,   LocReg(Lit(4), 0), Lit(0)
   Move_W,   LocReg(Lit(3), 0), Lit(0)

   Move_W3,  LocReg(Lit(12), 0), Lit(0x10000000000000000_96.0)

   Jump,     Lit("___GDCC__DiXU_W3$adjust16")

"___GDCC__DiXU_W3$adjust64_01"
   Move_W,   LocReg(Lit(5), 0), LocReg(Lit(4), 0)
   Move_W,   LocReg(Lit(4), 0), LocReg(Lit(3), 0)
   Move_W,   LocReg(Lit(3), 0), Lit(0)

   Move_W3,  LocReg(Lit(12), 0), Lit(0x100000000_96.0)

   Jump,     Lit("___GDCC__DiXU_W3$adjust16")

"___GDCC__DiXU_W3$adjust64_1"
   Move_W3,  LocReg(Lit(12), 0), Lit(0x1_96.0)

"___GDCC__DiXU_W3$adjust16"
   AndU_W,   Stk(), LocReg(Lit(5), 0), Lit(0xFFFF0000)
   Cjmp_Tru, Stk(), Lit("___GDCC__DiXU_W3$adjust8")

   ShLU_W3,  LocReg(Lit( 3), 0), LocReg(Lit( 3), 0), Lit(16)
   ShLU_W3,  LocReg(Lit(12), 0), LocReg(Lit(12), 0), Lit(16)

"___GDCC__DiXU_W3$adjust8"
   AndU_W,   Stk(), LocReg(Lit(5), 0), Lit(0xFF000000)
   Cjmp_Tru, Stk(), Lit("___GDCC__DiXU_W3$adjust4")

   ShLU_W3,  LocReg(Lit( 3), 0), LocReg(Lit( 3), 0), Lit(8)
   ShLU_W3,  LocReg(Lit(12), 0), LocReg(Lit(12), 0), Lit(8)

"___GDCC__DiXU_W3$adjust4"
   AndU_W,   Stk(), LocReg(Lit(5), 0), Lit(0xF0000000)
   Cjmp_Tru, Stk(), Lit("___GDCC__DiXU_W3$adjust2")

   ShLU_W3,  LocReg(Lit( 3), 0), LocReg(Lit( 3), 0), Lit(4)
   ShLU_W3,  LocReg(Lit(12), 0), LocReg(Lit(12), 0), Lit(4)

"___GDCC__DiXU_W3$adjust2"
   AndU_W,   Stk(), LocReg(Lit(5), 0), Lit(0xC0000000)
   Cjmp_Tru, Stk(), Lit("___GDCC__DiXU_W3$adjust1")

   ShLU_W3,  LocReg(Lit( 3), 0), LocReg(Lit( 3), 0), Lit(2)
   ShLU_W3,  LocReg(Lit(12), 0), LocReg(Lit(12), 0), Lit(2)

"___GDCC__DiXU_W3$adjust1"
   AndU_W,   Stk(), LocReg(Lit(5), 0), Lit(0x80000000)
   Cjmp_Tru, Stk(), Lit("___GDCC__DiXU_W3$adjust0")

   ShLU_W3,  LocReg(Lit( 3), 0), LocReg(Lit( 3), 0), Lit(1)
   ShLU_W3,  LocReg(Lit(12), 0), LocReg(Lit(12), 0), Lit(1)

"___GDCC__DiXU_W3$adjust0"


   ;;
   ;; Division loop, 3-word.
   ;;

   ; If high word of mask unset, skip to next loop.
   Move_W,     Stk(), LocReg(Lit(14), 0)
   Cjmp_Nil,   Stk(), Lit("___GDCC__DiXU_W3$loop2")

   Jump,       Lit("___GDCC__DiXU_W3$loop3cond")

"___GDCC__DiXU_W3$loop3body"
   ; Check for division at current bit.
   CmpU_GE_W3, Stk(), LocReg(Lit(9), 0), LocReg(Lit(3), 0)
   Cjmp_Nil,   Stk(), Lit("___GDCC__DiXU_W3$loop3divchk")

   ; quotient |= mask; remainder -= divisor;
   OrIU_W,     LocReg(Lit(8), 0), LocReg(Lit(8), 0), LocReg(Lit(14), 0)
   SubU_W3,    LocReg(Lit(9), 0), LocReg(Lit(9), 0), LocReg(Lit( 3), 0)
"___GDCC__DiXU_W3$loop3divchk"

   ; divisor >>= 1; mask >>= 1;
   ShRU_W3,    LocReg(Lit( 3), 0), LocReg(Lit( 3), 0), Lit(1)
   ShRU_W,     LocReg(Lit(14), 0), LocReg(Lit(14), 0), Lit(1)

   ; while(mask && remainder)
"___GDCC__DiXU_W3$loop3cond"
   Move_W,     Stk(), LocReg(Lit(14), 0)
   OrIU_W,     Stk(), LocReg(Lit(9), 0), LocReg(Lit(10), 0)
   OrIU_W,     Stk(), Stk(), LocReg(Lit(11), 0)
   Casm,       Lit(70) ; LAnd
   Cjmp_Tru,   Stk(), Lit("___GDCC__DiXU_W3$loop3body")

   Move_W,     LocReg(Lit(13), 0), Lit(0x80000000)
   Jump,       Lit("___GDCC__DiXU_W3$loop2cond")


   ;;
   ;; Division loop, 2-word.
   ;;

   ; If high word of mask unset, skip to next loop.
"___GDCC__DiXU_W3$loop2"
   Move_W,     Stk(), LocReg(Lit(13), 0)
   Cjmp_Nil,   Stk(), Lit("___GDCC__DiXU_W3$loop1cond")

   Jump,       Lit("___GDCC__DiXU_W3$loop2cond")

"___GDCC__DiXU_W3$loop2body"
   ; Check for division at current bit.
   CmpU_GE_W3, Stk(), LocReg(Lit(9), 0), LocReg(Lit(3), 0)
   Cjmp_Nil,   Stk(), Lit("___GDCC__DiXU_W3$loop2divchk")

   ; quotient |= mask; remainder -= divisor;
   OrIU_W,     LocReg(Lit(7), 0), LocReg(Lit(7), 0), LocReg(Lit(13), 0)
   SubU_W3,    LocReg(Lit(9), 0), LocReg(Lit(9), 0), LocReg(Lit( 3), 0)
"___GDCC__DiXU_W3$loop2divchk"

   ; divisor >>= 1; mask >>= 1;
   ShRU_W3,    LocReg(Lit( 3), 0), LocReg(Lit( 3), 0), Lit(1)
   ShRU_W,     LocReg(Lit(13), 0), LocReg(Lit(13), 0), Lit(1)

   ; while(mask && remainder)
"___GDCC__DiXU_W3$loop2cond"
   Move_W,     Stk(), LocReg(Lit(13), 0)
   OrIU_W,     Stk(), LocReg(Lit(9), 0), LocReg(Lit(10), 0)
   OrIU_W,     Stk(), Stk(), LocReg(Lit(11), 0)
   Casm,       Lit(70) ; LAnd
   Cjmp_Tru,   Stk(), Lit("___GDCC__DiXU_W3$loop2body")

   Move_W,     LocReg(Lit(12), 0), Lit(0x80000000)
   Jump,       Lit("___GDCC__DiXU_W3$loop1cond")


   ;;
   ;; Division loop, 1-word.
   ;;

"___GDCC__DiXU_W3$loop1body"
   ; Check for division at current bit.
   CmpU_GE_W3, Stk(), LocReg(Lit(9), 0), LocReg(Lit(3), 0)
   Cjmp_Nil,   Stk(), Lit("___GDCC__DiXU_W3$loop1divchk")

   ; quotient |= mask; remainder -= divisor;
   OrIU_W,     LocReg(Lit(6), 0), LocReg(Lit(6), 0), LocReg(Lit(12), 0)
   SubU_W3,    LocReg(Lit(9), 0), LocReg(Lit(9), 0), LocReg(Lit( 3), 0)
"___GDCC__DiXU_W3$loop1divchk"

   ; divisor >>= 1; mask >>= 1;
   ShRU_W3,    LocReg(Lit( 3), 0), LocReg(Lit( 3), 0), Lit(1)
   ShRU_W,     LocReg(Lit(12), 0), LocReg(Lit(12), 0), Lit(1)

   ; while(mask && remainder)
"___GDCC__DiXU_W3$loop1cond"
   Move_W,     Stk(), LocReg(Lit(12), 0)
   OrIU_W,     Stk(), LocReg(Lit(9), 0), LocReg(Lit(10), 0)
   OrIU_W,     Stk(), Stk(), LocReg(Lit(11), 0)
   Casm,       Lit(70) ; LAnd
   Cjmp_Tru,   Stk(), Lit("___GDCC__DiXU_W3$loop1body")


   ;;
   ;; Return quotient and remainder.
   ;;

"___GDCC__DiXU_W3$ret"
   Move_W3, Stk(), LocReg(Lit(6), 0)
   Move_W3, Stk(), LocReg(Lit(9), 0)
   Retn,    Stk(), Stk(), Stk(), Stk(), Stk(), Stk()
}

;;
;; ___GDCC__DivK_W
;;
Function "___GDCC__DivK_W" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__DivK_W$label" \
   linka    = ExtC \
   localReg = 2 \
   param    = 2 \
   retrn    = 1 \
   block
{
   Move_W,  Stk(), Lit(0)
   Move_W,  Stk(), LocReg(Lit(0), 0)
   Move_W,  Stk(), LocReg(Lit(1), 0)
   Move_W,  Stk(), Lit(0)
   DiXU_W2, Stk(), Stk(), Stk()
   Move_W3, Nul(), Stk()
   Retn,    Stk()
}

;;
;; ___GDCC__DivK_W2
;;
Function "___GDCC__DivK_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__DivK_W2$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 2 \
   block
{
   Move_W,  Stk(), Lit(0)
   Move_W2, Stk(), LocReg(Lit(0), 0)
   Move_W2, Stk(), LocReg(Lit(2), 0)
   Move_W,  Stk(), Lit(0)
   DiXU_W3, Stk(), Stk(), Stk()
   Move_W4, Nul(), Stk()
   Retn,    Stk(), Stk()
}

;;
;; ___GDCC__DivU_W
;;
Function "___GDCC__DivU_W" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__DivU_W$label" \
   linka    = ExtC \
   localReg = 3 \
   param    = 2 \
   retrn    = 1 \
   block
{
   AndU_W,   Stk(), LocReg(Lit(0), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__DivU_W$l0")

   ; l has high bit set.
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__DivU_W$l1r0")

   ; l and r have high bit set
   ; Therefore, if l < r, result is 0, otherwise result is 1.
   ; Therefore, result is l >= r.
   CmpI_GE_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   Retn,      Stk()

   ; l has high bit set, r does not.
"___GDCC__DivU_W$l1r0"
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(1)
   Cjmp_Nil, Stk(), Lit("___GDCC__DivU_W$l1r00")

   ; r has low bit set.

   ; Halve l and divide by r, then double result.
   ; If the result of the division is 0, then immediately return 1.
   ShRU_W, Stk(), LocReg(Lit(0), 0), Lit(1)
   DivI_W, Stk(), Stk(), LocReg(Lit(1), 0)
   Casm,   Lit(84), Lit(0), Lit("___GDCC__DivU_W$l1r01_1") ; Cjmp_Lit
   ShLU_W, Stk(), Stk(), Lit(1)
   Move_W, LocReg(Lit(2), 0), Stk()

   ; If l - res * r >= r, add one to result.
   Move_W,    Stk(), LocReg(Lit(0), 0)
   MulU_W,    Stk(), LocReg(Lit(1), 0), LocReg(Lit(2), 0)
   SubU_W,    Stk(), Stk(), Stk()
   CmpI_GE_W, Stk(), Stk(), LocReg(Lit(1), 0)
   Cjmp_Nil,  Stk(), Lit("___GDCC__DivU_W$l1r01_r")
   AddU_W,    LocReg(Lit(2), 0), LocReg(Lit(2), 0), Lit(1)

"___GDCC__DivU_W$l1r01_r"
   Move_W, Stk(), LocReg(Lit(2), 0)
   Retn,   Stk()

"___GDCC__DivU_W$l1r01_1"
   Move_W, Stk(), Lit(1)
   Retn,   Stk()

   ; r has low bit unset.
   ; Therefore, result is (l >> 1) / (r >> 1).
"___GDCC__DivU_W$l1r00"
   ShRU_W, Stk(), LocReg(Lit(0), 0), Lit(1)
   ShRI_W, Stk(), LocReg(Lit(1), 0), Lit(1)
   DivI_W, Stk(), Stk(), Stk()
   Retn,   Stk()

   ; l does not have high bit set.
"___GDCC__DivU_W$l0"
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__DivU_W$l0r0")

   ; l does not have high bit set, r does.
   ; Therefore, l < r, result is 0.
   Move_W, Stk(), Lit(0)
   Retn,   Stk()

   ; l and r do not have high bit set.
   ; Therefore, signed division will work.
"___GDCC__DivU_W$l0r0"
   DivI_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   Retn,   Stk()
}

;;
;; ___GDCC__DivU_W2
;;
Function "___GDCC__DivU_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__DivU_W2$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 2 \
   block
{
   Move_W2, Stk(), LocReg(Lit(0), 0)
   Move_W2, Stk(), LocReg(Lit(2), 0)
   Call,    Lit("___GDCC__DiXU_W2"), Lit(4), Stk(), Stk(), Stk(), Stk()
   Move_W2, Nul(), Stk()
   Retn,    Stk(), Stk()
}

;;
;; ___GDCC__DivU_W3
;;
Function "___GDCC__DivU_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__DivU_W3$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 6 \
   retrn    = 3 \
   block
{
   Move_W3, Stk(), LocReg(Lit(0), 0)
   Move_W3, Stk(), LocReg(Lit(3), 0)
   Call,    Lit("___GDCC__DiXU_W3"), Lit(6), Stk(), Stk(), Stk(), Stk(), Stk(), Stk()
   Move_W3, Nul(), Stk()
   Retn,    Stk(), Stk(), Stk()
}

;;
;; ___GDCC__ModU_W
;;
Function "___GDCC__ModU_W" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__ModU_W$label" \
   linka    = ExtC \
   localReg = 3 \
   param    = 2 \
   retrn    = 1 \
   block
{
   AndU_W,   Stk(), LocReg(Lit(0), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__ModU_W$l0")

   ; l has high bit set.
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__ModU_W$l1r0")

   ; l and r have high bit set
   ; Therefore, if l < r, result is 0, otherwise result is 1.
   ; Therefore, remainder is l >= r ? l - r : l.
   Move_W,    Stk(), LocReg(Lit(0), 0)
   CmpI_GE_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   Cjmp_Nil,  Stk(), Lit("___GDCC__ModU_W$l1r1_ret")
   SubU_W,    Stk(), Stk(), LocReg(Lit(1), 0)

"___GDCC__ModU_W$l1r1_ret"
   Retn,      Stk()

   ; l has high bit set, r does not.
"___GDCC__ModU_W$l1r0"
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(1)
   Cjmp_Nil, Stk(), Lit("___GDCC__ModU_W$l1r00")

   ; r has low bit set.

   ; Halve l and divide by r, then double result.
   ; If the result of the division is 0, then immediately return l - r.
   ShRU_W, Stk(), LocReg(Lit(0), 0), Lit(1)
   DivI_W, Stk(), Stk(), LocReg(Lit(1), 0)
   Casm,   Lit(84), Lit("___GDCC__ModU_W$l1r01_1") ; Cjmp_Lit
   ShLU_W, Stk(), Stk(), Lit(1)
   Move_W, LocReg(Lit(2), 0), Stk()

   ; If l - res * r >= r, add one to result.
   Move_W,    Stk(), LocReg(Lit(0), 0)
   MulU_W,    Stk(), LocReg(Lit(1), 0), LocReg(Lit(2), 0)
   SubU_W,    Stk(), Stk(), Stk()
   CmpI_GE_W, Stk(), Stk(), LocReg(Lit(1), 0)
   Cjmp_Nil,  Stk(), Lit("___GDCC__ModU_W$l1r01_r")
   AddU_W,    LocReg(Lit(2), 0), LocReg(Lit(2), 0), Lit(1)

"___GDCC__ModU_W$l1r01_r"
   Move_W, Stk(), LocReg(Lit(0), 0)
   MulU_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(2), 0)
   SubU_W, Stk(), Stk(), Stk()
   Retn,   Stk()

"___GDCC__ModU_W$l1r01_1"
   SubU_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   Retn,   Stk()

   ; r has low bit unset.
   ; Therefore, result is l - (l >> 1) / (r >> 1) * r.
"___GDCC__ModU_W$l1r00"
   Move_W, Stk(), LocReg(Lit(0), 0)
   ShRU_W, Stk(), LocReg(Lit(0), 0), Lit(1)
   ShRI_W, Stk(), LocReg(Lit(1), 0), Lit(1)
   DivI_W, Stk(), Stk(), Stk()
   MulU_W, Stk(), Stk(), LocReg(Lit(1), 0)
   SubU_W, Stk(), Stk(), Stk()
   Retn,   Stk()

   ; l does not have high bit set.
"___GDCC__ModU_W$l0"
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__ModU_W$l0r0")

   ; l does not have high bit set, r does.
   ; Therefore, l < r, remainder is l.
   Move_W, Stk(), LocReg(Lit(0), 0)
   Retn,   Stk()

   ; l and r do not have high bit set.
   ; Therefore, signed division will work.
"___GDCC__ModU_W$l0r0"
   ModI_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   Retn,   Stk()
}

;;
;; ___GDCC__ModU_W2
;;
Function "___GDCC__ModU_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__ModU_W2$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 2 \
   block
{
   Move_W2, Stk(), LocReg(Lit(0), 0)
   Move_W2, Stk(), LocReg(Lit(2), 0)
   Call,    Lit("___GDCC__DiXU_W2"), Lit(4), Stk(), Stk(), Stk(), Stk()
   Swap_W2, Stk(), Stk()
   Move_W2, Nul(), Stk()
   Retn,    Stk(), Stk()
}

;;
;; ___GDCC__ModU_W3
;;
Function "___GDCC__ModU_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__ModU_W3$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 6 \
   retrn    = 3 \
   block
{
   Move_W3, Stk(), LocReg(Lit(0), 0)
   Move_W3, Stk(), LocReg(Lit(3), 0)
   Call,    Lit("___GDCC__DiXU_W3"), Lit(6), Stk(), Stk(), Stk(), Stk(), Stk(), Stk()
   Swap_W3, Stk(), Stk()
   Move_W3, Nul(), Stk()
   Retn,    Stk(), Stk(), Stk()
}

;; EOF

