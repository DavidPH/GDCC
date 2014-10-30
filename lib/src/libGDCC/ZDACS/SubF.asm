;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Float sub routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;;
;; ___GDCC__SubF_W
;;
Function "___GDCC__SubF_W" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__SubF_W$label" \
   linka    = ExtC \
   localReg = 5 \
   param    = 2 \
   retrn    = 1 \
   block
{
   ; +0 - +0 = +0
   OrIU_W,   Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   Cjmp_Nil, Stk(), Lit("___GDCC__SubF_W$remin")

   ; -0 - -0 = +0
   CmpU_EQ_W, Stk(), LocReg(Lit(0), 0), Lit(0x80000000)
   CmpU_EQ_W, Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   AndU_W,    Stk(), Stk(), Stk()
   Cjmp_Tru,  Stk(), Lit("___GDCC__SubF_W$lemin")

   ; Is l negative? l - r = -(-l + r)
   AndU_W,   Stk(), LocReg(Lit(0), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__SubF_W$lpos")
   NegF_W,   Stk(), LocReg(Lit(0), 0)
   Move_W,   Stk(), LocReg(Lit(1), 0)
   AddF_W,   Stk(), Stk(), Stk()
   NegF_W,   Stk(), Stk()
   Retn,     Stk()
"___GDCC__SubF_W$lpos"

   ; Is r negative? l - r = l + -r
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__SubF_W$rpos")
   Move_W,   Stk(), LocReg(Lit(0), 0)
   NegF_W,   Stk(), LocReg(Lit(1), 0)
   AddF_W,   Stk(), Stk(), Stk()
   Retn,     Stk()
"___GDCC__SubF_W$rpos"

   ; Does l have special exponent?
   AndU_W, Stk(), LocReg(Lit(0), 0), Lit(0x7F800000)
   Casm,   Lit(84), Lit(0x7F800000), Lit("___GDCC__SubF_W$lemax") ; Cjmp_Lit
   Casm,   Lit(84), Lit(0x00000000), Lit("___GDCC__SubF_W$lemin") ; Cjmp_Lit
   ShRI_W, LocReg(Lit(2), 0), Stk(), Lit(23)

   ; Does r have special exponent?
   AndU_W, Stk(), LocReg(Lit(1), 0), Lit(0x7F800000)
   Casm,   Lit(84), Lit(0x7F800000), Lit("___GDCC__SubF_W$remax") ; Cjmp_Lit
   Casm,   Lit(84), Lit(0x00000000), Lit("___GDCC__SubF_W$remin") ; Cjmp_Lit
   ShRI_W, LocReg(Lit(3), 0), Stk(), Lit(23)


   ; Both are normalized.

   ; Is l > r?
   CmpI_GT_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__SubF_W$lgtr")

   ; Is l < r?
   CmpI_LT_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   Cjmp_Tru,  Stk(), Lit("___GDCC__SubF_W$lltr")


   ; l == r

   ; Return 0.
   Move_W, Stk(), Lit(0x00000000)
   Retn,   Stk()


   ; l > r
"___GDCC__SubF_W$lgtr"
   ; Calculate exponent difference.
   SubU_W, LocReg(Lit(4), 0), LocReg(Lit(2), 0), LocReg(Lit(3), 0)

   ; If difference is >=24, r is too small to affect l.
   CmpI_GE_W, Stk(), LocReg(Lit(4), 0), Lit(24)
   Cjmp_Tru,  Stk(), Lit("___GDCC__SubF_W$remin")

   ; l.manfull - (r.manfull >> difference)
   AndU_W, Stk(), LocReg(Lit(0), 0), Lit(0x007FFFFF)
   OrIU_W, Stk(), Stk(),             Lit(0x00800000)
   AndU_W, Stk(), LocReg(Lit(1), 0), Lit(0x007FFFFF)
   OrIU_W, Stk(), Stk(),             Lit(0x00800000)
   ShRI_W, Stk(), Stk(),             LocReg(Lit(4), 0)
   SubU_W, LocReg(Lit(4), 0), Stk(), Stk()

   CmpI_LE_W, Stk(), LocReg(Lit(4), 0), Lit(0x00FFFFFF)
   Cjmp_Tru,  Stk(), Lit("___GDCC__SubF_W$lgtr_shlc")

   ShRI_W, LocReg(Lit(4), 0), LocReg(Lit(4), 0), Lit(1)
   AddI_W, LocReg(Lit(2), 0), LocReg(Lit(2), 0), Lit(1)
   Jump,   Lit("___GDCC__SubF_W$lgtr_ret")

"___GDCC__SubF_W$lgtr_shl"
   ShLU_W, LocReg(Lit(4), 0), LocReg(Lit(4), 0), Lit(1)
   SubI_W, LocReg(Lit(2), 0), LocReg(Lit(2), 0), Lit(1)

   Move_W,   Stk(), LocReg(Lit(2), 0)
   Cjmp_Nil, Stk(), Lit("___GDCC__SubF_W$poszero")

"___GDCC__SubF_W$lgtr_shlc"
   CmpI_LE_W, Stk(), LocReg(Lit(4), 0), Lit(0x007FFFFF)
   Cjmp_Tru,  Stk(), Lit("___GDCC__SubF_W$lgtr_shl")

"___GDCC__SubF_W$lgtr_ret"
   ShLU_W, Stk(), LocReg(Lit(2), 0), Lit(23)
   AndU_W, Stk(), LocReg(Lit(4), 0), Lit(0x007FFFFF)
   OrIU_W, Stk(), Stk(), Stk()
   Retn,   Stk()


   ; l < r
"___GDCC__SubF_W$lltr"
   ; Calculate exponent difference.
   SubU_W, LocReg(Lit(4), 0), LocReg(Lit(3), 0), LocReg(Lit(2), 0)

   ; If difference is >=24, l is too small to affect r.
   CmpI_GE_W, Stk(), LocReg(Lit(4), 0), Lit(24)
   Cjmp_Tru,  Stk(), Lit("___GDCC__SubF_W$lemin")

   ; -(r.manfull - (l.manfull >> difference))
   AndU_W, Stk(), LocReg(Lit(1), 0), Lit(0x007FFFFF)
   OrIU_W, Stk(), Stk(),             Lit(0x00800000)
   AndU_W, Stk(), LocReg(Lit(0), 0), Lit(0x007FFFFF)
   OrIU_W, Stk(), Stk(),             Lit(0x00800000)
   ShRI_W, Stk(), Stk(),             LocReg(Lit(4), 0)
   SubU_W, LocReg(Lit(4), 0), Stk(), Stk()

   CmpI_LE_W, Stk(), LocReg(Lit(4), 0), Lit(0x00FFFFFF)
   Cjmp_Tru,  Stk(), Lit("___GDCC__SubF_W$lltr_shlc")

   ShRI_W, LocReg(Lit(4), 0), LocReg(Lit(4), 0), Lit(1)
   AddI_W, LocReg(Lit(3), 0), LocReg(Lit(3), 0), Lit(1)
   Jump,   Lit("___GDCC__SubF_W$lltr_ret")

"___GDCC__SubF_W$lltr_shl"
   ShLU_W, LocReg(Lit(4), 0), LocReg(Lit(4), 0), Lit(1)
   SubI_W, LocReg(Lit(3), 0), LocReg(Lit(3), 0), Lit(1)

   Move_W,   Stk(), LocReg(Lit(3), 0)
   Cjmp_Nil, Stk(), Lit("___GDCC__SubF_W$negzero")

"___GDCC__SubF_W$lltr_shlc"
   CmpI_LE_W, Stk(), LocReg(Lit(4), 0), Lit(0x007FFFFF)
   Cjmp_Tru,  Stk(), Lit("___GDCC__SubF_W$lltr_shl")

"___GDCC__SubF_W$lltr_ret"
   ShLU_W, Stk(), LocReg(Lit(3), 0), Lit(23)
   AndU_W, Stk(), LocReg(Lit(4), 0), Lit(0x007FFFFF)
   OrIU_W, Stk(), Stk(), Stk()
   NegF_W, Stk(), Stk()
   Retn,   Stk()


   ; l has max exponent. It is either INF or NaN.
"___GDCC__SubF_W$lemax"
   ; Is r also max exponent?
   AndU_W,    Stk(), LocReg(Lit(1), 0), Lit(0x7F800000)
   CmpU_EQ_W, Stk(), Stk(),             Lit(0x7F800000)
   Cjmp_Tru,  Stk(), Lit("___GDCC__SubF_W$lremax")

   ; r has min exponent. Therefore, r == 0 and the result is l.
"___GDCC__SubF_W$remin"
   Move_W, Stk(), LocReg(Lit(0), 0)
   Retn,   Stk()

   ; r has max exponent. It is either INF or NaN. Either way, return it.
"___GDCC__SubF_W$remax"
   ; l has min exponent. Therefore, l == 0 and the result is -r.
"___GDCC__SubF_W$lemin"
   NegF_W, Stk(), LocReg(Lit(1), 0)
   Retn,   Stk()

   ; l and r have max exponent.
"___GDCC__SubF_W$lremax"
   ; Is l NaN? If so, return l.
   AndU_W,   Stk(), LocReg(Lit(0), 0), Lit(0x007FFFFF)
   Cjmp_Tru, Stk(), Lit("___GDCC__SubF_W$remin")

   ; Is r NaN? If so, return r. (Sign inversion is fine.)
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(0x007FFFFF)
   Cjmp_Tru, Stk(), Lit("___GDCC__SubF_W$lemin")

   ; +INF - +INF = NaN
   Move_W, Stk(), Lit(0x7FFFFFFF)
   Retn,   Stk()

"___GDCC__SubF_W$poszero"
   Move_W, Stk(), Lit(0x00000000)
   Retn,   Stk()

"___GDCC__SubF_W$negzero"
   Move_W, Stk(), Lit(0x80000000)
   Retn,   Stk()
}

;;
;; ___GDCC__SubF_W2
;;
Function "___GDCC__SubF_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__SubF_W2$label" \
   linka    = ExtC \
   localReg = 8 \
   param    = 4 \
   retrn    = 2 \
   block
{
   ; +0 - +0 = +0
   OrIU_W,   Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Cjmp_Nil, Stk(), Lit("___GDCC__SubF_W2$remin")

   ; -0 - -0 = +0
   CmpU_EQ_W, Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   CmpU_EQ_W, Stk(), LocReg(Lit(3), 0), Lit(0x80000000)
   AndU_W,    Stk(), Stk(), Stk()
   Cjmp_Tru,  Stk(), Lit("___GDCC__SubF_W2$lemin")

   ; Is l negative? l - r = -(-l + r)
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__SubF_W2$lpos")
   NegF_W2,  Stk(), LocReg(Lit(0), 0)
   Move_W2,  Stk(), LocReg(Lit(2), 0)
   AddF_W2,  Stk(), Stk(), Stk()
   NegF_W2,  Stk(), Stk()
   Retn,     Stk(), Stk()
"___GDCC__SubF_W2$lpos"

   ; Is r negative? l - r = l + -r
   AndU_W,   Stk(), LocReg(Lit(3), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__SubF_W2$rpos")
   Move_W2,  Stk(), LocReg(Lit(0), 0)
   NegF_W2,  Stk(), LocReg(Lit(2), 0)
   AddF_W2,  Stk(), Stk(), Stk()
   Retn,     Stk(), Stk()
"___GDCC__SubF_W2$rpos"

   ; Does l have special exponent?
   AndU_W, Stk(), LocReg(Lit(1), 0), Lit(0x7FF00000)
   Casm,   Lit(84), Lit(0x7FF00000), Lit("___GDCC__SubF_W2$lemax") ; Cjmp_Lit
   Casm,   Lit(84), Lit(0x00000000), Lit("___GDCC__SubF_W2$lemin") ; Cjmp_Lit
   ShRI_W, LocReg(Lit(4), 0), Stk(), Lit(20)

   ; Does r have special exponent?
   AndU_W, Stk(), LocReg(Lit(3), 0), Lit(0x7FF00000)
   Casm,   Lit(84), Lit(0x7FF00000), Lit("___GDCC__SubF_W2$remax") ; Cjmp_Lit
   Casm,   Lit(84), Lit(0x00000000), Lit("___GDCC__SubF_W2$remin") ; Cjmp_Lit
   ShRI_W, LocReg(Lit(5), 0), Stk(), Lit(20)


   ; Both are normalized.

   ; Is l > r?
   CmpI_GT_W2, Stk(), LocReg(Lit(0), 0), LocReg(Lit(2), 0)
   Cjmp_Tru,   Stk(), Lit("___GDCC__SubF_W2$lgtr")

   ; Is l < r?
   CmpI_LT_W2, Stk(), LocReg(Lit(0), 0), LocReg(Lit(2), 0)
   Cjmp_Tru,   Stk(), Lit("___GDCC__SubF_W2$lltr")


   ; l == r

   ; Return 0.
   Move_W2, Stk(), Lit(0x0000000000000000_64.0)
   Retn,    Stk(), Stk()


   ; l > r
"___GDCC__SubF_W2$lgtr"
   ; Calculate exponent difference.
   SubU_W, LocReg(Lit(6), 0), LocReg(Lit(4), 0), LocReg(Lit(5), 0)

   ; If difference is >=53, r is too small to affect l.
   CmpI_GE_W, Stk(), LocReg(Lit(6), 0), Lit(53)
   Cjmp_Tru,  Stk(), Lit("___GDCC__SubF_W2$remin")

   ; l.manfull - (r.manfull >> difference)
   Move_W,  Stk(), LocReg(Lit(0), 0)
   AndU_W,  Stk(), LocReg(Lit(1), 0), Lit(0x000FFFFF)
   OrIU_W,  Stk(), Stk(),             Lit(0x00100000)
   Move_W,  Stk(), LocReg(Lit(2), 0)
   AndU_W,  Stk(), LocReg(Lit(3), 0), Lit(0x000FFFFF)
   OrIU_W,  Stk(), Stk(),             Lit(0x00100000)
   ShRI_W2, Stk(), Stk(),             LocReg(Lit(6), 0)
   SubU_W2, LocReg(Lit(6), 0), Stk(), Stk()

   CmpI_LE_W, Stk(), LocReg(Lit(7), 0), Lit(0x001FFFFF)
   Cjmp_Tru,  Stk(), Lit("___GDCC__SubF_W2$lgtr_shlc")

   ShRI_W2, LocReg(Lit(6), 0), LocReg(Lit(6), 0), Lit(1)
   AddI_W,  LocReg(Lit(4), 0), LocReg(Lit(4), 0), Lit(1)
   Jump,    Lit("___GDCC__SubF_W2$lgtr_ret")

"___GDCC__SubF_W2$lgtr_shl"
   ShLU_W2, LocReg(Lit(6), 0), LocReg(Lit(6), 0), Lit(1)
   SubI_W,  LocReg(Lit(4), 0), LocReg(Lit(4), 0), Lit(1)

   Move_W,   Stk(), LocReg(Lit(4), 0)
   Cjmp_Nil, Stk(), Lit("___GDCC__SubF_W2$poszero")

"___GDCC__SubF_W2$lgtr_shlc"
   CmpI_LE_W, Stk(), LocReg(Lit(7), 0), Lit(0x000FFFFF)
   Cjmp_Tru,  Stk(), Lit("___GDCC__SubF_W2$lgtr_shl")

"___GDCC__SubF_W2$lgtr_ret"
   Move_W, Stk(), LocReg(Lit(6), 0)
   ShLU_W, Stk(), LocReg(Lit(4), 0), Lit(20)
   AndU_W, Stk(), LocReg(Lit(7), 0), Lit(0x000FFFFF)
   OrIU_W, Stk(), Stk(), Stk()
   Retn,   Stk(), Stk()


   ; l < r
"___GDCC__SubF_W2$lltr"
   ; Calculate exponent difference.
   SubU_W, LocReg(Lit(6), 0), LocReg(Lit(5), 0), LocReg(Lit(4), 0)

   ; If difference is >=53, l is too small to affect r.
   CmpI_GE_W, Stk(), LocReg(Lit(6), 0), Lit(53)
   Cjmp_Tru,  Stk(), Lit("___GDCC__SubF_W2$lemin")

   ; -(r.manfull - (l.manfull >> difference))
   Move_W,  Stk(), LocReg(Lit(2), 0)
   AndU_W,  Stk(), LocReg(Lit(3), 0), Lit(0x000FFFFF)
   OrIU_W,  Stk(), Stk(),             Lit(0x00100000)
   Move_W,  Stk(), LocReg(Lit(0), 0)
   AndU_W,  Stk(), LocReg(Lit(1), 0), Lit(0x000FFFFF)
   OrIU_W,  Stk(), Stk(),             Lit(0x00100000)
   ShRI_W2, Stk(), Stk(),             LocReg(Lit(6), 0)
   SubU_W2, LocReg(Lit(6), 0), Stk(), Stk()

   CmpI_LE_W, Stk(), LocReg(Lit(7), 0), Lit(0x001FFFFF)
   Cjmp_Tru,  Stk(), Lit("___GDCC__SubF_W2$lltr_shlc")

   ShRI_W2, LocReg(Lit(6), 0), LocReg(Lit(6), 0), Lit(1)
   AddI_W,  LocReg(Lit(5), 0), LocReg(Lit(5), 0), Lit(1)
   Jump,    Lit("___GDCC__SubF_W2$lltr_ret")

"___GDCC__SubF_W2$lltr_shl"
   ShLU_W2, LocReg(Lit(6), 0), LocReg(Lit(6), 0), Lit(1)
   SubI_W,  LocReg(Lit(5), 0), LocReg(Lit(5), 0), Lit(1)

   Move_W,   Stk(), LocReg(Lit(5), 0)
   Cjmp_Nil, Stk(), Lit("___GDCC__SubF_W2$negzero")

"___GDCC__SubF_W2$lltr_shlc"
   CmpI_LE_W, Stk(), LocReg(Lit(7), 0), Lit(0x000FFFFF)
   Cjmp_Tru,  Stk(), Lit("___GDCC__SubF_W2$lltr_shl")

"___GDCC__SubF_W2$lltr_ret"
   Move_W, Stk(), LocReg(Lit(6), 0)
   ShLU_W, Stk(), LocReg(Lit(5), 0), Lit(20)
   AndU_W, Stk(), LocReg(Lit(7), 0), Lit(0x000FFFFF)
   OrIU_W, Stk(), Stk(), Stk()
   NegF_W, Stk(), Stk()
   Retn,   Stk(), Stk()


   ; l has max exponent. It is either INF or NaN.
"___GDCC__SubF_W2$lemax"
   ; Is r also max exponent?
   AndU_W,    Stk(), LocReg(Lit(3), 0), Lit(0x7FF00000)
   CmpU_EQ_W, Stk(), Stk(),             Lit(0x7FF00000)
   Cjmp_Tru,  Stk(), Lit("___GDCC__SubF_W2$lremax")

   ; r has min exponent. Therefore, r == 0 and the result is l.
"___GDCC__SubF_W2$remin"
   Move_W2, Stk(), LocReg(Lit(0), 0)
   Retn,    Stk(), Stk()

   ; r has max exponent. It is either INF or NaN. Either way, return it.
"___GDCC__SubF_W2$remax"
   ; l has min exponent. Therefore, l == 0 and the result is -r.
"___GDCC__SubF_W2$lemin"
   NegF_W2, Stk(), LocReg(Lit(2), 0)
   Retn,    Stk(), Stk()

   ; l and r have max exponent.
"___GDCC__SubF_W2$lremax"
   ; Is l NaN? If so, return l.
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(0x000FFFFF)
   OrIU_W,   Stk(), Stk(), LocReg(Lit(0), 0)
   Cjmp_Tru, Stk(), Lit("___GDCC__SubF_W2$remin")

   ; Is r NaN? If so, return r. (Sign inversion is fine.)
   AndU_W,   Stk(), LocReg(Lit(3), 0), Lit(0x000FFFFF)
   OrIU_W,   Stk(), Stk(), LocReg(Lit(2), 0)
   Cjmp_Tru, Stk(), Lit("___GDCC__SubF_W2$lemin")

   ; +INF - +INF = NaN
   Move_W2, Stk(), Lit(0x7FFFFFFFFFFFFFFF_64.0)
   Retn,    Stk(), Stk()

"___GDCC__SubF_W2$poszero"
   ; Return +0.
   Move_W2, Stk(), Lit(0x0000000000000000_64.0)
   Retn,    Stk(), Stk()

"___GDCC__SubF_W2$negzero"
   ; Return -0.
   Move_W2, Stk(), Lit(0x8000000000000000_64.0)
   Retn,    Stk(), Stk()
}

;; EOF

