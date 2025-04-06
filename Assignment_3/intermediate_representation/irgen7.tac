label func_ellipses:
	func_prologue  func_ellipses
		$0 = a + b
		return $0
	func_epilogue  func_ellipses
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
		$1 = call func_ellipses, 11a = $1
		param 8
		param 2
		param 5
		$2 = call func_ellipses, 3
		a = $2
		$3 = &ap = $3
		$4 = *p
		$4 = 100
	func_epilogue  main
