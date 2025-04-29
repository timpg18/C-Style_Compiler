label f:
	func_begin f
		
		return 0
	func_end f
label main:
	func_begin main
		$0 = 0 * 4
		x#block6[$0] = 10
		$2 = 5 * 1
		ch#block6[$2] = 'k'
		$4 = 0 * 4
		y#block6 = x#block6[$4]
		$5 = 1 * 4
		x#block6[$5] = 1
		$7 = 2 * 4
		x#block6[$7] = 2
		$9 = 5 * 1
		g#block6 = ch#block6[$9]
		param "%d %c k \n"
		param y#block6
		param g#block6
		$10 = call printf, 3
		$11 = 0 + 0
		c#block6[$11] = 1
		$13 = 0 + 4
		c#block6[$13] = 'k'
		$15 = 0 + 0
		xx#block6 = c#block6[$15]
		$16 = 0 + 4
		ck#block6 = c#block6[$16]
		param "%d %c \n"
		param xx#block6
		param ck#block6
		$17 = call printf, 3
		$18 = 0 + 0
		obj1#block6[$18] = 5
		$20 = 0 + 0
		obj1#block6[$20] = 'k'
		$22 = 0 + 0
		xx#block6 = obj1#block6[$22]
		$24 = 0 + 0
		ck#block6 = obj1#block6[$24]
		param "%d %c \n"
		param xx#block6
		param ck#block6
		$26 = call printf, 3
		gg#block6 = 1
		xx#block6 = 1
		param "%d %c \n"
		param xx#block6
		param ck#block6
		$28 = call printf, 3
		$29 = 0 + 8
		$30 = $29 + 1
		ob#block6[$30] = 5
		$32 = 0 + 8
		$33 = $32 + 0
		ob#block6[$33] = 'b'
		$35 = 0 + 8
		$36 = $35 + 1
		ww#block6 = ob#block6[$36]
		cp#block6 = 'b'
		param "%d %c \n"
		param ww#block6
		param cp#block6
		$37 = call printf, 3
		$38 = 0 * 4
		a#block6[$38] = 9
		$40 = 2 * 4
		a#block6[$40] = 10
		$42 = 1 * 4
		$43 = 2 * 4
		$45 = a#block6[$42]
		$46 = a#block6[$43]
		$44 = $45 + $46
		ff#block6 = $44
		$47 = cast: float -> double 9.0
		cc#block6 = $47
		$48 = cast: float -> double 7.0
		d#block6 = $48
		$49 = cc#block6 + d#block6
		b#block6 = $49
		param "%f %f"
		param cc#block6
		param d#block6
		$50 = call printf, 3
		param "%d %f\n"
		param ff#block6
		param b#block6
		$51 = call printf, 3
	func_end main
