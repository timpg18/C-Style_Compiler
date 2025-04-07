label func:
	func_prologue  func
	func_epilogue  func
label main:
	func_prologue  main
		a = 1
		b = 12
		if a == 0 goto @L2
		goto @L0
	label @L0:
		if b == 0 goto @L2
		goto @L1
	label @L1:
		c = 1
		goto @L3
	label @L2:
		c = 0
	label @L3:
		$0 = a & b
		c = $0
		$1 = a + b
		c = $1
		c = c + a
		c = c - b
		$2 = c
		c = c + 1
		$3 = a - 1
		$4 = a >> b
		c = $4
		$5 = a << b
		c = $5
		c = 4
		$6 = c * bk = $6
		c = c ^ b
		c = c % k
	label lab:
		$7 = c > b
		if $7 goto @L7
		goto @L8
	label @L7:
		c1 = 0
	label for_begin1:
		$8 = c1 < 100
		if $8 goto @L4
		goto for_end1
	label @L4:
		$10 = c1
		c1 = c1 + 1
		goto for_begin1
		gg = 10
		$9 = c1
		c1 = c1 + 1
		goto for_begin1
	label for_end1:
		goto @tmp14
	label @L8:
	label while_begin2:
		$11 = c < 1000
		if $11 goto @L5
		goto while_end2
	label @L5:
		$12 = c
		c = c + 1
		$13 = a
		a = a + 1
		goto label while_end2:
		goto while_begin2
	label while_end2:
	label do_while_begin3:
		$14 = c
		c = c - 1
		$15 = b
		b = b - 1
	label @L6:
		$16 = c > 0
		if $16 goto do_while_begin3
		goto do_while_end3
	label do_while_end3:
		goto lab
		if c == 1 goto @L9
		if c == 2 goto @L11
		goto @L12
	label @L9:
		$17 = c
		c = c + 1
	label @L10:
		goto @L13
	label @L11:
		c = c - 10
		goto @L13
	label @L12:
		$18 = c
		c = c + 1
		goto @L13
	label @L13:
		$19 = 0 * 100
		$20 = $19 + 1
		$21 = $20 * 10
		$22 = $21 + 2
		$23 = $22 * 12
		$24 = $23 + 1
		$25 = $24 * 4
		arr[$25] = 3
		$26 = 10 * 2
		$27 = $26 + 12
		$28 = $27 * 3
		$29 = $28 + 12
		$30 = $29 * 4
		$31 = $30 + 32
		$32 = $31 * 4
		brr[$32] = 10
		x = 10
		$33 = x
		x = x + 1
		g = 122
		$34 = g
		g = g - 1
		$35 = &xkk = $35
	func_epilogue  main
