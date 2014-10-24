;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Unsigned 2-word division routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

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
   ; TODO

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
   CmpU_GT_W2, Stk(), LocReg(Lit(6), 0), LocReg(Lit(2), 0)
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
   CmpU_GT_W2, Stk(), LocReg(Lit(6), 0), LocReg(Lit(2), 0)
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

;; EOF

