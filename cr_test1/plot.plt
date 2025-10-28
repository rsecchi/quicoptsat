#!/usr/bin/gnuplot

set term png
set out "iw_no_cc.png"

#set multiplot layout 2,1

set xlabel "time (s)"
set ylabel "cwnd (packets)"
set key left


plot 'cwnd_iw10.tr' u 1:($2/1500) t 'IW=10' w l, \
	 'cwnd_iw20.tr' u 1:($2/1500) t 'IW=20' w l, \
     'cwnd_iw100.tr' u 1:($2/1500) t 'IW=100' w l, \
     'cwnd_iw200.tr' u 1:($2/1500) t 'IW=200' w l


#plot 'bq_iw10.tr' u 1:($2/1500) w l, \
#	 'bq_iw20.tr' u 1:($2/1500) w l, \
#    'bq_iw100.tr' u 1:($2/1500) w l, \
#     'bq_iw200.tr' u 1:($2/1500) w l



