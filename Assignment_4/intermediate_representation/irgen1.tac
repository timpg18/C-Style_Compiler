label main:
	func_begin main
		a#block1 = 99
		$0 = a#block1 < 9
		if $0 goto @L1
		goto @L0
	label @L0:
		$1 = a#block1 > 0
		if $1 goto @L1
		goto @L2
	label @L1:
		$2 = a#block1
		a#block1 = a#block1 + 1
		goto @L3
	label @L2:
		$3 = a#block1
		a#block1 = a#block1 - 1
	label @L3:
	func_end main
