label f:
	func_begin f
		param "we are passing a lot of param boiii \n"
		$0 = call printf, 1
		
		return 1
	func_end f
label main:
	func_begin main
		param 1
		param 2
		param 4
		param 9.0
		param 9.8
		param 4
		$1 = call f, 6
		b#block2 = $1
		$2 = &q#block2
		p#block2 = $2
		$4 = &b#block2
		a#block2 = $4
		$5 = *a#block2
		x#block2 = $5
		t#block2 = 2
		$6 = &t#block2
		t1#block2 = $6
		$7 = &t1#block2
		t2#block2 = $7
		$8 = &t2#block2
		p#block2 = $8
		$10 = *p#block2
		$11 = *$10
		$12 = *$11
		xx#block2 = $12
		param "%d %d\n"
		param x#block2
		param xx#block2
		$13 = call printf, 3
		$14 = 3 * 4
		ar#block2[$14] = t1#block2
		$16 = *t1#block2
		soln#block2 = $16
		param "%d \n"
		param soln#block2
		$17 = call printf, 2
		i#block3 = 0
	label @L6:
		$18 = i#block3 < 4
		if $18 goto @L7
		goto @L8
	label @L7:
		j#block5 = 0
	label @L3:
		$20 = j#block5 < 5
		if $20 goto @L4
		goto @L5
	label @L4:
		k#block7 = 0
	label @L0:
		$22 = k#block7 < 6
		if $22 goto @L1
		goto @L2
	label @L1:
		$24 = i#block3 + j#block5
		$25 = $24 + k#block7
		x#block8 = $25
		param "%d "
		param x#block8
		$26 = call printf, 2
		$23 = k#block7
		k#block7 = k#block7 + 1
		goto @L0
	label @L2:
		$21 = j#block5
		j#block5 = j#block5 + 1
		goto @L3
	label @L5:
		$19 = i#block3
		i#block3 = i#block3 + 1
		goto @L6
	label @L8:
	func_end main
