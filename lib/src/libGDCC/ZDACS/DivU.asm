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
;; ___GDCC__DiXU_W1
;;
Function "___GDCC__DiXU_W1" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 3 \
   param    = 2 \
   retrn    = 2 \
   block
{
   AndU_W   1, Stk(), LocReg(Lit(0)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit(:"$l0")

   ; l has high bit set.
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit(:"$l1r0")

   ; l and r have high bit set
   ; Therefore, if l < r, result is {0, l}, otherwise result is {1, l - r}.
   CmpI_LT_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Jcnd_Tru  1, Stk(), Lit(:"$l1r1_lt")

   Move_W 1, Stk(), Lit(1)
   SubU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Retn   2, Stk()

:"$l1r1_lt"
   Move_W 1, Stk(), Lit(0)
   Move_W 1, Stk(), LocReg(Lit(0))
   Retn   2, Stk()

   ; l has high bit set, r does not.
:"$l1r0"
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(1)
   Jcnd_Nil 1, Stk(), Lit(:"$l1r00")

   ; r has low bit set.

   ; Halve l and divide by r, then double result.
   ; If the result of the division is 0, then immediately return {1, l - r}.
   ShRU_W 1, Stk(), LocReg(Lit(0)), Lit(1)
   DivI_W 1, Stk(), Stk(), LocReg(Lit(1))
   Casm   0, Lit(84), Lit(0), Lit(:"$l1r01_1") ; Jcnd_Lit
   ShLU_W 1, Stk(), Stk(), Lit(1)
   Move_W 1, LocReg(Lit(2)), Stk()

   ; If l - res * r >= r, add one to result.
   Move_W    1, Stk(), LocReg(Lit(0))
   MulU_W    1, Stk(), LocReg(Lit(1)), LocReg(Lit(2))
   SubU_W    1, Stk(), Stk(), Stk()
   CmpI_GE_W 1, Stk(), Stk(), LocReg(Lit(1))
   Jcnd_Nil  1, Stk(), Lit(:"$l1r01_r")
   AddU_W    1, LocReg(Lit(2)), LocReg(Lit(2)), Lit(1)

:"$l1r01_r"
   Move_W 1, Stk(), LocReg(Lit(2))

   Move_W 1, Stk(), LocReg(Lit(0))
   MulU_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(1))
   SubU_W 1, Stk(), Stk(), Stk()

   Retn   2, Stk()

:"$l1r01_1"
   Move_W 1, Stk(), Lit(1)
   SubU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Retn   2, Stk()

   ; r has low bit unset.
   ; Therefore, div result is (l >> 1) / (r >> 1).
   ; Therefore, mod result is l - quot * r.
:"$l1r00"
   ShRU_W 1, Stk(), LocReg(Lit(0)), Lit(1)
   ShRI_W 1, Stk(), LocReg(Lit(1)), Lit(1)
   DivI_W 1, LocReg(Lit(2)), Stk(), Stk()
   Move_W 1, Stk(), LocReg(Lit(2))

   Move_W 1, Stk(), LocReg(Lit(0))
   MulU_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(1))
   SubU_W 1, Stk(), Stk(), Stk()

   Retn   2, Stk()

   ; l does not have high bit set.
:"$l0"
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit(:"$l0r0")

   ; l does not have high bit set, r does.
   ; Therefore, l < r, result is {0, l}.
   Move_W 1, Stk(), Lit(0)
   Move_W 1, Stk(), LocReg(Lit(0))
   Retn   2, Stk()

   ; l and r do not have high bit set.
   ; Therefore, signed division will work.
