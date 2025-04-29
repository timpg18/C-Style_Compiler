label main:
	func_begin main
		x#block1 = 5
		param "%d \n"
		param x#block1
		$0 = call printf, 2
	func_end main
