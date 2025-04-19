label main:
	func_begin main
		sum#block1 = 0
		i#block2 = 1
	label @L0:
		$0 = i#block2 <= 10
		if $0 goto @L1
		goto @L2
	label @L1:
		$2 = sum#block1 + i#block2
		sum#block1 = $2
		$1 = i#block2 + 1
		i#block2 = $1
		goto @L0
	label @L2:
		
		return sum#block1
	func_end main
