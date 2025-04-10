glb1 = 10
label bar:
func_begin bar
lcl = 10
label while_begin1:
if b == 0 goto while_end1
goto @L0
label @L0:
$0 = b
b = b - 1
goto while_begin1
label while_end1:
$1 = a == 0
if $1 goto @L1
goto @L2
label @L1:

return 0
label @L2:
$2 = a - 1
param $2
$3 = a + lcl
param $3
$4 = call bar, 2
func_end bar
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
