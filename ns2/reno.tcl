set ns [new Simulator]
set nf [open out.nam w]
$ns namtrace-all $nf

#Define finish 
proc finish {} {
	global ns nf
	$ns flush-trace
	close $nf
	exec nam out.nam &
	exit 0
}

#declare nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]

#estabilish connections 
$ns duplex-link $n0 $n1 1Mb 10ms DropTail
$ns duplex-link $n1 $n2 1Mb 10ms DropTail
$ns duplex-link $n1 $n3 1Mb 10ms DropTail
$ns duplex-link $n2 $n3 1Mb 10ms DropTail

#Setting TCP Connections 
set tcp [new Agent/TCP]
set sink [new Agent/TCPSink]
set ftp [new Application/FTP]

$ns rtproto LS

$ns attach-agent $n0 $tcp
$ns attach-agent $n2 $sink
$ns connect $tcp $sink
$ftp attach-agent $tcp

#Downing the link
$ns rtmodel-at 0.8 down $n1 $n2
$ns rtmodel-at 1.2 up $n1 $n2

#Send data
$ns at 0.2 "$ftp start"
$ns at 1.5 "$ftp stop"

$ns at 1.6 "finish"
$ns run
