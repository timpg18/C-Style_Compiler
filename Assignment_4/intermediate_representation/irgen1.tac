label main:
	func_begin main
		a = 0
		$0 = a > 1
		if $0 goto @L4
		goto @L5
	label @L4:
		$1 = a
		a = a + 1
		goto @L1
	label @L5:
		$2 = a
		a = a - 1
		$3 = a < 10
		if $3 goto @L2
		goto @L1
	label @L2:
		a = a - 9
		$4 = a > 700
		if $4 goto @L0
		goto @L1
	label @L0:
		a = a - 9000
	label @L1:
		
		return 0
	func_end main
