label main:
	func_begin main
		a = 0
		$0 = 8 * 9
		a#block1 = $0
		z = 12	
		$1 = z#block1 * 9
		a#block1 = $1
		
		return 0
	func_end main
