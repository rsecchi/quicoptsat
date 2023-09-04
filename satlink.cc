#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/traffic-control-helper.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/bulk-send-application.h"
#include "ns3/config-store.h"
#include "ns3/tcp-socket-base.h"
#include "ns3/traffic-control-layer.h"
#include "ns3/fq-codel-queue-disc.h"
#include "ns3/packet.h"

#include "ns3/pcap-file.h"
#include "ns3/pcap-file-wrapper.h"

#include <fstream>
#include <memory>
#include <string>
#include <type_traits>


using namespace ns3;

uint32_t iw = 10;

void list_aggregates(Ptr<Object> p) {
	Object::AggregateIterator it = p->GetAggregateIterator();
	while(it.HasNext())
		std::cout << it.Next()->GetInstanceTypeId().GetName() << std::endl;
}


uint32_t num_flows = 1;
std::ofstream cwnd[10];
std::ofstream ssthresh[10];
std::ofstream rtt[10];
std::ofstream flightsize[10];
std::ofstream bttln_queue;

/* function callback to print the congestion window */
static void CwndTracer(std::string context, uint32_t oldval, uint32_t newval)
{
	uint32_t socket = int(context[43] - 48);
	cwnd[socket] << Simulator::Now().GetSeconds() << " " << oldval << std::endl;
	cwnd[socket] << Simulator::Now().GetSeconds() << " " << newval << std::endl;

}

static void RWNDTracer(std::string context, uint32_t oldval, uint32_t newval)
{
	uint32_t socket = int(context[43] - 48);
	ssthresh[socket] << Simulator::Now().GetSeconds() << " " << oldval << std::endl;
	ssthresh[socket] << Simulator::Now().GetSeconds() << " " << newval << std::endl;

}

/* function callback to print the RTT */
static void RttTracer(std::string context, Time oldval, Time newval)
{
	uint32_t socket = int(context[43] - 48);
	rtt[socket] << Simulator::Now().GetSeconds() << " " << oldval.GetSeconds() << std::endl;
	rtt[socket] << Simulator::Now().GetSeconds() << " " << newval.GetSeconds() << std::endl;
}

static void FlightSizeTracer(std::string context, uint32_t  oldval, uint32_t newval)
{
	uint32_t socket = int(context[43] - 48);
	flightsize[socket] << Simulator::Now().GetSeconds() << " " << oldval << std::endl;
	flightsize[socket] << Simulator::Now().GetSeconds() << " " << newval << std::endl;
}

/* function callback when a transfer is completed*/
void Completed(Ptr<Socket> sock)
{
	Ptr<TcpSocketBase> tcp = sock->GetObject<TcpSocketBase>();
	Address address;
	tcp->GetPeerName(address);
	InetSocketAddress transport = InetSocketAddress::ConvertFrom(address);	

	std::cout << transport.GetPort()-9 << " ";
	std::cout << Simulator::Now().GetSeconds() << std::endl;

}

void ChangeIW() {
	Config::SetDefault("ns3::TcpSocket::InitialCwnd", UintegerValue(iw));
}

void SocketTraces(uint32_t i, Ptr<BulkSendApplication> bulk) {

	std::string name = 
		"/NodeList/0/$ns3::TcpL4Protocol/SocketList/" +	std::to_string(i);

	Config::Connect(name + "/CongestionWindow", MakeCallback(&CwndTracer));
	Config::Connect(name + "/RTT", MakeCallback(&RttTracer));
	Config::Connect(name + "/SlowStartThreshold", MakeCallback(&RWNDTracer));
	Config::Connect(name + "/BytesInFlight", MakeCallback(&FlightSizeTracer));

	/* access to the socket */
	Ptr<Socket> socket = bulk->GetSocket();
	
	socket->SetCloseCallbacks(MakeCallback(&Completed), 
				MakeNullCallback<void, Ptr<Socket>>());
}

void traceenqueue(Ptr<const QueueDisc> qd)
{
	bttln_queue << Simulator::Now().GetSeconds() <<  " " ;
	bttln_queue << qd->GetNPackets() << std::endl;
	Simulator::Schedule(MilliSeconds(1), &traceenqueue, qd);
}

