;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014-2015 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Multi-word add routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;;
;; ___GDCC__AddU_W2
;;
Function "___GDCC__AddU_W2" \
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
   AdXU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   AddU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   AddU_W 1, Stk(), Stk(), Stk()
   Retn   2, Stk()
}

;;
;; ___GDCC__AddU_W3
;;
Function "___GDCC__AddU_W3" \
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
   AdXU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(3))
   AdXU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(4)), Stk()
   AddU_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(5))
   AddU_W 1, Stk(), Stk(), Stk()
   Retn   3, Stk()
}

;; EOF

