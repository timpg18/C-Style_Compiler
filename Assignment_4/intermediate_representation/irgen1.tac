label main:
	func_begin main
		a#block1 = 99
		$0 = a#block1 * 9
		a#block1 = $0
		
		return a#block1
	func_end main