:"$l0r0"
   DivI_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   ModI_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Retn   2, Stk()
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
   label    = :"$label" \
   linka    = ExtC \
   localReg = 10 \
   param    = 4 \
   retrn    = 4 \
   block
{
   ; If high word unset for both operands, use 1-word division.
   OrIU_W   1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Jcnd_Tru 1, Stk(), Lit(:"$full")

   DiXU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Move_W 1, LocReg(Lit(2)), Stk()
   Move_W 1, LocReg(Lit(0)), Stk()

   Move_W 2, Stk(), LocReg(Lit(0))
   Move_W 2, Stk(), LocReg(Lit(2))

   Retn   4, Stk()

:"$full"
   Move_W 2, LocReg(Lit(4)), Lit(0_64.0)
   Move_W 2, LocReg(Lit(6)), LocReg(Lit(0))

   ; If divisor > dividend, return now.
   CmpU_GT_W 2, Stk(), LocReg(Lit(2)), LocReg(Lit(0))
   Jcnd_Tru  1, Stk(), Lit(:"$ret")


   ;;
   ;; Adjust divisor and calculate mask.
   ;;

   ; To minimize the number of shifts, test half the remaining size at a time.

   Move_W   1, Stk(), LocReg(Lit(3))
   Jcnd_Tru 1, Stk(), Lit(:"$adjust32_1")

   Move_W   1, LocReg(Lit(3)), LocReg(Lit(2))
   Move_W   1, LocReg(Lit(2)), Lit(0)

   Move_W   2, LocReg(Lit(8)), Lit(0x100000000_64.0)

   Jump     0, Lit(:"$adjust16")

:"$adjust32_1"
   Move_W   2, LocReg(Lit(8)), Lit(0x1_64.0)

:"$adjust16"
   AndU_W   1, Stk(), LocReg(Lit(3)), Lit(0xFFFF0000)
   Jcnd_Tru 1, Stk(), Lit(:"$adjust8")

   ShLU_W   2, LocReg(Lit(2)), LocReg(Lit(2)), Lit(16)
   ShLU_W   2, LocReg(Lit(8)), LocReg(Lit(8)), Lit(16)

:"$adjust8"
   AndU_W   1, Stk(), LocReg(Lit(3)), Lit(0xFF000000)
   Jcnd_Tru 1, Stk(), Lit(:"$adjust4")

   ShLU_W   2, LocReg(Lit(2)), LocReg(Lit(2)), Lit(8)
   ShLU_W   2, LocReg(Lit(8)), LocReg(Lit(8)), Lit(8)

:"$adjust4"
   AndU_W   1, Stk(), LocReg(Lit(3)), Lit(0xF0000000)
   Jcnd_Tru 1, Stk(), Lit(:"$adjust2")

   ShLU_W   2, LocReg(Lit(2)), LocReg(Lit(2)), Lit(4)
   ShLU_W   2, LocReg(Lit(8)), LocReg(Lit(8)), Lit(4)

:"$adjust2"
   AndU_W   1, Stk(), LocReg(Lit(3)), Lit(0xC0000000)
   Jcnd_Tru 1, Stk(), Lit(:"$adjust1")

   ShLU_W   2, LocReg(Lit(2)), LocReg(Lit(2)), Lit(2)
   ShLU_W   2, LocReg(Lit(8)), LocReg(Lit(8)), Lit(2)

:"$adjust1"
   AndU_W   1, Stk(), LocReg(Lit(3)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit(:"$adjust0")

   ShLU_W   2, LocReg(Lit(2)), LocReg(Lit(2)), Lit(1)
   ShLU_W   2, LocReg(Lit(8)), LocReg(Lit(8)), Lit(1)

:"$adjust0"


   ;;
   ;; Division loop, 2-word.
   ;;

   ; If high word of mask unset, skip to next loop.
   Move_W    1, Stk(), LocReg(Lit(9))
   Jcnd_Nil  1, Stk(), Lit(:"$loop1cond")

   Jump      0, Lit(:"$loop2cond")

:"$loop2body"
   ; Check for division at current bit.
   CmpU_GE_W 2, Stk(), LocReg(Lit(6)), LocReg(Lit(2))
   Jcnd_Nil  1, Stk(), Lit(:"$loop2divchk")

   ; quotient |= mask; remainder -= divisor;
   OrIU_W    1, LocReg(Lit(5)), LocReg(Lit(5)), LocReg(Lit(9))
   SubU_W    2, LocReg(Lit(6)), LocReg(Lit(6)), LocReg(Lit(2))
:"$loop2divchk"

   ; divisor >>= 1; mask >>= 1;
   ShRU_W    2, LocReg(Lit(2)), LocReg(Lit(2)), Lit(1)
   ShRU_W    1, LocReg(Lit(9)), LocReg(Lit(9)), Lit(1)

   ; while(mask && remainder)
:"$loop2cond"
   Move_W    1, Stk(), LocReg(Lit(9))
   OrIU_W    1, Stk(), LocReg(Lit(6)), LocReg(Lit(7))
   LAnd      1, Stk(), Stk(), Stk()
   Jcnd_Tru  1, Stk(), Lit(:"$loop2body")

   Move_W    1, LocReg(Lit(8)), Lit(0x80000000)
   Jump      0, Lit(:"$loop1cond")


   ;;
   ;; Division loop, 1-word.
   ;;

:"$loop1body"
   ; Check for division at current bit.
   CmpU_GE_W 2, Stk(), LocReg(Lit(6)), LocReg(Lit(2))
   Jcnd_Nil  1, Stk(), Lit(:"$loop1divchk")

   ; quotient |= mask; remainder -= divisor;
   OrIU_W    1, LocReg(Lit(4)), LocReg(Lit(4)), LocReg(Lit(8))
   SubU_W    2, LocReg(Lit(6)), LocReg(Lit(6)), LocReg(Lit(2))
:"$loop1divchk"

   ; divisor >>= 1; mask >>= 1;
   ShRU_W    2, LocReg(Lit(2)), LocReg(Lit(2)), Lit(1)
   ShRU_W    1, LocReg(Lit(8)), LocReg(Lit(8)), Lit(1)

   ; while(mask && remainder)
:"$loop1cond"
   Move_W    1, Stk(), LocReg(Lit(8))
   OrIU_W    1, Stk(), LocReg(Lit(6)), LocReg(Lit(7))
   LAnd      1, Stk(), Stk(), Stk()
   Jcnd_Tru  1, Stk(), Lit(:"$loop1body")


   ;;
   ;; Return quotient and remainder.
   ;;

:"$ret"
   Move_W 2, Stk(), LocReg(Lit(4))
   Move_W 2, Stk(), LocReg(Lit(6))
   Retn   4, Stk()
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
   label    = :"$label" \
   linka    = ExtC \
   localReg = 15 \
   param    = 6 \
   retrn    = 6 \
   block
{
   ; If high word unset for both operands, use 2-word division.
   OrIU_W   1, Stk(), LocReg(Lit(2)), LocReg(Lit(5))
   Jcnd_Tru 1, Stk(), Lit(:"$full")

   ; If mid word unset for both operands, use 1-word division.
   OrIU_W   1, Stk(), LocReg(Lit(1)), LocReg(Lit(4))
   Jcnd_Tru 1, Stk(), Lit(:"$part")

   DiXU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(3))
   Move_W 1, LocReg(Lit(3)), Stk()
   Move_W 1, LocReg(Lit(0)), Stk()

   Move_W 3, Stk(), LocReg(Lit(0))
   Move_W 3, Stk(), LocReg(Lit(3))

   Retn   6, Stk()

:"$part"
   DiXU_W 2, Stk(), LocReg(Lit(0)), LocReg(Lit(3))
   Move_W 2, LocReg(Lit(3)), Stk()
   Move_W 2, LocReg(Lit(0)), Stk()

   Move_W 3, Stk(), LocReg(Lit(0))
   Move_W 3, Stk(), LocReg(Lit(3))

   Retn   6, Stk()

:"$full"
   Move_W 3, LocReg(Lit(6)), Lit(0_96.0)
   Move_W 3, LocReg(Lit(9)), LocReg(Lit(0))

   ; If divisor > dividend, return now.
   CmpU_GT_W 3, Stk(), LocReg(Lit(3)), LocReg(Lit(0))
   Jcnd_Tru  1, Stk(), Lit(:"$ret")


   ;;
   ;; Adjust divisor and calculate mask.
   ;;

   ; To minimize the number of shifts, test half the remaining size at a time.

   Move_W   1, Stk(), LocReg(Lit(5))
   Jcnd_Tru 1, Stk(), Lit(:"$adjust64_1")

   Move_W   1, Stk(), LocReg(Lit(4))
   Jcnd_Tru 1, Stk(), Lit(:"$adjust64_01")

   Move_W   1, LocReg(Lit(5)), LocReg(Lit(3))
   Move_W   1, LocReg(Lit(4)), Lit(0)
   Move_W   1, LocReg(Lit(3)), Lit(0)

   Move_W   3, LocReg(Lit(12)), Lit(0x10000000000000000_96.0)

   Jump     0, Lit(:"$adjust16")

:"$adjust64_01"
   Move_W   1, LocReg(Lit(5)), LocReg(Lit(4))
   Move_W   1, LocReg(Lit(4)), LocReg(Lit(3))
   Move_W   1, LocReg(Lit(3)), Lit(0)

   Move_W   3, LocReg(Lit(12)), Lit(0x100000000_96.0)

   Jump     0, Lit(:"$adjust16")

:"$adjust64_1"
   Move_W   3, LocReg(Lit(12)), Lit(0x1_96.0)

:"$adjust16"
   AndU_W   1, Stk(), LocReg(Lit(5)), Lit(0xFFFF0000)
   Jcnd_Tru 1, Stk(), Lit(:"$adjust8")

   ShLU_W   3, LocReg(Lit( 3)), LocReg(Lit( 3)), Lit(16)
   ShLU_W   3, LocReg(Lit(12)), LocReg(Lit(12)), Lit(16)

:"$adjust8"
   AndU_W   1, Stk(), LocReg(Lit(5)), Lit(0xFF000000)
   Jcnd_Tru 1, Stk(), Lit(:"$adjust4")

   ShLU_W   3, LocReg(Lit( 3)), LocReg(Lit( 3)), Lit(8)
   ShLU_W   3, LocReg(Lit(12)), LocReg(Lit(12)), Lit(8)

:"$adjust4"
   AndU_W   1, Stk(), LocReg(Lit(5)), Lit(0xF0000000)
   Jcnd_Tru 1, Stk(), Lit(:"$adjust2")

   ShLU_W   3, LocReg(Lit( 3)), LocReg(Lit( 3)), Lit(4)
   ShLU_W   3, LocReg(Lit(12)), LocReg(Lit(12)), Lit(4)

:"$adjust2"
   AndU_W   1, Stk(), LocReg(Lit(5)), Lit(0xC0000000)
   Jcnd_Tru 1, Stk(), Lit(:"$adjust1")

   ShLU_W   3, LocReg(Lit( 3)), LocReg(Lit( 3)), Lit(2)
   ShLU_W   3, LocReg(Lit(12)), LocReg(Lit(12)), Lit(2)

:"$adjust1"
   AndU_W   1, Stk(), LocReg(Lit(5)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit(:"$adjust0")

   ShLU_W   3, LocReg(Lit( 3)), LocReg(Lit( 3)), Lit(1)
   ShLU_W   3, LocReg(Lit(12)), LocReg(Lit(12)), Lit(1)

:"$adjust0"


   ;;
   ;; Division loop, 3-word.
   ;;

   ; If high word of mask unset, skip to next loop.
   Move_W    1, Stk(), LocReg(Lit(14))
   Jcnd_Nil  1, Stk(), Lit(:"$loop2")

   Jump      0, Lit(:"$loop3cond")

:"$loop3body"
   ; Check for division at current bit.
   CmpU_GE_W 3, Stk(), LocReg(Lit(9)), LocReg(Lit(3))
   Jcnd_Nil  1, Stk(), Lit(:"$loop3divchk")

   ; quotient |= mask; remainder -= divisor;
   OrIU_W    1, LocReg(Lit(8)), LocReg(Lit(8)), LocReg(Lit(14))
   SubU_W    3, LocReg(Lit(9)), LocReg(Lit(9)), LocReg(Lit( 3))
:"$loop3divchk"

   ; divisor >>= 1; mask >>= 1;
   ShRU_W    3, LocReg(Lit( 3)), LocReg(Lit( 3)), Lit(1)
   ShRU_W    1, LocReg(Lit(14)), LocReg(Lit(14)), Lit(1)

   ; while(mask && remainder)
:"$loop3cond"
   Move_W    1, Stk(), LocReg(Lit(14))
   OrIU_W    1, Stk(), LocReg(Lit(9)), LocReg(Lit(10))
   OrIU_W    1, Stk(), Stk(), LocReg(Lit(11))
   LAnd      1, Stk(), Stk(), Stk()
   Jcnd_Tru  1, Stk(), Lit(:"$loop3body")

   Move_W    1, LocReg(Lit(13)), Lit(0x80000000)
   Jump      0, Lit(:"$loop2cond")


   ;;
   ;; Division loop, 2-word.
   ;;

   ; If high word of mask unset, skip to next loop.
:"$loop2"
   Move_W    1, Stk(), LocReg(Lit(13))
   Jcnd_Nil  1, Stk(), Lit(:"$loop1cond")

   Jump      0, Lit(:"$loop2cond")

:"$loop2body"
   ; Check for division at current bit.
   CmpU_GE_W 3, Stk(), LocReg(Lit(9)), LocReg(Lit(3))
   Jcnd_Nil  1, Stk(), Lit(:"$loop2divchk")

   ; quotient |= mask; remainder -= divisor;
   OrIU_W    1, LocReg(Lit(7)), LocReg(Lit(7)), LocReg(Lit(13))
   SubU_W    3, LocReg(Lit(9)), LocReg(Lit(9)), LocReg(Lit( 3))
:"$loop2divchk"

   ; divisor >>= 1; mask >>= 1;
   ShRU_W    3, LocReg(Lit( 3)), LocReg(Lit( 3)), Lit(1)
   ShRU_W    1, LocReg(Lit(13)), LocReg(Lit(13)), Lit(1)

   ; while(mask && remainder)
:"$loop2cond"
   Move_W    1, Stk(), LocReg(Lit(13))
   OrIU_W    1, Stk(), LocReg(Lit(9)), LocReg(Lit(10))
   OrIU_W    1, Stk(), Stk(), LocReg(Lit(11))
   LAnd      1, Stk(), Stk(), Stk()
   Jcnd_Tru  1, Stk(), Lit(:"$loop2body")

   Move_W    1, LocReg(Lit(12)), Lit(0x80000000)
   Jump      0, Lit(:"$loop1cond")


   ;;
   ;; Division loop, 1-word.
   ;;

:"$loop1body"
   ; Check for division at current bit.
   CmpU_GE_W 3, Stk(), LocReg(Lit(9)), LocReg(Lit(3))
   Jcnd_Nil  1, Stk(), Lit(:"$loop1divchk")

   ; quotient |= mask; remainder -= divisor;
   OrIU_W    1, LocReg(Lit(6)), LocReg(Lit(6)), LocReg(Lit(12))
   SubU_W    3, LocReg(Lit(9)), LocReg(Lit(9)), LocReg(Lit( 3))
:"$loop1divchk"

   ; divisor >>= 1; mask >>= 1;
   ShRU_W    3, LocReg(Lit( 3)), LocReg(Lit( 3)), Lit(1)
   ShRU_W    1, LocReg(Lit(12)), LocReg(Lit(12)), Lit(1)

   ; while(mask && remainder)
:"$loop1cond"
   Move_W    1, Stk(), LocReg(Lit(12))
   OrIU_W    1, Stk(), LocReg(Lit(9)), LocReg(Lit(10))
   OrIU_W    1, Stk(), Stk(), LocReg(Lit(11))
   LAnd      1, Stk(), Stk(), Stk()
   Jcnd_Tru  1, Stk(), Lit(:"$loop1body")


   ;;
   ;; Return quotient and remainder.
   ;;

:"$ret"
   Move_W 3, Stk(), LocReg(Lit(6))
   Move_W 3, Stk(), LocReg(Lit(9))
   Retn   6, Stk()
}

;;
;; ___GDCC__DivK_W
;;
Function "___GDCC__DivK_W" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 2 \
   param    = 2 \
   retrn    = 1 \
   block
{
   Move_W 1, Stk(), Lit(0)
   Move_W 1, Stk(), LocReg(Lit(0))
   Move_W 1, Stk(), LocReg(Lit(1))
   Move_W 1, Stk(), Lit(0)
   DiXU_W 2, Stk(), Stk(), Stk()
   Move_W 3, Nul(), Stk()
   Retn   1, Stk()
}

;;
;; ___GDCC__DivK_W2
;;
Function "___GDCC__DivK_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 2 \
   block
{
   Move_W 1, Stk(), Lit(0)
   Move_W 2, Stk(), LocReg(Lit(0))
   Move_W 2, Stk(), LocReg(Lit(2))
   Move_W 1, Stk(), Lit(0)
   DiXU_W 3, Stk(), Stk(), Stk()
   Move_W 4, Nul(), Stk()
   Retn   2, Stk()
}

;;
;; ___GDCC__DivU_W1
;;
Function "___GDCC__DivU_W1" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 3 \
   param    = 2 \
   retrn    = 1 \
   block
{
   AndU_W   1, Stk(), LocReg(Lit(0)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit(:"$l0")

   ; l has high bit set.
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit(:"$l1r0")

   ; l and r have high bit set
   ; Therefore, if l < r, result is 0, otherwise result is 1.
   ; Therefore, result is l >= r.
   CmpI_GE_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Retn      1, Stk()

   ; l has high bit set, r does not.
:"$l1r0"
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(1)
   Jcnd_Nil 1, Stk(), Lit(:"$l1r00")

   ; r has low bit set.

   ; Halve l and divide by r, then double result.
   ; If the result of the division is 0, then immediately return 1.
   ShRU_W 1, Stk(), LocReg(Lit(0)), Lit(1)
   DivI_W 1, Stk(), Stk(), LocReg(Lit(1))
   Casm   0, Lit(84), Lit(0), Lit(:"$l1r01_1") ; Jcnd_Lit
   ShLU_W 1, Stk(), Stk(), Lit(1)
   Move_W 1, LocReg(Lit(2)), Stk()

   ; If l - res * r >= r, add one to result.
   Move_W    1, Stk(), LocReg(Lit(0))
   MulU_W    1, Stk(), LocReg(Lit(1)), LocReg(Lit(2))
   SubU_W    1, Stk(), Stk(), Stk()
   CmpI_GE_W 1, Stk(), Stk(), LocReg(Lit(1))
   Jcnd_Nil  1, Stk(), Lit(:"$l1r01_r")
   AddU_W    1, LocReg(Lit(2)), LocReg(Lit(2)), Lit(1)

:"$l1r01_r"
   Move_W 1, Stk(), LocReg(Lit(2))
   Retn   1, Stk()

:"$l1r01_1"
   Move_W 1, Stk(), Lit(1)
   Retn   1, Stk()

   ; r has low bit unset.
   ; Therefore, result is (l >> 1) / (r >> 1).
:"$l1r00"
   ShRU_W 1, Stk(), LocReg(Lit(0)), Lit(1)
   ShRI_W 1, Stk(), LocReg(Lit(1)), Lit(1)
   DivI_W 1, Stk(), Stk(), Stk()
   Retn   1, Stk()

   ; l does not have high bit set.
:"$l0"
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit(:"$l0r0")

   ; l does not have high bit set, r does.
   ; Therefore, l < r, result is 0.
   Move_W 1, Stk(), Lit(0)
   Retn   1, Stk()

   ; l and r do not have high bit set.
   ; Therefore, signed division will work.
:"$l0r0"
   DivI_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Retn   1, Stk()
}

;;
;; ___GDCC__DivU_W2
;;
Function "___GDCC__DivU_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 2 \
   block
{
   DiXU_W 2, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Move_W 2, Nul(), Stk()
   Retn   2, Stk()
}

;;
;; ___GDCC__DivU_W3
;;
Function "___GDCC__DivU_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 6 \
   retrn    = 3 \
   block
{
   DiXU_W 3, Stk(), LocReg(Lit(0)), LocReg(Lit(3))
   Move_W 3, Nul(), Stk()
   Retn   3, Stk()
}

;;
;; ___GDCC__ModU_W1
;;
Function "___GDCC__ModU_W1" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 3 \
   param    = 2 \
   retrn    = 1 \
   block
{
   AndU_W   1, Stk(), LocReg(Lit(0)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit(:"$l0")

   ; l has high bit set.
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit(:"$l1r0")

   ; l and r have high bit set
   ; Therefore, if l < r, result is 0, otherwise result is 1.
   ; Therefore, remainder is l >= r ? l - r : l.
   Move_W    1, Stk(), LocReg(Lit(0))
   CmpI_GE_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Jcnd_Nil  1, Stk(), Lit(:"$l1r1_ret")
   SubU_W    1, Stk(), Stk(), LocReg(Lit(1))

:"$l1r1_ret"
   Retn      1, Stk()

   ; l has high bit set, r does not.
:"$l1r0"
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(1)
   Jcnd_Nil 1, Stk(), Lit(:"$l1r00")

   ; r has low bit set.

   ; Halve l and divide by r, then double result.
   ; If the result of the division is 0, then immediately return l - r.
   ShRU_W 1, Stk(), LocReg(Lit(0)), Lit(1)
   DivI_W 1, Stk(), Stk(), LocReg(Lit(1))
   Casm   0, Lit(84), Lit(:"$l1r01_1") ; Jcnd_Lit
   ShLU_W 1, Stk(), Stk(), Lit(1)
   Move_W 1, LocReg(Lit(2)), Stk()

   ; If l - res * r >= r, add one to result.
   Move_W    1, Stk(), LocReg(Lit(0))
   MulU_W    1, Stk(), LocReg(Lit(1)), LocReg(Lit(2))
   SubU_W    1, Stk(), Stk(), Stk()
   CmpI_GE_W 1, Stk(), Stk(), LocReg(Lit(1))
   Jcnd_Nil  1, Stk(), Lit(:"$l1r01_r")
   AddU_W    1, LocReg(Lit(2)), LocReg(Lit(2)), Lit(1)

:"$l1r01_r"
   Move_W 1, Stk(), LocReg(Lit(0))
   MulU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(2))
   SubU_W 1, Stk(), Stk(), Stk()
   Retn   1, Stk()

:"$l1r01_1"
   SubU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Retn   1, Stk()

   ; r has low bit unset.
   ; Therefore, result is l - (l >> 1) / (r >> 1) * r.
:"$l1r00"
   Move_W 1, Stk(), LocReg(Lit(0))
   ShRU_W 1, Stk(), LocReg(Lit(0)), Lit(1)
   ShRI_W 1, Stk(), LocReg(Lit(1)), Lit(1)
   DivI_W 1, Stk(), Stk(), Stk()
   MulU_W 1, Stk(), Stk(), LocReg(Lit(1))
   SubU_W 1, Stk(), Stk(), Stk()
   Retn   1, Stk()

   ; l does not have high bit set.
:"$l0"
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit(:"$l0r0")

   ; l does not have high bit set, r does.
   ; Therefore, l < r, remainder is l.
   Move_W 1, Stk(), LocReg(Lit(0))
   Retn   1, Stk()

   ; l and r do not have high bit set.
   ; Therefore, signed division will work.
:"$l0r0"
   ModI_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Retn   1, Stk()
}

;;
;; ___GDCC__ModU_W2
;;
Function "___GDCC__ModU_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 2 \
   block
{
   DiXU_W 2, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Swap_W 2, Stk(), Stk()
   Move_W 2, Nul(), Stk()
   Retn   2, Stk()
}

;;
;; ___GDCC__ModU_W3
;;
Function "___GDCC__ModU_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 6 \
   retrn    = 3 \
   block
{
   DiXU_W 3, Stk(), LocReg(Lit(0)), LocReg(Lit(3))
   Swap_W 3, Stk(), Stk()
   Move_W 3, Nul(), Stk()
   Retn   3, Stk()
}

;; EOF