int main(int argc, char* argv[])
{

	uint32_t seed = 1;
	uint32_t run = 0;
	double error_p = 0.0;
	double duration = 100.0;
	double offset = 0.001;
    double ssthresh_reset = 0.0;
	uint64_t data_kbytes = 1000;
	bool boolp = false;
	bool boolp_iw = false;
    bool boolpg = false;

	std::string transport_prot = "TcpCubic";
	std::string bandwidth = "5Mbps";
	std::string delay = "45ms";
	std::string last_rtt = "10ms";
	std::string last_cwnd = "10";
	std::string qlen = "100p";
	std::string mtu = "1440";
	std::string pacing = "0";
	std::string pacing_iw = "0";
    std::string prog_growth = "0";
	std::string hs = "0";
	std::string hspp = "0";
	std::string ss_pacing_ratio = "200";
    std::string max_pacing_rate = "1Gbps";
	std::string recovery = "ns3::TcpClassicRecovery";

	CommandLine cmd(__FILE__);
	cmd.AddValue("num_flows", "Number of flows", num_flows);
	cmd.AddValue("bandwidth", "Bottleneck bandwidth", bandwidth);
	cmd.AddValue("delay", "Bottleneck delay (ms)", delay);
	cmd.AddValue("transport_prot", "Transport protocol", transport_prot);
	cmd.AddValue("qlen", "Queue Length", qlen);
	cmd.AddValue("mtu", "Packet size", mtu);
	cmd.AddValue("error_p", "Packet error rate", error_p);
	cmd.AddValue("duration", "duration", duration);
	cmd.AddValue("data", "Number of kBs to send", data_kbytes);
	cmd.AddValue("seed", "Random numbers seed", seed);
	cmd.AddValue("offset", "Spacing between connections", offset);
	cmd.AddValue("iw", "Initial congestion window", iw);
	cmd.AddValue("pacing", "Enable Pacing (0/1)", pacing);
	cmd.AddValue("pacing_iw", "Enable Pacing for IW (0/1)", pacing_iw);
	cmd.AddValue("ss_pacing_ratio", "pacing perc. increase in SS", ss_pacing_ratio);
	cmd.AddValue("max_pacing_rate", "Maximum Pacing Rate", max_pacing_rate);
	cmd.AddValue("hystartpp", "Enable Hystart++", hspp);
	cmd.AddValue("hystart", "Enable Hystart++", hs);
	cmd.AddValue("last_rtt", "Careful Resume stored RTT", last_rtt);
	cmd.AddValue("last_cwnd", "Careful Resume stored cwnd", last_cwnd);
    cmd.AddValue("ssthresh_reset", "Multiplier for ssthresh", ssthresh_reset);
    cmd.AddValue("prog_growth", "Progressive growth in Reiconassance", prog_growth);
	cmd.AddValue("recovery", "Define the Safe Retreat Recovery algorithm", recovery);
	cmd.Parse(argc, argv);

	if (pacing == "1")
		boolp = true;

	if (pacing_iw == "1")
		boolp_iw = true;

	if (prog_growth == "1")
		boolpg = true;

	if (hspp == "1")
	{
		Config::SetDefault("ns3::TcpCubic::HyStartpp", BooleanValue(true));
		Config::SetDefault("ns3::TcpCubic::HyStartDetect", IntegerValue(2));	
	}

	if (hs == "0")
		Config::SetDefault("ns3::TcpCubic::HyStart", BooleanValue(false));

	/* config */
	/* pacing */
	Config::SetDefault("ns3::TcpSocketState::EnablePacing", BooleanValue(boolp));
	Config::SetDefault("ns3::TcpSocketState::PaceInitialWindow", BooleanValue(boolp_iw));
	Config::SetDefault("ns3::TcpSocketState::PacingSsRatio", UintegerValue(stoi(ss_pacing_ratio)));
	Config::SetDefault("ns3::TcpSocketState::MaxPacingRate", StringValue(max_pacing_rate));
	
	// Config::SetDefault("ns3::TcpSocket::InitialCwnd", UintegerValue(iw));
	Config::SetDefault("ns3::TcpSocket::RcvBufSize", UintegerValue(327680000));
	Config::SetDefault("ns3::TcpSocket::SndBufSize", UintegerValue(327688000));
	Config::SetDefault("ns3::TcpSocketBase::MaxWindowSize", UintegerValue(65534));
	Config::SetDefault("ns3::TcpSocketBase::WindowScaling", BooleanValue(true));
	Config::SetDefault("ns3::FqCoDelQueueDisc::MaxSize", StringValue(qlen) );

	transport_prot = "ns3::" + transport_prot;
	Config::SetDefault("ns3::TcpL4Protocol::SocketType",
		TypeIdValue(TypeId::LookupByName(transport_prot)));
	Config::SetDefault("ns3::TcpL4Protocol::RecoveryType",
		TypeIdValue(TypeId::LookupByName(recovery)));

	Config::SetDefault("ns3::TcpSocket::SegmentSize", UintegerValue(std::stoi(mtu)));

	Config::SetDefault("ns3::TcpCubicCr::LastRtt", StringValue(last_rtt));
	Config::SetDefault("ns3::TcpCubicCr::LastWindow", UintegerValue(std::stoi(last_cwnd)));
    Config::SetDefault("ns3::TcpCubicCr::ssthreshReset", DoubleValue(ssthresh_reset));
	Config::SetDefault("ns3::TcpCubicCr::ProgressiveGrowth", BooleanValue(boolpg));

    //PcapFileWrapper::Enable("output.pcap", std::ios::out, PcapFile::DLT_EN10MB);



	SeedManager::SetSeed(seed);
	SeedManager::SetRun(run);

	/* create nodes */
	NodeContainer nodes;
	nodes.Create(2);
	
	/* Configure the error model */
	Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable>();
	uv->SetStream(50);
	RateErrorModel error_model;
	error_model.SetRandomVariable(uv);
	error_model.SetUnit(RateErrorModel::ERROR_UNIT_PACKET);
	error_model.SetRate(error_p);

	/* create link */
	PointToPointHelper pp;
	pp.SetDeviceAttribute("DataRate", StringValue(bandwidth));
	pp.SetChannelAttribute("Delay", StringValue(delay));
	pp.SetQueue("ns3::DropTailQueue", "MaxSize", StringValue(qlen));
	pp.SetDeviceAttribute("ReceiveErrorModel", PointerValue(&error_model));
	NetDeviceContainer devs = pp.Install(nodes);

	/* Create Internet stack (TCP/IP and assign addresses */
	InternetStackHelper stack;
	stack.Install(nodes);
	Ipv4AddressHelper address;
	address.SetBase("10.1.1.0", "255.255.255.0");
	Ipv4InterfaceContainer ifv4 = address.Assign(devs);
	pp.EnablePcapAll("tcpsat");


	//list_aggregates(nodes.Get(0));
	bttln_queue.open("bottln_queue.tr", std::ios::out);
	Ptr<TrafficControlLayer> tc = nodes.Get(0)->GetObject<TrafficControlLayer>();
	Ptr<QueueDisc> qdisc = tc->GetRootQueueDiscOnDevice(devs.Get(0));
	Simulator::Schedule(Seconds(0), &traceenqueue, qdisc);
	
	// Connect to a QueueDisc "Drop" tracer
	//Config::ConnectWithoutContext(
	//	"/NodeList/0/$ns3::TrafficControlLayer/RootQueueDiscList/0/Drop",
	//	Make(&traceenqueue));

	/* create sources */	
	ApplicationContainer apps;
	for(uint32_t i=0; i<num_flows; i++) {
		InetSocketAddress daddr = InetSocketAddress(ifv4.GetAddress(1), i+9);
		BulkSendHelper src("ns3::TcpSocketFactory", daddr);
		src.SetAttribute("MaxBytes", UintegerValue(data_kbytes*1000));
		apps = src.Install(nodes.Get(0));
		apps.Start(Seconds(i*offset));
		cwnd[i].open("cwnd" + std::to_string(i) + ".tr", std::ios::out);
		rtt[i].open("rtt" + std::to_string(i) + ".tr", std::ios::out);
		flightsize[i].open("flightsize" + std::to_string(i) + ".tr", std::ios::out);
		ssthresh[i].open("ssthresh" + std::to_string(i) + ".tr", std::ios::out);
	
		/* get reference to bulk application */
		Ptr<BulkSendApplication> bulk;
		bulk = apps.Get(0)->GetObject<BulkSendApplication>();

		Simulator::Schedule(Seconds(i*offset+0.001), &SocketTraces, i, bulk);

	}

	Simulator::Schedule(Seconds(offset-0.001), &ChangeIW);

	/* create sinks */
	ApplicationContainer sinks;
	for(uint32_t i=0; i<num_flows; i++) {
		InetSocketAddress saddr = InetSocketAddress(Ipv4Address::GetAny(), i+9);
		PacketSinkHelper sink("ns3::TcpSocketFactory", saddr);
		sinks = sink.Install(nodes.Get(1));
		sinks.Start(Seconds(0));
	}

	/* start simulation */
	//FlowMonitorHelper fmon;
	//fmon.InstallAll();
	Simulator::Stop(Seconds(duration));
	Simulator::Run();

	// fmon.SerializeToXmlFile("tcpsat.flowmonitor", true, true);
	Simulator::Destroy();

	return 0;
}

