label f:
	func_begin f
		
		return 0
	func_end f
label main:
	func_begin main
<<<<<<< HEAD
		param 1
		param 2
		param 4
		param 9.0
		param 9.8
		param 4
		$0 = call f, 6
		b#block2 = $0
=======
		$0 = 0 * 4
		x#block5[$0] = 10
		$2 = 5 * 1
		ch#block5[$2] = 'k'
		$4 = 0 * 4
		y#block5 = x#block5[$4]
		$5 = 1 * 4
		x#block5[$5] = 1
		$7 = 2 * 4
		x#block5[$7] = 2
		$9 = 5 * 1
		g#block5 = ch#block5[$9]
		param "%d %c k \n"
		param y#block5
		param g#block5
		$10 = call printf, 3
		$11 = 1 * 4
		$12 = 2 * 4
		$14 = x#block5[$11]
		$15 = x#block5[$12]
		$13 = $14 + $15
		f#block5 = $13
		$16 = cast: float -> double 7.0
		d#block5 = $16
		param "%d %f \n"
		param f#block5
		param d#block5
		$17 = call printf, 3
		$18 = 0 + 0
		c#block5[$18] = 1
		$20 = 0 + 4
		c#block5[$20] = 'k'
		$22 = 0 + 0
		xx#block5 = c#block5[$22]
		$23 = 0 + 4
		ck#block5 = c#block5[$23]
		param "%d %c \n"
		param xx#block5
		param ck#block5
		$24 = call printf, 3
		$25 = 0 + 0
		obj1#block5[$25] = 5
		$27 = 0 + 0
		obj1#block5[$27] = 'k'
		$29 = 0 + 0
		xx#block5 = obj1#block5[$29]
		$31 = 0 + 0
		ck#block5 = obj1#block5[$31]
		param "%d %c \n"
		param xx#block5
		param ck#block5
		$33 = call printf, 3
		gg#block5 = 1
		xx#block5 = 1
		param "%d %c \n"
		param xx#block5
		param ck#block5
		$35 = call printf, 3
		$36 = 0 + 8
		$37 = $36 + 1
		ob#block5[$37] = 5
		$39 = 0 + 8
		$40 = $39 + 0
		ob#block5[$40] = 'b'
		$42 = 0 + 8
		$43 = $42 + 1
		ww#block5 = ob#block5[$43]
		cp#block5 = 'b'
		param "%d %c \n"
		param ww#block5
		param cp#block5
		$44 = call printf, 3
>>>>>>> origin/codegen
	func_end main
