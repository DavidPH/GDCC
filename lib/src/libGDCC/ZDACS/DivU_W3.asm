;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Unsigned 3-word division routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

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
   ; TODO

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
   CmpU_GT_W3, Stk(), LocReg(Lit(9), 0), LocReg(Lit(3), 0)
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
   CmpU_GT_W3, Stk(), LocReg(Lit(9), 0), LocReg(Lit(3), 0)
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
   CmpU_GT_W3, Stk(), LocReg(Lit(9), 0), LocReg(Lit(3), 0)
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

