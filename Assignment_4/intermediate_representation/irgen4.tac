label main:
	func_begin main
		ww#block1 = 100
		x#block1 = 1
		l#block1 = 0
	label @L0:
		$0 = x#block1 != 10
		if $0 goto @L1
		goto @L2
	label @L1:
		$1 = x#block1
		x#block1 = x#block1 + 1
		goto @L0
		$2 = l#block1
		l#block1 = l#block1 + 1
		goto @L0
	label @L2:
		param "%d %d %d\n"
		param x#block1
		param l#block1
		param ww#block1
		$3 = call printf, 4
	label @L3:
		$4 = x#block1
		x#block1 = x#block1 - 1
	label @L4:
		$5 = x#block1 > 0
		if $5 goto @L3
		goto @L5
	label @L5:
		param "%d \n"
		param x#block1
		$6 = call printf, 2
		t#block1 = 1
		i#block4 = 0
	label @L6:
		$7 = i#block4 < 10
		if $7 goto @L7
		goto @L8
	label @L7:
		$9 = t#block1
		t#block1 = t#block1 + 1
		goto @L8
		$8 = i#block4
		i#block4 = i#block4 + 1
		goto @L6
	label @L8:
		param "%d \n"
		param t#block1
		$10 = call printf, 2
	func_end main
