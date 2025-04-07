label main:
	func_prologue  main
		i = 0
		j = 0
		i = 0
	label for_begin1:
		$0 = i < 5
		if $0 goto @L3
		goto for_end1
	label @L3:
		$2 = i == 2
		if $2 goto @L0
		goto @L2
	label @L0:
		goto for_begin1
		$3 = i == 4
		if $3 goto @L1
		goto @L2
	label @L1:
		goto for_end1
	label @L2:
		param "For with break/continue: i = %d\n"
		param i
		$4 = call printf, 2
		$1 = i
		i = i + 1
		goto for_begin1
	label for_end1:
		
		return 0
	func_epilogue  main
