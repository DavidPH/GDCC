;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Three-word comparison routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;;
;; ___GDCC__CmpI_GE_W3
;;
Function "___GDCC__CmpI_GE_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__CmpI_GE_W3$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 6 \
   retrn    = 1 \
   block
{
   CmpI_EQ_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(5), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__CmpI_GE_W3$eq2")

   CmpI_GT_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(5), 0)
   Retn,      Stk()

"___GDCC__CmpI_GE_W3$eq2"
   CmpU_EQ_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(4), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__CmpI_GE_W3$eq1")

   CmpU_GT_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(4), 0)
   Retn,      Stk()

"___GDCC__CmpI_GE_W3$eq1"
   CmpU_GE_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(3), 0)
   Retn,      Stk()
}

;;
;; ___GDCC__CmpI_GT_W3
;;
Function "___GDCC__CmpI_GT_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__CmpI_GT_W3$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 6 \
   retrn    = 1 \
   block
{
   CmpI_EQ_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(5), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__CmpI_GT_W3$eq2")

   CmpI_GT_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(5), 0)
   Retn,      Stk()

"___GDCC__CmpI_GT_W3$eq2"
   CmpU_EQ_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(4), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__CmpI_GT_W3$eq1")

   CmpU_GT_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(4), 0)
   Retn,      Stk()

"___GDCC__CmpI_GT_W3$eq1"
   CmpU_GT_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(3), 0)
   Retn,      Stk()
}

;;
;; ___GDCC__CmpI_LE_W3
;;
Function "___GDCC__CmpI_LE_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__CmpI_LE_W3$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 6 \
   retrn    = 1 \
   block
{
   CmpI_EQ_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(5), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__CmpI_LE_W3$eq2")

   CmpI_LT_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(5), 0)
   Retn,      Stk()

"___GDCC__CmpI_LE_W3$eq2"
   CmpU_EQ_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(4), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__CmpI_LE_W3$eq1")

   CmpU_LT_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(4), 0)
   Retn,      Stk()

"___GDCC__CmpI_LE_W3$eq1"
   CmpU_LE_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(3), 0)
   Retn,      Stk()
}

;;
;; ___GDCC__CmpI_LT_W3
;;
Function "___GDCC__CmpI_LT_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__CmpI_LT_W3$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 6 \
   retrn    = 1 \
   block
{
   CmpI_EQ_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(5), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__CmpI_LT_W3$eq2")

   CmpI_LT_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(5), 0)
   Retn,      Stk()

"___GDCC__CmpI_LT_W3$eq2"
   CmpU_EQ_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(4), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__CmpI_LT_W3$eq1")

   CmpU_LT_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(4), 0)
   Retn,      Stk()

"___GDCC__CmpI_LT_W3$eq1"
   CmpU_LT_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(3), 0)
   Retn,      Stk()
}

;;
;; ___GDCC__CmpU_EQ_W3
;;
Function "___GDCC__CmpU_EQ_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__CmpU_EQ_W3$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 6 \
   retrn    = 1 \
   block
{
   CmpU_EQ_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(3), 0)
   CmpU_EQ_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(4), 0)
   AndU_W,    Stk(), Stk(), Stk()
   CmpU_EQ_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(5), 0)
   AndU_W,    Stk(), Stk(), Stk()
   Retn,      Stk()
}

;;
;; ___GDCC__CmpU_GE_W3
;;
Function "___GDCC__CmpU_GE_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__CmpU_GE_W3$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 6 \
   retrn    = 1 \
   block
{
   CmpU_EQ_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(5), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__CmpU_GE_W3$eq2")

   CmpU_GT_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(5), 0)
   Retn,      Stk()

"___GDCC__CmpU_GE_W3$eq2"
   CmpU_EQ_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(4), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__CmpU_GE_W3$eq1")

   CmpU_GT_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(4), 0)
   Retn,      Stk()

"___GDCC__CmpU_GE_W3$eq1"
   CmpU_GE_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(3), 0)
   Retn,      Stk()
}

;;
;; ___GDCC__CmpU_GT_W3
;;
Function "___GDCC__CmpU_GT_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__CmpU_GT_W3$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 6 \
   retrn    = 1 \
   block
{
   CmpU_EQ_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(5), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__CmpU_GT_W3$eq2")

   CmpU_GT_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(5), 0)
   Retn,      Stk()

"___GDCC__CmpU_GT_W3$eq2"
   CmpU_EQ_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(4), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__CmpU_GT_W3$eq1")

   CmpU_GT_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(4), 0)
   Retn,      Stk()

"___GDCC__CmpU_GT_W3$eq1"
   CmpU_GT_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(3), 0)
   Retn,      Stk()
}

;;
;; ___GDCC__CmpU_LE_W3
;;
Function "___GDCC__CmpU_LE_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__CmpU_LE_W3$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 6 \
   retrn    = 1 \
   block
{
   CmpU_EQ_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(5), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__CmpU_LE_W3$eq2")

   CmpU_LT_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(5), 0)
   Retn,      Stk()

"___GDCC__CmpU_LE_W3$eq2"
   CmpU_EQ_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(4), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__CmpU_LE_W3$eq1")

   CmpU_LT_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(4), 0)
   Retn,      Stk()

"___GDCC__CmpU_LE_W3$eq1"
   CmpU_LE_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(3), 0)
   Retn,      Stk()
}

;;
;; ___GDCC__CmpU_LT_W3
;;
Function "___GDCC__CmpU_LT_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__CmpU_LT_W3$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 6 \
   retrn    = 1 \
   block
{
   CmpU_EQ_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(5), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__CmpU_LT_W3$eq2")

   CmpU_LT_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(5), 0)
   Retn,      Stk()

"___GDCC__CmpU_LT_W3$eq2"
   CmpU_EQ_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(4), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__CmpU_LT_W3$eq1")

   CmpU_LT_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(4), 0)
   Retn,      Stk()

"___GDCC__CmpU_LT_W3$eq1"
   CmpU_LT_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(3), 0)
   Retn,      Stk()
}

;;
;; ___GDCC__CmpU_NE_W3
;;
Function "___GDCC__CmpU_NE_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__CmpU_NE_W3$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 6 \
   retrn    = 1 \
   block
{
   CmpU_NE_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(3), 0)
   CmpU_NE_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(4), 0)
   OrIU_W,    Stk(), Stk(), Stk()
   CmpU_NE_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(5), 0)
   OrIU_W,    Stk(), Stk(), Stk()
   Retn,      Stk()
}

;; EOF

