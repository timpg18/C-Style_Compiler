label fac:
	func_begin fac
		$0 = n#block1 == 1
		if $0 goto @L0
		goto @L1
	label @L0:
		
		return 1
		goto @L2
	label @L1:
		$1 = n#block1 - 1
		param $1
		$2 = call fac, 1
		$3 = $2 * n#block1
		return $3
	label @L2:
	func_end fac
label main:
	func_begin main
		param 3
		$4 = call fac, 1
		ans#block4 = $4
		
		return ans#block4
		$0 = 4 * 4
		x#block1[$0] = 1
		$1 = 4 * 20
		$2 = $1 + 1
		$3 = $2 * 4
		y#block1[$3] = 10
		$4 = 5 * 10
		t#block1[$4] = p#block1
	func_end main
