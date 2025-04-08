label main:
func_begin main
x = 2
y = 1
if x == 1 goto @L0
if x == 2 goto @L5
if x == 3 goto @L6
goto @L7
label @L0:
param "Case 1 in outer switch\n"
$0 = call printf, 1
if y == 0 goto @L1
if y == 1 goto @L2
goto @L3
label @L1:
param "Case 0 in inner switch\n"
$1 = call printf, 1
goto @L4
label @L2:
param "Case 1 in inner switch\n"
$2 = call printf, 1
goto @L4
label @L3:
param "Default in inner switch\n"
$3 = call printf, 1
goto @L4
label @L4:
goto @L8
label @L5:
param "Case 2 in outer switch\n"
$4 = call printf, 1
goto @L8
label @L6:
param "Case 3 in outer switch\n"
$5 = call printf, 1
goto @L8
label @L7:
param "Default in outer switch\n"
$6 = call printf, 1
label @L8:

return 0
func_end main
