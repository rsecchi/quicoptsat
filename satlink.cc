#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/traffic-control-helper.h"
#include "ns3/flow-monitor-helper.h"
#include <fstream>
#include <string>


using namespace ns3;

uint32_t num_flows = 1;
std::ofstream cwnd[10];

static void CwndTracer(std::string context, uint32_t oldval, uint32_t newval)
{

	uint32_t socket = int(context[43] - 48);
	cwnd[socket] << Simulator::Now().GetSeconds() << " " << newval << std::endl;

}

void ConnectSocketTraces(uint32_t i) {

	std::string name = 
		"/NodeList/0/$ns3::TcpL4Protocol/SocketList/" +	std::to_string(i)
		+ "/CongestionWindow";
		std::cout << name << std::endl;

		Config::Connect(name, MakeCallback(&CwndTracer));	
}

int main(int argc, char* argv[])
{

	uint32_t run = 0;
	double error_p = 0.0;
	double duration = 100.0;
	double offset = 0.001;
	uint64_t data_mbytes = 0;
	std::string transport_prot = "TcpCubic";
	std::string bandwidth = "5Mbps";
	std::string delay = "45ms";
	std::string qlen = "100p";
	std::string mtu = "1500";

	CommandLine cmd(__FILE__);
	cmd.AddValue("num_flows", "Number of flows", num_flows);
	cmd.AddValue("bandwidth", "Bottleneck bandwidth", bandwidth);
	cmd.AddValue("delay", "Bottleneck delay (ms)", delay);
	cmd.AddValue("transport_prot", "Transport protocol", transport_prot);
	cmd.AddValue("qlen", "Queue Length", qlen);
	cmd.AddValue("mtu", "Packet size", mtu);
	cmd.AddValue("error_p", "Packet error rate", error_p);
	cmd.AddValue("duration", "duration", duration);
	cmd.AddValue("data", "Number of MBs to send", data_mbytes);
	cmd.AddValue("offset", "Spacing between connections", offset);
	cmd.Parse(argc, argv);

	/* config */
	printf("num_flows=%d\n", num_flows);
	transport_prot = "ns3::" + transport_prot;
	Config::SetDefault("ns3::TcpL4Protocol::SocketType",
		TypeIdValue(TypeId::LookupByName(transport_prot)));
	Config::SetDefault("ns3::TcpSocket::SegmentSize", UintegerValue(std::stoi(mtu)));

	SeedManager::SetSeed(1);
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

	
	/* assign addresses */
	InternetStackHelper stack;
	stack.Install(nodes);
	Ipv4AddressHelper address;
	address.SetBase("10.1.1.0", "255.255.255.0");
	Ipv4InterfaceContainer ifv4 = address.Assign(devs);
	pp.EnablePcapAll("tcpsat");

	/* create sources */	
	ApplicationContainer apps;
	for(uint32_t i=0; i<num_flows; i++) {
		InetSocketAddress daddr = InetSocketAddress(ifv4.GetAddress(1), i+9);
		BulkSendHelper src("ns3::TcpSocketFactory", daddr);
		src.SetAttribute("MaxBytes", UintegerValue(data_mbytes*1000000));
		apps = src.Install(nodes.Get(0));
		apps.Start(Seconds(i*offset));
		apps.Stop(Seconds(duration));
		cwnd[i].open("cwnd" + std::to_string(i) + ".tr", std::ios::out);
		Simulator::Schedule(Seconds(i*offset+0.001), &ConnectSocketTraces, i);
	}

	/* create sinks */
	ApplicationContainer sinks;
	for(uint32_t i=0; i<num_flows; i++) {
		InetSocketAddress saddr = InetSocketAddress(Ipv4Address::GetAny(), i+9);
		PacketSinkHelper sink("ns3::TcpSocketFactory", saddr);
		sinks = sink.Install(nodes.Get(1));
		sinks.Start(Seconds(0));
		sinks.Stop(Seconds(duration));
	}


	/* start simulation */
	FlowMonitorHelper fmon;
	fmon.InstallAll();
	Simulator::Stop(Seconds(duration));
	Simulator::Run();

	fmon.SerializeToXmlFile("tcpsat.flowmonitor", true, true);
	Simulator::Destroy();

	return 0;
}

