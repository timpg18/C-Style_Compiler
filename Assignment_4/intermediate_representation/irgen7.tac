label main:
	func_begin main
		param "the number of arguments: %d\n"
		param argv#block1
		$0 = call printf, 2
		$1 = argc#block1 + 8
		param "argument 1:%d \n"
		param $1
		$2 = call printf, 2
		
		return 0
	func_end main
