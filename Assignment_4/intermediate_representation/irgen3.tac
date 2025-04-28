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
		$10 = x#block1 < 0
		if $10 goto @L9
		goto @L10
	label @L9:
		$11 = x#block1
		x#block1 = x#block1 + 1
		goto @L11
	label @L10:
		$12 = x#block1
		x#block1 = x#block1 - 1
	label @L11:
		param "%d \n"
		param x#block1
		$13 = call printf, 2
		t#block1 = 0
		i#block8 = 0
	label @L15:
		$14 = i#block8 <= 10
		if $14 goto @L16
		goto @L17
	label @L16:
		j#block10 = 0
	label @L12:
		$16 = -5
		$17 = j#block10 > $16
		if $17 goto @L13
		goto @L14
	label @L13:
		$19 = i#block8 - j#block10
		t#block1 = t#block1 + $19
		$18 = j#block10
		j#block10 = j#block10 - 1
		goto @L12
	label @L14:
		$15 = i#block8
		i#block8 = i#block8 + 1
		goto @L15
	label @L17:
		param "%d %d\n"
		param x#block1
		param t#block1
		$21 = call printf, 3
	func_end main
