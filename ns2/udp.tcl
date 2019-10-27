set ns [new Simulator]
set nf [open out.nam w]
$ns namtrace-all $nf

proc finish {} {
	
	global ns nf
	$ns flush-trace
	close $nf
	exec nam out.nam &
	exit()
}

set n0 [$ns node]
set n1 [$ns node]

$ns duplex-link $n0 $n1 1Mb 10ms DropTail
set udp0 [new Agent/UDP]	

$ns attach-agent $n0 $udp0
set cbr [new Application/Traffic/CBR]
$cbr set packetSize 512
$cbr set interval 0.005
$cbr attach-agent $udp0

set null0 [new Agent/Null]
$ns attach-agent $n1 $null0

$ns connect $udp0 $null0

$ns at 0.5 "$cbr start"
$ns at 4.5 "$cbr stop"

$ns at 5.0 "finish"
$ns run
