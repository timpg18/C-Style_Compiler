label fac:
	func_begin fac
		$0 = n#block1 == 1
		if $0 goto @L0
		goto @L1
	label @L0:
		
		return 1
	label @L1:
		$1 = n#block1 - 1
		param $1
		$2 = call fac, 1
		$3 = n#block1 * $2
		return $3
	func_end fac
label main:
	func_begin main
		num#block3 = 5
		param num#block3
		$4 = call fac, 1
		sum#block3 = $4
		param "factorial of %d is %d \n"
		param num#block3
		param sum#block3
		$5 = call printf, 3
		num#block3 = 9
		param num#block3
		$7 = call fac, 1
		sum#block3 = $7
		param "factorial of %d is %d \n"
		param num#block3
		param sum#block3
		$9 = call printf, 3
		
		return 0
	func_end main
