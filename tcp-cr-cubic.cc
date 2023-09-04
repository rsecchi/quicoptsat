/*
 * Copyright (c) 2014 Natale Patriciello <natale.patriciello@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 */

#define NS_LOG_APPEND_CONTEXT                                                                      \
    {                                                                                              \
        std::clog << Simulator::Now().GetSeconds() << " ";                                         \
    }

#include "tcp-cr-cubic.h"

#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE("TcpCubicCr");

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED(TcpCubicCr);

TypeId
TcpCubicCr::GetTypeId()
{
    static TypeId tid =
        TypeId("ns3::TcpCubicCr")
            .SetParent<TcpSocketBase>()
            .AddConstructor<TcpCubicCr>()
            .SetGroupName("Internet")
            .AddAttribute("FastConvergence",
                          "Enable (true) or disable (false) fast convergence",
                          BooleanValue(true),
                          MakeBooleanAccessor(&TcpCubicCr::m_fastConvergence),
                          MakeBooleanChecker())
            .AddAttribute("Beta",
                          "Beta for multiplicative decrease",
                          DoubleValue(0.7),
                          MakeDoubleAccessor(&TcpCubicCr::m_beta),
                          MakeDoubleChecker<double>(0.0))
            .AddAttribute("HyStart",
                          "Enable (true) or disable (false) hybrid slow start algorithm",
                          BooleanValue(true),
                          MakeBooleanAccessor(&TcpCubicCr::m_hystart),
                          MakeBooleanChecker())
            .AddAttribute("HyStartpp",
                          "Enable (true) or disable (false) CSS in Hystart",
                          BooleanValue(false),
                          MakeBooleanAccessor(&TcpCubicCr::m_hystartpp),
                          MakeBooleanChecker())
            .AddAttribute("css_growth_divisor",
                          "Exponential growth factor CSS in Hystart",
                          UintegerValue(8),
                          MakeUintegerAccessor(&TcpCubicCr::m_css_growth_divisor),
                          MakeUintegerChecker<uint8_t>())
            .AddAttribute("css_max_rounds",
                          "Maximum number of probing rounds in CSS",
                          UintegerValue(5),
                          MakeUintegerAccessor(&TcpCubicCr::m_css_max_rounds),
                          MakeUintegerChecker<uint8_t>())
            .AddAttribute("HyStartLowWindow",
                          "Lower bound cWnd for hybrid slow start (segments)",
                          UintegerValue(16),
                          MakeUintegerAccessor(&TcpCubicCr::m_hystartLowWindow),
                          MakeUintegerChecker<uint32_t>())
			.AddAttribute("LastWindow",
						  "Last cwnd at start of unvalidated period",
						  UintegerValue(16),
						  MakeUintegerAccessor(&TcpCubicCr::m_lastWindow),
						  MakeUintegerChecker<uint32_t>())
            .AddAttribute("LastRtt",
                          "Spacing between ack's indicating train",
                          TimeValue(MilliSeconds(2)),
                          MakeTimeAccessor(&TcpCubicCr::m_lastRtt),
                          MakeTimeChecker())
            .AddAttribute("ssthreshReset",
                          "Multiplier for ssthresh after Unvalidated",
                          DoubleValue(0.0),
                          MakeDoubleAccessor(&TcpCubicCr::m_ssthreshReset),
                          MakeDoubleChecker<double>(0.0))
            .AddAttribute("ProgressiveGrowth",
                          "Enable (true) or disable (false) Progressive Growth",
                          BooleanValue(false),
                          MakeBooleanAccessor(&TcpCubicCr::m_progGrowth),
                          MakeBooleanChecker())
            .AddAttribute("HyStartDetect",
                          "Hybrid Slow Start detection mechanisms:"
                          "1: packet train, 2: delay, 3: both",
                          IntegerValue(3),
                          MakeIntegerAccessor(&TcpCubicCr::m_hystartDetect),
                          MakeIntegerChecker<int>(1, 3))
            .AddAttribute("HyStartMinSamples",
                          "Number of delay samples for detecting the increase of delay",
                          UintegerValue(8),
                          MakeUintegerAccessor(&TcpCubicCr::m_hystartMinSamples),
                          MakeUintegerChecker<uint8_t>())
            .AddAttribute("HyStartAckDelta",
                          "Spacing between ack's indicating train",
                          TimeValue(MilliSeconds(2)),
                          MakeTimeAccessor(&TcpCubicCr::m_hystartAckDelta),
                          MakeTimeChecker())
            .AddAttribute("HyStartDelayMin",
                          "Minimum time for hystart algorithm",
                          TimeValue(MilliSeconds(4)),
                          MakeTimeAccessor(&TcpCubicCr::m_hystartDelayMin),
                          MakeTimeChecker())
            .AddAttribute("HyStartDelayMax",
                          "Maximum time for hystart algorithm",
                          TimeValue(MilliSeconds(1000)),
                          MakeTimeAccessor(&TcpCubicCr::m_hystartDelayMax),
                          MakeTimeChecker())
            .AddAttribute("CubicDelta",
                          "Delta Time to wait after fast recovery before adjusting param",
                          TimeValue(MilliSeconds(10)),
                          MakeTimeAccessor(&TcpCubicCr::m_cubicDelta),
                          MakeTimeChecker())
            .AddAttribute("CntClamp",
                          "Counter value when no losses are detected (counter is used"
                          " when incrementing cWnd in congestion avoidance, to avoid"
                          " floating point arithmetic). It is the modulo of the (avoided)"
                          " division",
                          UintegerValue(20),
                          MakeUintegerAccessor(&TcpCubicCr::m_cntClamp),
                          MakeUintegerChecker<uint8_t>())
            .AddAttribute("C",
                          "Cubic Scaling factor",
                          DoubleValue(0.4),
                          MakeDoubleAccessor(&TcpCubicCr::m_c),
                          MakeDoubleChecker<double>(0.0));
    return tid;
}

