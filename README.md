# Quick Opt Satellite Materials

- **`tcp.*cc`**: These files represent the TCP variants used to integrate Hystart++ into TCP.  
  - Initially tested with **TCP NewReno**  
  - Later implemented with **TCP CUBIC**
  - This should be applied against ns-3.37

- We modified the TCP implementation in **NS-3.39** to include CR, and a corresponding **patch file** is available.
- The patch ns3.39_CR should be applied againg ns-3.39



## Program Options
- **`satlink.cc`**: This file contains the NS-3 script required to run the Quick Opt satellite simulation with the option below.

```text
--transport_prot:    Transport protocol to use: TcpNewReno, TcpLinuxReno,
  TcpHybla, TcpHighSpeed, TcpHtcp, TcpVegas, TcpScalable, TcpVeno, TcpBic,
  TcpYeah, TcpIllinois, TcpWestwood, TcpWestwoodPlus, TcpLedbat, TcpLp,
  TcpDctcp, TcpCubic, TcpBbr [TcpWestwood]
--error_p:           Packet error rate [0]
--bandwidth:         Bottleneck bandwidth [2Mbps]
--delay:             Bottleneck delay [0.01ms]
--access_bandwidth:  Access link bandwidth [10Mbps]
--access_delay:      Access link delay [45ms]
--tracing:           Flag to enable/disable tracing [false]
--prefix_name:       Prefix of output trace file [TcpVariantsComparison]
--data:              Number of Megabytes of data to transmit [0]
--mtu:               Size of IP packets to send in bytes [400]
--num_flows:         Number of flows [1]
--duration:          Time to allow flows to run in seconds [100]
--run:               Run index (for setting repeatable seeds) [0]
--flow_monitor:      Enable flow monitor [false]
--pcap_tracing:      Enable or disable PCAP tracing [false]
--queue_disc_type:   Queue disc type for gateway (e.g. ns3::CoDelQueueDisc) [ns3::PfifoFastQueueDisc]
--sack:              Enable or disable SACK option [true]
--recovery:          Recovery algorithm type to use (e.g., ns3::TcpPrrRecovery) [ns3::TcpClassicRecovery]
```

- The following options are available for CR in the CR patch:
  
```text
--transport_prot=TcpCubicCr     Specifies the transport protocol,
                                in this case TCP with the Careful Resume mechanism.
--iw=10                         Sets the initial congestion window to 10 segments.
--last_cwnd=200                 Provides the previous congestion window value (eg 200 packets),
                                allowing CR to resume transmission with knowledge of the last session’s rate.
--last_rtt=50ms                 Provides the previous round-trip time (eg 50ms),
                                used to validate that the path has not significantly
                                changed before resuming at a higher rate.
--ssthresh_reset=1.0            Configures the slow-start threshold reset behaviour,
                                allowing the threshold to be fully restored
                                (1.0× previous value) when resuming.
```
