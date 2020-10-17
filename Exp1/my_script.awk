BEGIN {FS=","
	print "My items list for today"}
$3>300 {print $2}
/stationary/ {print}
END {print "**********************************"}