TcpCubicCr::TcpCubicCr()
    : TcpCongestionOps(),
      m_cWndCnt(0),
      m_lastMaxCwnd(0),
      m_bicOriginPoint(0),
      m_bicK(0.0),
      m_delayMin(Time::Min()),
      m_epochStart(Time::Min()),
      m_found(0),
      m_lastWindow(10),
      m_lastRtt(Time::Min()),
      m_crState(CarefulResumeState::CR_RECON),
      m_ssthreshReset(0),
      m_progGrowth(false),
      m_roundStart(Time::Min()),
      m_endSeq(0),
      m_lastAck(Time::Min()),
      m_cubicDelta(Time::Min()),
      m_currRtt(Time::Min()),
      m_sampleCnt(0),
      m_limit(SequenceNumber32(0)),
      m_hystartRounds(0),
      m_divisor(1),
      m_baselineRtt(Time::Min())
{
    NS_LOG_FUNCTION(this);
}

TcpCubicCr::TcpCubicCr(const TcpCubicCr& sock)
    : TcpCongestionOps(sock),
      m_fastConvergence(sock.m_fastConvergence),
      m_beta(sock.m_beta),
      m_hystart(sock.m_hystart),
      m_hystartDetect(sock.m_hystartDetect),
      m_hystartLowWindow(sock.m_hystartLowWindow),
      m_hystartAckDelta(sock.m_hystartAckDelta),
      m_hystartDelayMin(sock.m_hystartDelayMin),
      m_hystartDelayMax(sock.m_hystartDelayMax),
      m_hystartMinSamples(sock.m_hystartMinSamples),
      m_initialCwnd(sock.m_initialCwnd),
      m_cntClamp(sock.m_cntClamp),
      m_c(sock.m_c),
      m_cWndCnt(sock.m_cWndCnt),
      m_lastMaxCwnd(sock.m_lastMaxCwnd),
      m_bicOriginPoint(sock.m_bicOriginPoint),
      m_bicK(sock.m_bicK),
      m_delayMin(sock.m_delayMin),
      m_epochStart(sock.m_epochStart),
      m_found(sock.m_found),
      m_lastWindow(sock.m_lastWindow),
      m_lastRtt(sock.m_lastRtt),
      m_crState(sock.m_crState),
      m_ssthreshReset(sock.m_ssthreshReset),
      m_progGrowth(sock.m_progGrowth),
      m_roundStart(sock.m_roundStart),
      m_endSeq(sock.m_endSeq),
      m_lastAck(sock.m_lastAck),
      m_cubicDelta(sock.m_cubicDelta),
      m_currRtt(sock.m_currRtt),
      m_sampleCnt(sock.m_sampleCnt),
      m_limit(sock.m_limit),
      m_hystartRounds(sock.m_hystartRounds),
      m_divisor(sock.m_divisor),
      m_baselineRtt(sock.m_baselineRtt)
{
    NS_LOG_FUNCTION(this);
}

