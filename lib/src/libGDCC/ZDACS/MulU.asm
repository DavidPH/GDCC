;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Multi-word unsigned and fixed-point multiply routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;;
;; ___GDCC__MuXU_W1
;;
Function "___GDCC__MuXU_W1" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 2 \
   retrn    = 2 \
   block
{
   AndU_W 1, LocReg(Lit(2)), LocReg(Lit(0)), Lit(0xFFFF)
   ShRU_W 1, LocReg(Lit(3)), LocReg(Lit(0)), Lit(16)
   AndU_W 1, LocReg(Lit(4)), LocReg(Lit(1)), Lit(0xFFFF)
   ShRU_W 1, LocReg(Lit(5)), LocReg(Lit(1)), Lit(16)

   MulU_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(4))
   Move_W 1, Stk(), Lit(0)

   MulU_W 1, LocReg(Lit(0)), LocReg(Lit(2)), LocReg(Lit(5))
   ShLU_W 1, Stk(), LocReg(Lit(0)), Lit(16)
   ShRU_W 1, Stk(), LocReg(Lit(0)), Lit(16)

   MulU_W 1, LocReg(Lit(0)), LocReg(Lit(3)), LocReg(Lit(4))
   ShLU_W 1, Stk(), LocReg(Lit(0)), Lit(16)
   ShRU_W 1, Stk(), LocReg(Lit(0)), Lit(16)

   MulU_W 1, Stk(), LocReg(Lit(3)), LocReg(Lit(5))
   AddU_W 1, Stk(), Stk(), Stk()

   AddU_W 2, Stk(), Stk(), Stk()
   AddU_W 2, Stk(), Stk(), Stk()

   Retn   2, Stk()
}

;;
;; ___GDCC__MuXU_W2
;;
Function "___GDCC__MuXU_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 4 \
   block
{
   MuXU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Move_W 1, Stk(), Lit(0)

   MuXU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(3))
   AddU_W 2, Stk(), Stk(), Stk()
   Move_W 1, Stk(), Lit(0)

   MuXU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(2))
   Move_W 1, Stk(), Lit(0)
   AddU_W 3, Stk(), Stk(), Stk()

   MuXU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   AddU_W 2, Stk(), Stk(), Stk()

   Retn   4, Stk()
}

;;
;; ___GDCC__MulK_W1
;;
Function "___GDCC__MulK_W1" \
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
   MuXU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Swap_W 1, Stk(), Stk()
   Move_W 1, Nul(), Stk()
   Retn   1, Stk()
}

;;
;; ___GDCC__MulK_W2
;;
Function "___GDCC__MulK_W2" \
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
   MuXU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Swap_W 1, Stk(), Stk()
   Move_W 1, Nul(), Stk()
   Move_W 1, Stk(), Lit(0)

   MuXU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(3))
   AddU_W 2, Stk(), Stk(), Stk()

   MuXU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(2))
   AddU_W 2, Stk(), Stk(), Stk()

   MulU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   AddU_W 1, Stk(), Stk(), Stk()

   Retn   2, Stk()
}

;;
;; ___GDCC__MulU_W2
;;
Function "___GDCC__MulU_W2" \
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
   MuXU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(2))

   MulU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(3))
   AddU_W 1, Stk(), Stk(), Stk()

   MulU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(2))
   AddU_W 1, Stk(), Stk(), Stk()

   Retn   2, Stk()
}

;;
;; ___GDCC__MulU_W3
;;
Function "___GDCC__MulU_W3" \
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
   MuXU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(3))
   Move_W 1, Stk(), Lit(0)

   MuXU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(4))
   AddU_W 2, Stk(), Stk(), Stk()

   MuXU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   AddU_W 2, Stk(), Stk(), Stk()

   MulU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(5))
   AddU_W 1, Stk(), Stk(), Stk()

   MulU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(4))
   AddU_W 1, Stk(), Stk(), Stk()

   MulU_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(3))
   AddU_W 1, Stk(), Stk(), Stk()

   Retn   3, Stk()
}

;;
;; ___GDCC__MulX_W2
;;
Function "___GDCC__MulX_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 4 \
   retrn    = 2 \
   block
{
   ShRI_W 1, LocReg(Lit(4)), LocReg(Lit(1)), Lit(31)
   ShRI_W 1, LocReg(Lit(5)), LocReg(Lit(3)), Lit(31)

   MuXU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Swap_W 1, Stk(), Stk()
   Move_W 1, Nul(), Stk()
   Move_W 1, Stk(), Lit(0)

   MuXU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(3))
   AddU_W 2, Stk(), Stk(), Stk()

   MuXU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(2))
   AddU_W 2, Stk(), Stk(), Stk()

   MulU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(5))
   AddU_W 1, Stk(), Stk(), Stk()

   MulU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   AddU_W 1, Stk(), Stk(), Stk()

   MulU_W 1, Stk(), LocReg(Lit(4)), LocReg(Lit(2))
   AddU_W 1, Stk(), Stk(), Stk()

   Retn   2, Stk()
}

;; EOF

