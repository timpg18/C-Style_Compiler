label main:
	func_begin main
		$0 = 1.0 + 1.2
		$1 = $0 + 1.4
		b#block1 = $1
		a#block1 = b#block1
		
		return 0
	func_end main