std::string
TcpCubicCr::GetName() const
{
    return "TcpCubicCr";
}

void
TcpCubicCr::HystartReset(Ptr<const TcpSocketState> tcb)
{
    NS_LOG_FUNCTION(this);

    m_roundStart = m_lastAck = Simulator::Now();
    m_endSeq = tcb->m_highTxMark;
    m_currRtt = Time::Min();
    m_sampleCnt = 0;
    // m_limit = SequenceNumber32(0);
	// std::cout<<"crap";
}

void
TcpCubicCr::IncreaseWindow(Ptr<TcpSocketState> tcb, uint32_t segmentsAcked)
{
    NS_LOG_FUNCTION(this << tcb << segmentsAcked);

    if (tcb->m_cWnd < tcb->m_ssThresh)
    {
        if (m_hystart && tcb->m_lastAckedSeq > m_endSeq)
        {
            HystartReset(tcb);
            m_hystartRounds++;
        }

        // In Linux, the QUICKACK socket option enables the receiver to send
        // immediate acks initially (during slow start) and then transition
        // to delayed acks.  ns-3 does not implement QUICKACK, and if ack
        // counting instead of byte counting is used during slow start window
        // growth, when TcpSocket::DelAckCount==2, then the slow start will
        // not reach as large of an initial window as in Linux.  Therefore,
        // we can approximate the effect of QUICKACK by making this slow
        // start phase perform Appropriate Byte Counting (RFC 3465)
        tcb->m_cWnd += segmentsAcked * tcb->m_segmentSize / m_divisor;
        segmentsAcked = 0;

        NS_LOG_INFO("In SlowStart, updated to cwnd " << tcb->m_cWnd << " ssthresh "
                                                     << tcb->m_ssThresh);
    }

    if (tcb->m_cWnd >= tcb->m_ssThresh && segmentsAcked > 0)
    {
        m_cWndCnt += segmentsAcked;
        uint32_t cnt = Update(tcb);

        /* According to RFC 6356 even once the new cwnd is
         * calculated you must compare this to the number of ACKs received since
         * the last cwnd update. If not enough ACKs have been received then cwnd
         * cannot be updated.
         */
        if (m_cWndCnt >= cnt)
        {
            tcb->m_cWnd += tcb->m_segmentSize;
            m_cWndCnt -= cnt;
            NS_LOG_INFO("In CongAvoid, updated to cwnd " << tcb->m_cWnd);
        }
        else
        {
            NS_LOG_INFO("Not enough segments have been ACKed to increment cwnd."
                        "Until now "
                        << m_cWndCnt << " cnd " << cnt);
        }
    }
}

