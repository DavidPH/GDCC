;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Float add routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;;
;; ___GDCC__AddF_W
;;
Function "___GDCC__AddF_W" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__AddF_W$label" \
   linka    = ExtC \
   localReg = 5 \
   param    = 2 \
   retrn    = 1 \
   block
{
   ; Is l negative? l + r = r + l = r - -l
   AndU_W,   Stk(), LocReg(Lit(0), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__AddF_W$lpos")
   Move_W,   Stk(), LocReg(Lit(1), 0)
   NegF_W,   Stk(), LocReg(Lit(0), 0)
   SubF_W,   Stk(), Stk(), Stk()
   Retn,     Stk()
"___GDCC__AddF_W$lpos"

   ; Is r negative? l + r = l - -r
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__AddF_W$rpos")
   Move_W,   Stk(), LocReg(Lit(0), 0)
   NegF_W,   Stk(), LocReg(Lit(1), 0)
   SubF_W,   Stk(), Stk(), Stk()
   Retn,     Stk()
"___GDCC__AddF_W$rpos"

   ; Does l have special exponent?
   AndU_W, Stk(), LocReg(Lit(0), 0), Lit(0x7F800000)
   Casm,   Lit(84), Lit(0x7F800000), Lit("___GDCC__AddF_W$lemax") ; Cjmp_Lit
   Casm,   Lit(84), Lit(0x00000000), Lit("___GDCC__AddF_W$lemin") ; Cjmp_Lit
   ShRI_W, LocReg(Lit(2), 0), Stk(), Lit(23)

   ; Does r have special exponent?
   AndU_W, Stk(), LocReg(Lit(1), 0), Lit(0x7F800000)
   Casm,   Lit(84), Lit(0x7F800000), Lit("___GDCC__AddF_W$remax") ; Cjmp_Lit
   Casm,   Lit(84), Lit(0x00000000), Lit("___GDCC__AddF_W$remin") ; Cjmp_Lit
   ShRI_W, LocReg(Lit(3), 0), Stk(), Lit(23)


   ; Both are normalized.

   ; Is l.exp > r.exp?
   CmpI_GT_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(3), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__AddF_W$lgtr")

   ; Is l.exp < r.exp?
   CmpI_LT_W, Stk(), LocReg(Lit(2), 0), LocReg(Lit(3), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__AddF_W$lltr")


   ; l.exp == r.exp

   ; l.manfull + r.manfull
   AndU_W, Stk(), LocReg(Lit(0), 0), Lit(0x007FFFFF)
   OrIU_W, Stk(), Stk(),             Lit(0x00800000)
   AndU_W, Stk(), LocReg(Lit(1), 0), Lit(0x007FFFFF)
   OrIU_W, Stk(), Stk(),             Lit(0x00800000)
   AddU_W, LocReg(Lit(4), 0), Stk(), Stk()

   CmpI_LE_W, Stk(), LocReg(Lit(4), 0), Lit(0x00FFFFFF)
   Cjmp_Tru,  Stk(), Lit("___GDCC__AddF_W$leqr_ret")

   ShRI_W, LocReg(Lit(4), 0), LocReg(Lit(4), 0), Lit(1)
   AddI_W, LocReg(Lit(2), 0), LocReg(Lit(2), 0), Lit(1)

   ; If exponent increased to 0xFF, return infinity.
   CmpI_EQ_W, Stk(), LocReg(Lit(2), 0), Lit(0xFF)
   Cjmp_Tru,  Stk(), Lit("___GDCC__AddF_W$infinity")

"___GDCC__AddF_W$leqr_ret"
   ShLU_W, Stk(), LocReg(Lit(2), 0), Lit(23)
   AndU_W, Stk(), LocReg(Lit(4), 0), Lit(0x007FFFFF)
   OrIU_W, Stk(), Stk(), Stk()
   Retn,   Stk()


   ; l.exp > r.exp
"___GDCC__AddF_W$lgtr"
   ; Calculate exponent difference.
   SubU_W, LocReg(Lit(4), 0), LocReg(Lit(2), 0), LocReg(Lit(3), 0)

   ; If difference is >=24, r is too small to affect l.
   CmpI_GE_W, Stk(), LocReg(Lit(4), 0), Lit(24)
   Cjmp_Tru,  Stk(), Lit("___GDCC__AddF_W$remin")

   ; l.manfull + (r.manfull >> difference)
   AndU_W, Stk(), LocReg(Lit(0), 0), Lit(0x007FFFFF)
   OrIU_W, Stk(), Stk(),             Lit(0x00800000)
   AndU_W, Stk(), LocReg(Lit(1), 0), Lit(0x007FFFFF)
   OrIU_W, Stk(), Stk(),             Lit(0x00800000)
   ShRI_W, Stk(), Stk(),             LocReg(Lit(4), 0)
   AddU_W, LocReg(Lit(4), 0), Stk(), Stk()

   CmpI_LE_W, Stk(), LocReg(Lit(4), 0), Lit(0x00FFFFFF)
   Cjmp_Tru,  Stk(), Lit("___GDCC__AddF_W$lgtr_ret")

   ShRI_W, LocReg(Lit(4), 0), LocReg(Lit(4), 0), Lit(1)
   AddI_W, LocReg(Lit(2), 0), LocReg(Lit(2), 0), Lit(1)

   ; If exponent increased to 0xFF, return infinity.
   CmpI_EQ_W, Stk(), LocReg(Lit(2), 0), Lit(0xFF)
   Cjmp_Tru,  Stk(), Lit("___GDCC__AddF_W$infinity")

"___GDCC__AddF_W$lgtr_ret"
   ShLU_W, Stk(), LocReg(Lit(2), 0), Lit(23)
   AndU_W, Stk(), LocReg(Lit(4), 0), Lit(0x007FFFFF)
   OrIU_W, Stk(), Stk(), Stk()
   Retn,   Stk()


   ; l.exp < r.exp
"___GDCC__AddF_W$lltr"
   ; Calculate exponent difference.
   SubU_W, LocReg(Lit(4), 0), LocReg(Lit(3), 0), LocReg(Lit(2), 0)

   ; If difference is >=24, l is too small to affect r.
   CmpI_GE_W, Stk(), LocReg(Lit(4), 0), Lit(24)
   Cjmp_Tru,  Stk(), Lit("___GDCC__AddF_W$lemin")

   ; (l.manfull >> difference) + r.manfull
   AndU_W, Stk(), LocReg(Lit(0), 0), Lit(0x007FFFFF)
   OrIU_W, Stk(), Stk(),             Lit(0x00800000)
   ShRI_W, Stk(), Stk(),             LocReg(Lit(4), 0)
   AndU_W, Stk(), LocReg(Lit(1), 0), Lit(0x007FFFFF)
   OrIU_W, Stk(), Stk(),             Lit(0x00800000)
   AddU_W, LocReg(Lit(4), 0), Stk(), Stk()

   CmpI_LE_W, Stk(), LocReg(Lit(4), 0), Lit(0x00FFFFFF)
   Cjmp_Tru,  Stk(), Lit("___GDCC__AddF_W$lltr_ret")

   ShRI_W, LocReg(Lit(4), 0), LocReg(Lit(4), 0), Lit(1)
   AddI_W, LocReg(Lit(3), 0), LocReg(Lit(3), 0), Lit(1)

   ; If exponent increased to 0xFF, return infinity.
   CmpI_EQ_W, Stk(), LocReg(Lit(3), 0), Lit(0xFF)
   Cjmp_Tru,  Stk(), Lit("___GDCC__AddF_W$infinity")

"___GDCC__AddF_W$lltr_ret"
   ShLU_W, Stk(), LocReg(Lit(3), 0), Lit(23)
   AndU_W, Stk(), LocReg(Lit(4), 0), Lit(0x007FFFFF)
   OrIU_W, Stk(), Stk(), Stk()
   Retn,   Stk()


   ; l has max exponent. It is either INF or NaN.
"___GDCC__AddF_W$lemax"
   ; Is r NaN? If so, return r.
   AndU_W,    Stk(), LocReg(Lit(1), 0), Lit(0x7F800000)
   CmpU_EQ_W, Stk(), Stk(),             Lit(0x7F800000)
   AndU_W,    Stk(), LocReg(Lit(1), 0), Lit(0x007FFFFF)
   Casm,      Lit(70) ; LogAnd
   Cjmp_Tru,  Stk(), Lit("___GDCC__AddF_W$lemin")

   ; r has min exponent. Therefore, r == 0 and the result is l.
"___GDCC__AddF_W$remin"
   Move_W, Stk(), LocReg(Lit(0), 0)
   Retn,   Stk()

   ; r has max exponent. It is either INF or NaN. Either way, return it.
"___GDCC__AddF_W$remax"
   ; l has min exponent. Therefore, l == 0 and the result is r.
"___GDCC__AddF_W$lemin"
   Move_W, Stk(), LocReg(Lit(1), 0)
   Retn,   Stk()

"___GDCC__AddF_W$infinity"
   ; Return infinity.
   Move_W, Stk(), Lit(0x7F800000)
   Retn,   Stk()
}

;;
;; ___GDCC__AddF_W2
;;
Function "___GDCC__AddF_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__AddF_W2$label" \
   linka    = ExtC \
   localReg = 8 \
   param    = 4 \
   retrn    = 2 \
   block
{
   ; Is l negative? l + r = r + l = r - -l
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__AddF_W2$lpos")
   Move_W2,  Stk(), LocReg(Lit(2), 0)
   NegF_W2,  Stk(), LocReg(Lit(0), 0)
   SubF_W2,  Stk(), Stk(), Stk()
   Retn,     Stk(), Stk()
"___GDCC__AddF_W2$lpos"

   ; Is r negative? l + r = l - -r
   AndU_W,   Stk(), LocReg(Lit(3), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__AddF_W2$rpos")
   Move_W2,  Stk(), LocReg(Lit(0), 0)
   NegF_W2,  Stk(), LocReg(Lit(2), 0)
   SubF_W2,  Stk(), Stk(), Stk()
   Retn,     Stk(), Stk()
"___GDCC__AddF_W2$rpos"

   ; Does l have special exponent?
   AndU_W, Stk(), LocReg(Lit(1), 0), Lit(0x7FF00000)
   Casm,   Lit(84), Lit(0x7FF00000), Lit("___GDCC__AddF_W2$lemax") ; Cjmp_Lit
   Casm,   Lit(84), Lit(0x00000000), Lit("___GDCC__AddF_W2$lemin") ; Cjmp_Lit
   ShRI_W, LocReg(Lit(4), 0), Stk(), Lit(20)

   ; Does r have special exponent?
   AndU_W, Stk(), LocReg(Lit(3), 0), Lit(0x7FF00000)
   Casm,   Lit(84), Lit(0x7FF00000), Lit("___GDCC__AddF_W2$remax") ; Cjmp_Lit
   Casm,   Lit(84), Lit(0x00000000), Lit("___GDCC__AddF_W2$remin") ; Cjmp_Lit
   ShRI_W, LocReg(Lit(5), 0), Stk(), Lit(20)


   ; Both are normalized.

   ; Is l.exp > r.exp?
   CmpI_GT_W, Stk(), LocReg(Lit(4), 0), LocReg(Lit(5), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__AddF_W2$lgtr")

   ; Is l.exp < r.exp?
   CmpI_LT_W, Stk(), LocReg(Lit(4), 0), LocReg(Lit(5), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__AddF_W2$lltr")


   ; l.exp == r.exp

   ; l.manfull + r.manfull
   Move_W,  Stk(), LocReg(Lit(0), 0)
   AndU_W,  Stk(), LocReg(Lit(1), 0), Lit(0x000FFFFF)
   OrIU_W,  Stk(), Stk(),             Lit(0x00100000)
   Move_W,  Stk(), LocReg(Lit(2), 0)
   AndU_W,  Stk(), LocReg(Lit(3), 0), Lit(0x000FFFFF)
   OrIU_W,  Stk(), Stk(),             Lit(0x00100000)
   AddU_W2, LocReg(Lit(6), 0), Stk(), Stk()

   CmpI_LE_W, Stk(), LocReg(Lit(7), 0), Lit(0x001FFFFF)
   Cjmp_Tru,  Stk(), Lit("___GDCC__AddF_W2$leqr_ret")

   ShRI_W2, LocReg(Lit(6), 0), LocReg(Lit(6), 0), Lit(1)
   AddI_W,  LocReg(Lit(4), 0), LocReg(Lit(4), 0), Lit(1)

   ; If exponent increased to 0x7FF, return infinity.
   CmpI_EQ_W, Stk(), LocReg(Lit(4), 0), Lit(0x7FF)
   Cjmp_Tru,  Stk(), Lit("___GDCC__AddF_W2$infinity")

"___GDCC__AddF_W2$leqr_ret"
   Move_W, Stk(), LocReg(Lit(6), 0)
   ShLU_W, Stk(), LocReg(Lit(4), 0), Lit(20)
   AndU_W, Stk(), LocReg(Lit(7), 0), Lit(0x000FFFFF)
   OrIU_W, Stk(), Stk(), Stk()
   Retn,   Stk(), Stk()


   ; l.exp > r.exp
"___GDCC__AddF_W2$lgtr"
   ; Calculate exponent difference.
   SubU_W, LocReg(Lit(6), 0), LocReg(Lit(4), 0), LocReg(Lit(5), 0)

   ; If difference is >=53, r is too small to affect l.
   CmpI_GE_W, Stk(), LocReg(Lit(6), 0), Lit(53)
   Cjmp_Tru,  Stk(), Lit("___GDCC__AddF_W2$remin")

   ; l.manfull + (r.manfull >> difference)
   Move_W,  Stk(), LocReg(Lit(0), 0)
   AndU_W,  Stk(), LocReg(Lit(1), 0), Lit(0x000FFFFF)
   OrIU_W,  Stk(), Stk(),             Lit(0x00100000)
   Move_W,  Stk(), LocReg(Lit(2), 0)
   AndU_W,  Stk(), LocReg(Lit(3), 0), Lit(0x000FFFFF)
   OrIU_W,  Stk(), Stk(),             Lit(0x00100000)
   ShRI_W2, Stk(), Stk(),             LocReg(Lit(6), 0)
   AddU_W2, LocReg(Lit(6), 0), Stk(), Stk()

   CmpI_LE_W, Stk(), LocReg(Lit(4), 0), Lit(0x001FFFFF)
   Cjmp_Tru,  Stk(), Lit("___GDCC__AddF_W2$lgtr_ret")

   ShRI_W, LocReg(Lit(4), 0), LocReg(Lit(4), 0), Lit(1)
   AddI_W, LocReg(Lit(2), 0), LocReg(Lit(2), 0), Lit(1)

   ; If exponent increased to 0x7FF, return infinity.
   CmpI_EQ_W, Stk(), LocReg(Lit(4), 0), Lit(0x7FF)
   Cjmp_Tru,  Stk(), Lit("___GDCC__AddF_W2$infinity")

"___GDCC__AddF_W2$lgtr_ret"
   Move_W, Stk(), LocReg(Lit(6), 0)
   ShLU_W, Stk(), LocReg(Lit(4), 0), Lit(20)
   AndU_W, Stk(), LocReg(Lit(7), 0), Lit(0x000FFFFF)
   OrIU_W, Stk(), Stk(), Stk()
   Retn,   Stk(), Stk()


   ; l.exp < r.exp
"___GDCC__AddF_W2$lltr"
   ; Calculate exponent difference.
   SubU_W, LocReg(Lit(6), 0), LocReg(Lit(5), 0), LocReg(Lit(4), 0)

   ; If difference is >=24, l is too small to affect r.
   CmpI_GE_W, Stk(), LocReg(Lit(6), 0), Lit(24)
   Cjmp_Tru,  Stk(), Lit("___GDCC__AddF_W2$lemin")

   ; (l.manfull >> difference) + r.manfull
   Move_W,  Stk(), LocReg(Lit(0), 0)
   AndU_W,  Stk(), LocReg(Lit(1), 0), Lit(0x000FFFFF)
   OrIU_W,  Stk(), Stk(),             Lit(0x00100000)
   ShRI_W2, Stk(), Stk(),             LocReg(Lit(6), 0)
   Move_W,  Stk(), LocReg(Lit(2), 0)
   AndU_W,  Stk(), LocReg(Lit(3), 0), Lit(0x000FFFFF)
   OrIU_W,  Stk(), Stk(),             Lit(0x00100000)
   AddU_W2, LocReg(Lit(6), 0), Stk(), Stk()

   CmpI_LE_W, Stk(), LocReg(Lit(4), 0), Lit(0x001FFFFF)
   Cjmp_Tru,  Stk(), Lit("___GDCC__AddF_W2$lltr_ret")

   ShRI_W, LocReg(Lit(6), 0), LocReg(Lit(6), 0), Lit(1)
   AddI_W, LocReg(Lit(5), 0), LocReg(Lit(5), 0), Lit(1)

   ; If exponent increased to 0x7FF, return infinity.
   CmpI_EQ_W, Stk(), LocReg(Lit(5), 0), Lit(0x7FF)
   Cjmp_Tru,  Stk(), Lit("___GDCC__AddF_W2$infinity")

"___GDCC__AddF_W2$lltr_ret"
   Move_W, Stk(), LocReg(Lit(6), 0)
   ShLU_W, Stk(), LocReg(Lit(5), 0), Lit(20)
   AndU_W, Stk(), LocReg(Lit(7), 0), Lit(0x000FFFFF)
   OrIU_W, Stk(), Stk(), Stk()
   Retn,   Stk(), Stk()


   ; l has max exponent. It is either INF or NaN. Either way, return it.
"___GDCC__AddF_W2$lemax"
   ; Is r NaN? If so, return r.
   AndU_W,    Stk(), LocReg(Lit(3), 0), Lit(0x7FF00000)
   CmpU_EQ_W, Stk(), Stk(),             Lit(0x7FF00000)
   AndU_W,    Stk(), LocReg(Lit(3), 0), Lit(0x000FFFFF)
   OrIU_W,    Stk(), Stk(), LocReg(Lit(2), 0)
   Casm,      Lit(70) ; LogAnd
   Cjmp_Tru,  Stk(), Lit("___GDCC__AddF_W2$lemin")

   ; r has min exponent. Therefore, r == 0 and the result is l.
"___GDCC__AddF_W2$remin"
   Move_W2, Stk(), LocReg(Lit(0), 0)
   Retn,    Stk(), Stk()

   ; r has max exponent. It is either INF or NaN. Either way, return it.
"___GDCC__AddF_W2$remax"
   ; l has min exponent. Therefore, l == 0 and the result is r.
"___GDCC__AddF_W2$lemin"
   Move_W2, Stk(), LocReg(Lit(2), 0)
   Retn,    Stk(), Stk()

"___GDCC__AddF_W2$infinity"
   ; Return infinity.
   Move_W2, Stk(), Lit(0x7FF0000000000000_64.0)
   Retn,    Stk(), Stk()
}

;; EOF

