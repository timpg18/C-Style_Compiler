label func_ellipses:
	func_prologue  func_ellipses
		$0 = a + b
		return $0
	func_epilogue  func_ellipses
label add:
	func_prologue  add
		$1 = &k
		return $1
	func_epilogue  add
label main:
	func_prologue  main
		param 7
		param 2
		param 3
		param 5
		param 6
		param 5
		param 4
		param 3
		param 3
		param 8
		param 3
		$2 = call func_ellipses, 11a = $2
		param 8
		param 2
		param 5
		$3 = call func_ellipses, 3
		a = $3
	label func:
	label f2:
		$6 = &ap2 = $6
		$7 = *p2
		$7 = 100
	func_epilogue  main