uint32_t
TcpCubicCr::Update(Ptr<TcpSocketState> tcb)
{
    NS_LOG_FUNCTION(this);
    Time t;
    uint32_t delta;
    uint32_t bicTarget;
    uint32_t cnt = 0;
    double offs;
    uint32_t segCwnd = tcb->GetCwndInSegments();

    if (m_epochStart == Time::Min())
    {
        m_epochStart = Simulator::Now(); // record the beginning of an epoch

        if (m_lastMaxCwnd <= segCwnd)
        {
            NS_LOG_DEBUG("lastMaxCwnd <= m_cWnd. K=0 and origin=" << segCwnd);
            m_bicK = 0.0;
            m_bicOriginPoint = segCwnd;
        }
        else
        {
            m_bicK = std::pow((m_lastMaxCwnd - segCwnd) / m_c, 1 / 3.);
            m_bicOriginPoint = m_lastMaxCwnd;
            NS_LOG_DEBUG("lastMaxCwnd > m_cWnd. K=" << m_bicK << " and origin=" << m_lastMaxCwnd);
        }
    }

    t = Simulator::Now() + m_delayMin - m_epochStart;

    if (t.GetSeconds() < m_bicK) /* t - K */
    {
        offs = m_bicK - t.GetSeconds();
        NS_LOG_DEBUG("t=" << t.GetSeconds() << " <k: offs=" << offs);
    }
    else
    {
        offs = t.GetSeconds() - m_bicK;
        NS_LOG_DEBUG("t=" << t.GetSeconds() << " >= k: offs=" << offs);
    }

    /* Constant value taken from Experimental Evaluation of Cubic Tcp, available at
     * eprints.nuim.ie/1716/1/Hamiltonpfldnet2007_cubic_final.pdf */
    delta = m_c * std::pow(offs, 3);

    NS_LOG_DEBUG("delta: " << delta);

    if (t.GetSeconds() < m_bicK)
    {
        // below origin
        bicTarget = m_bicOriginPoint - delta;
        NS_LOG_DEBUG("t < k: Bic Target: " << bicTarget);
    }
    else
    {
        // above origin
        bicTarget = m_bicOriginPoint + delta;
        NS_LOG_DEBUG("t >= k: Bic Target: " << bicTarget);
    }

    // Next the window target is converted into a cnt or count value. CUBIC will
    // wait until enough new ACKs have arrived that a counter meets or exceeds
    // this cnt value. This is how the CUBIC implementation simulates growing
    // cwnd by values other than 1 segment size.
    if (bicTarget > segCwnd)
    {
        cnt = segCwnd / (bicTarget - segCwnd);
        NS_LOG_DEBUG("target>cwnd. cnt=" << cnt);
    }
    else
    {
        cnt = 100 * segCwnd;
    }

    if (m_lastMaxCwnd == 0 && cnt > m_cntClamp)
    {
        cnt = m_cntClamp;
    }

    // The maximum rate of cwnd increase CUBIC allows is 1 packet per
    // 2 packets ACKed, meaning cwnd grows at 1.5x per RTT.
    return std::max(cnt, 2U);
}

void
TcpCubicCr::PktsAcked(Ptr<TcpSocketState> tcb, uint32_t segmentsAcked, const Time& rtt)
{
    uint32_t limit_bytes, target_bytes;

    NS_LOG_FUNCTION(this << tcb << segmentsAcked << rtt);

    /* Careful Resume */
    if (m_crState == CarefulResumeState::CR_RECON)
    {
        /* Actions to perform in Recoinassance phase */
        if (rtt <= m_lastRtt/10 || rtt >= m_lastRtt*10)
        {
            /* Drop plans to jump because past RTT is too different */
            m_crState = CarefulResumeState::CR_NORMAL;
        }
        limit_bytes = tcb->m_segmentSize * (tcb->m_initialCWnd-1);
        m_limit = SequenceNumber32(limit_bytes);

        if (m_progGrowth) {
            target_bytes = tcb->m_segmentSize * segmentsAcked *
                            (m_lastWindow - tcb->m_initialCWnd);
//			printf("increase=%u iw=%u segmentsAcked=%u\n", target_bytes/tcb->m_initialCWnd, tcb->m_initialCWnd, segmentsAcked);

            tcb->m_cWnd += target_bytes / tcb->m_initialCWnd; 
        }

        if (tcb->m_lastAckedSeq >= m_limit) {
            /* Switch to Unvalidated phase */
            m_crState = CarefulResumeState::CR_UNVAL;

            /* Calculate new seqno boundary for cwnd validation */
            limit_bytes += tcb->m_segmentSize * m_lastWindow;
            m_limit = SequenceNumber32(limit_bytes);

            tcb->m_cWnd = limit_bytes;
        }
        return;
    }

    if (m_crState == CarefulResumeState::CR_UNVAL) 
    {
        /* Actions to perform in Unvalidated phase */
        if (tcb->m_lastAckedSeq >= m_limit) {
           /* Previous cwnd is now validated, resume normally */
            m_crState = CarefulResumeState::CR_NORMAL;
            if (m_ssthreshReset>0)
                tcb->m_ssThresh = m_ssthreshReset*tcb->m_cWnd; 
        }
    }

    /* Careful Resume proceeds with normal congestion control */


    /* Discard delay samples right after fast recovery */
    if (m_epochStart != Time::Min() && (Simulator::Now() - m_epochStart) < m_cubicDelta)
    {
        return;
    }

    /* first time call or link delay decreases */
    if (m_delayMin == Time::Min() || m_delayMin > rtt)
    {
        m_delayMin = rtt;
    }

    /* hystart triggers when cwnd is larger than some threshold */
    if (m_hystart && tcb->m_cWnd <= tcb->m_ssThresh &&
        tcb->m_cWnd >= m_hystartLowWindow * tcb->m_segmentSize)
    {
        HystartUpdate(tcb, rtt);
    }
}

