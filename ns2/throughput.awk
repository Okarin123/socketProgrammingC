BEGIN {
       recvdSize = 0
       startTime = 1e6
       stopTime = 0
  }
   
  {
       
       # Trace line format: new
       if ($2 == "-t") {
             event = $1
             time = $3
             node_id = $5
             flow_id = $39
             pkt_id = $41
             pkt_size = $37
             flow_t = $45
             level = $19
       }
   
  # Store start time
  if (level == "AGT" && (event == "+" || event == "s") && pkt_size >= 512) {
    if (time < startTime) {
             startTime = time
             }
       }
   
  # Update total received packets' size and store packets arrival time
  if (level == "AGT" && event == "r" && pkt_size >= 512) {
       if (time > stopTime) {
             stopTime = time
             }
       # Rip off the header
       hdr_size = pkt_size % 512
       pkt_size -= hdr_size
       # Store received packet's size
       recvdSize += pkt_size
       }
  }
   
  END {
       printf("Average Throughput[kbps] = %.2f\t\t StartTime=%.2f\tStopTime=%.2f\n",(recvdSize/(stopTime-startTime))*(8/1000),startTime,stopTime)
  }
