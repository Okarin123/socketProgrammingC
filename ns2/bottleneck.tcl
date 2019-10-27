set ns [new Simulator]
set nf [open out.nam w]
set tf [open out.tf w]

$ns namtrace-all $nf
$ns trace-all $tf

proc finish {} {
	global ns nf tf
	$ns flush-trace
	close $nf
	close $tf
	exec nam out.nam &
	exit 0
}

$ns color 1 Blue
$ns color 2 Red

set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]

$ns duplex-link $n0 $n2 2Mb 10ms DropTail
$ns duplex-link $n1 $n2 2Mb 10ms DropTail
$ns simplex-link $n2 $n3 0.3Mb 10ms DropTail
$ns queue-limit $n2 $n3 5
$ns simplex-link $n3 $n2 0.3Mb 10ms DropTail
$ns duplex-link $n3 $n4 0.5Mb 10ms DropTail
$ns duplex-link $n3 $n5 0.5Mb 10ms DropTail

#Setting up a TCP connection 
set tcp [new Agent/TCP/Newreno]
set sink [new Agent/TCPSink/DelAck]
set ftp [new Application/FTP]

$ns attach-agent $n1 $tcp
$ns attach-agent $n5 $sink
$ns connect $tcp $sink
$tcp set packetSize 512
$ftp attach-agent $tcp 

#Setting up a UDP Connection 
set udp [new Agent/UDP]
set null0 [new Agent/Null]
set cbr [new Application/Traffic/CBR]
$cbr set packetSize 512

$ns attach-agent $n0 $udp
$ns attach-agent $n4 $null0
$cbr attach-agent $udp
$ns connect $udp $null0

#Start
$ns at 1.0 "$cbr start"
$ns at 2.0 "$cbr stop"
$ns at 0.5 "$ftp start"
$ns at 2.5 "$ftp stop"

$ns at 5.0 "finish"
$ns run 
