label f:
	func_begin f
		
		return 0
	func_end f
label main:
	func_begin main
		param 1
		param 2
		param 4
		param 9.0
		param 9.8
		param 4
		$0 = call f, 6
		b#block2 = $0
	func_end main
