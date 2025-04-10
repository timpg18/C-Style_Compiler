label main:
	func_begin main
		a = 0
		i = 0
	label @L2:
		$0 = i < 5
		if $0 goto @L3
		goto @L4
	label @L3:
		$2 = i == 2
		if $2 goto @L0
		goto @L1
	label @L0:
		goto @L2
	label @L1:
		param "For with break/continue: i = %d\n"
		param i
		$3 = call printf, 2
		$1 = i
		i = i + 1
		goto @L2
	label @L4:
		
		return 0
	func_end main
