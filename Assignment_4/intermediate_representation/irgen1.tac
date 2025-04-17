label main:
	func_begin main
		param 4
		$0 = call malloc, 1
		$1 = cast: class A* $0
		ptr#block2 = $1
	func_end main
