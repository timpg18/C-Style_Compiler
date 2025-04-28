label main:
	func_begin main
		gg#block1 = 1
		param "%d "
		param gg#block1
		$0 = call printf, 2
	func_end main
