label main:
	func_begin main
		param "a+b"
		$0 = call printf, 1
		
		return 0
	func_end main
