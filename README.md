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


## Example of script to run CR

The cr_test1 directory contains an example setup, including the script cr_test1/run_single.sh.
This script loops over a specified range and runs NS-3 simulations for the chosen network scenario and TCP variant.
It automates comparisons of TCP congestion control variants (Cubic and CubicCr) under different initial
congestion windows (IW) and network conditions, including LEO and GEO satellite links.

It requires NS-3 version 3.39 to be installed and the simulation script located at scratch/satlink.cc within the NS-3.39 directory.

**Usage:**

1. Configure paths

   Edit the script to set your NS-3 installation paths:
   ```bash
   NS3="/path/to/ns-3/ns3"
   NS3_DIR="/path/to/ns-3/"
   SCRIPT="scratch/satlink.cc"
   ```

2. Set simulation parameters

   PARAMS_LEO / PARAMS_GEO – Network parameters for LEO or GEO satellite links.
   TEST – TCP variant and Careful Resume options:
   ```code
   --transport_prot=TcpCubicCr   # TCP variant
   --iw=10                       # Initial congestion window
   --last_cwnd=200               # Previous congestion window (for CR)
   --last_rtt=50ms               # Previous RTT (for CR)
   --ssthresh_reset=1.0          # Slow-start threshold reset
   ```

   RANGE – Number of iterations or data values to simulate.

3. Make the script  executable and runit:

   ```bash
   chmod +x run_single.sh
   ./run_single.sh
   ```

4. Example trace outputs
   - leo_cubic_iw10.tr – LEO scenario, TCP Cubic, IW=10.
   - leo_cubic_iw200.tr – LEO scenario, TCP Cubic, IW=200.
   - geo_cr_cwnd900.tr – GEO scenario, TCP CubicCr with previous cwnd=900.
   - geo_cubic_iw10.tr – GEO scenario, TCP Cubic, IW=10.
   - geo_cubic_iw900.tr – GEO scenario, TCP Cubic, IW=900.