void
TcpCubicCr::HystartUpdate(Ptr<TcpSocketState> tcb, const Time& delay)
{
    NS_LOG_FUNCTION(this << delay);

    if (!(m_found & m_hystartDetect))
    {
        Time now = Simulator::Now();

        /* first detection parameter - ack-train detection */
        if ((now - m_lastAck) <= m_hystartAckDelta)
        {
            m_lastAck = now;

            if ((now - m_roundStart) > m_delayMin)
            {
                m_found |= PACKET_TRAIN;
            }
        }

        /* obtain the minimum delay of more than sampling packets */
        if (m_sampleCnt < m_hystartMinSamples)
        {
            if (m_currRtt == Time::Min() || m_currRtt > delay)
            {
                m_currRtt = delay;
            }

            ++m_sampleCnt;
        }
        else
        {
            if (m_currRtt > m_delayMin + HystartDelayThresh(m_delayMin))
            {
                m_found |= DELAY;
            }
        }

        /*
         * Either one of two conditions are met,
         * we exit from slow start immediately.
         */
        if ((m_found & m_hystartDetect) && !m_hystartpp)
        {
            NS_LOG_DEBUG("Exit from SS, immediately :-)");
            tcb->m_ssThresh = tcb->m_cWnd;
        }

        /* if CSS is enabled, record baseline RTT and
         * entry round, and change SS growth rate (probing)
         */
        if ((m_found & m_hystartDetect) && m_hystartpp)
        {
            m_baselineRtt = m_delayMin;
            m_divisor = m_css_growth_divisor;
            m_css_first_round = m_hystartRounds;
        }
    }

    /* Conservative Slow Start (CSS) */
    if ((m_found & m_hystartDetect) && m_hystartpp)
    {
        /* Enter CA after max rounds in CSS */
        if (m_hystartRounds - m_css_first_round > m_css_max_rounds)
        {
            tcb->m_ssThresh = tcb->m_cWnd;
            m_currRtt = Time::Min();
            m_divisor = 1;
        }
        else
		{
            /*  Back to SS in delay drops below baseline */
            if (m_delayMin < m_baselineRtt) {
                m_divisor = 1;
                m_found = 0;
            }
        }
    }

}

Time
TcpCubicCr::HystartDelayThresh(const Time& t) const
{
    NS_LOG_FUNCTION(this << t);

    Time ret = t/8;
    if (t > m_hystartDelayMax)
    {
        ret = m_hystartDelayMax;
    }
    else if (t < m_hystartDelayMin)
    {
        ret = m_hystartDelayMin;
    }

    return ret;
}

uint32_t
TcpCubicCr::GetSsThresh(Ptr<const TcpSocketState> tcb, uint32_t bytesInFlight)
{
    NS_LOG_FUNCTION(this << tcb << bytesInFlight);

	if (m_crState == CarefulResumeState::CR_RECOVERY) 
	{
		m_crState = CarefulResumeState::CR_NORMAL;
		return 40 * tcb->m_segmentSize;
	}
	

    uint32_t segCwnd = tcb->GetCwndInSegments();
    NS_LOG_DEBUG("Loss at cWnd=" << segCwnd
                                 << " segments in flight=" << bytesInFlight / tcb->m_segmentSize);

    /* Wmax and fast convergence */
    if (segCwnd < m_lastMaxCwnd && m_fastConvergence)
    {
        m_lastMaxCwnd = (segCwnd * (1 + m_beta)) / 2; // Section 4.6 in RFC 8312
    }
    else
    {
        m_lastMaxCwnd = segCwnd;
    }

    m_epochStart = Time::Min(); // end of epoch

    /* Formula taken from the Linux kernel */
    uint32_t ssThresh = std::max(static_cast<uint32_t>(segCwnd * m_beta), 2U) * tcb->m_segmentSize;

    NS_LOG_DEBUG("SsThresh = " << ssThresh);

    return ssThresh;
}

void
TcpCubicCr::CongestionStateSet(Ptr<TcpSocketState> tcb, const TcpSocketState::TcpCongState_t newState)
{
    NS_LOG_FUNCTION(this << tcb << newState);

    if (m_crState == CarefulResumeState::CR_UNVAL &&
		(newState == TcpSocketState::CA_RECOVERY || 
        newState == TcpSocketState::CA_LOSS))
    {
        m_crState = CarefulResumeState::CR_RECOVERY;
    }

    if (newState == TcpSocketState::CA_LOSS)
    {
        CubicReset(tcb);
        HystartReset(tcb);
    }
	

}

