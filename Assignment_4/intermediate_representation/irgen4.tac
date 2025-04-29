label main:
	func_begin main
		param 4
		$0 = call malloc, 1
		$1 = cast: VOID* -> INT* $0
		p#block1 = $1
		$2 = *p#block1
		$2 = 7
		
		return 0
	func_end main
