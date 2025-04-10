label main:
	func_begin main
		a = 0
		$0 = a < 1
		if $0 goto @L6
		goto @L7
	label @L6:
		a = 10000
		$1 = a > 1
		if $1 goto @L3
		goto @L4
	label @L3:
		a = 2
		$2 = a > 1
		if $2 goto @L0
		goto @L1
	label @L0:
		$3 = a
		a = a + 1
		goto @L2
	label @L1:
		$4 = a
		a = a - 1
	label @L2:
		a = 9
		goto @L5
	label @L4:
		a = a + 2
	label @L5:
		$5 = a
		a = a + 1
		goto @L10
	label @L7:
		a = a - 2
	label @L10:
		$6 = a < 9
		if $6 goto @L9
		goto @L11
	label @L9:
		a = a + 4
	label @L8:
	label @L11:
		
		return 0
	func_end main