void
TcpCubicCr::CubicReset(Ptr<const TcpSocketState> tcb)
{
    NS_LOG_FUNCTION(this << tcb);

    m_lastMaxCwnd = 0;
    m_bicOriginPoint = 0;
    m_bicK = 0;
    m_delayMin = Time::Min();
    m_found = false;
}

Ptr<TcpCongestionOps>
TcpCubicCr::Fork()
{
    NS_LOG_FUNCTION(this);
    return CopyObject<TcpCubicCr>(this);
}


// Careful Resume Recovery 


NS_OBJECT_ENSURE_REGISTERED(TcpCrRecovery);

TypeId
TcpCrRecovery::GetTypeId()
{
    static TypeId tid = TypeId("ns3::TcpCrRecovery")
                            .SetParent<TcpClassicRecovery>()
                            .SetGroupName("Internet")
                            .AddConstructor<TcpCrRecovery>();
    return tid;
}

TcpCrRecovery::TcpCrRecovery()
    : TcpClassicRecovery()
{
    NS_LOG_FUNCTION(this);
}

TcpCrRecovery::TcpCrRecovery(const TcpCrRecovery& sock)
    : TcpClassicRecovery(sock),
	m_enterRecoveryTime(Time::Min()),
	m_rttLastAck(Time::Min()),
	m_probeEnd(Time::Min())
{
    NS_LOG_FUNCTION(this);
	m_cr_done = false;
}

TcpCrRecovery::~TcpCrRecovery()
{
    NS_LOG_FUNCTION(this);
}


void
TcpCrRecovery::EnterRecovery(Ptr<TcpSocketState> tcb,
                                  uint32_t dupAckCount,
                                  uint32_t unAckDataCount [[maybe_unused]],
                                  uint32_t deliveredBytes [[maybe_unused]])
{
    NS_LOG_FUNCTION(this << tcb << dupAckCount << unAckDataCount);

	m_enterRecoveryTime = Simulator::Now();
	m_rttLastAck = tcb->m_minRtt;
	m_probeEnd = m_enterRecoveryTime + 5*m_rttLastAck/4;

    //tcb->m_cWnd = tcb->m_ssThresh;
	tcb->m_cWnd = tcb->m_segmentSize;	


    tcb->m_cWndInfl = tcb->m_ssThresh + (dupAckCount * tcb->m_segmentSize);

	if (m_cr_done == true)
		printf("JK-TRUE ");
	else
		printf("JK-FALSE ");

}

void
TcpCrRecovery::DoRecovery(Ptr<TcpSocketState> tcb, uint32_t deliveredBytes [[maybe_unused]])
{
    NS_LOG_FUNCTION(this << tcb << deliveredBytes);
	Time now = Simulator::Now();


	if (now < m_probeEnd)
		tcb->m_cWnd += tcb->m_segmentSize/2;

    tcb->m_cWndInfl += tcb->m_segmentSize;


}

void
TcpCrRecovery::ExitRecovery(Ptr<TcpSocketState> tcb)
{
    NS_LOG_FUNCTION(this << tcb);
    // Follow NewReno procedures to exit FR if SACK is disabled
    // (RFC2582 sec.3 bullet #5 paragraph 2, option 2)
    // In this implementation, actual m_cWnd value is reset to ssThresh
    // immediately before calling ExitRecovery(), so we just need to
    // reset the inflated cWnd trace variable
    tcb->m_cWndInfl = tcb->m_ssThresh.Get();

	std::cout << "         >>      " << Simulator::Now().GetSeconds();
	std::cout << "AR:" << tcb->GetSsThreshInSegments();
	std::cout << "ARC:" << tcb->GetCwndInSegments();
	tcb->m_ssThresh = 2 * tcb->m_cWnd;
	std::cout << "ER:" << tcb->GetSsThreshInSegments();
}

std::string
TcpCrRecovery::GetName() const
{
    return "TcpCrRecovery";
}

Ptr<TcpRecoveryOps>
TcpCrRecovery::Fork()
{
    return CopyObject<TcpCrRecovery>(this);
}

} // namespace ns3
