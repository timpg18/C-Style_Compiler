label main:
	func_begin main
		x#block1 = 2
		$0 = x#block1 > 0
		if $0 goto @L0
		goto @L1
	label @L0:
		$1 = x#block1
		x#block1 = x#block1 + 1
		goto @L2
	label @L1:
		$2 = x#block1
		x#block1 = x#block1 - 1
	label @L2:
		param "%d \n"
		param x#block1
		$3 = call printf, 2
		i#block4 = 0
	label @L3:
		$4 = i#block4 <= 10
		if $4 goto @L4
		goto @L5
	label @L4:
		x#block1 = x#block1 + i#block4
		$5 = i#block4
		i#block4 = i#block4 + 1
		goto @L3
	label @L5:
		param "%d\n"
		param x#block1
		$7 = call printf, 2
		if x#block1 == 1 goto @L6
		if x#block1 == 58 goto @L7
		goto @L8
	label @L6:
		$8 = x#block1
		x#block1 = x#block1 + 1
		goto @L8
	label @L7:
		x#block1 = 20
		goto @L8
	label @L8:
		param "%d\n"
		param x#block1
		$10 = call printf, 2
	func_end main
