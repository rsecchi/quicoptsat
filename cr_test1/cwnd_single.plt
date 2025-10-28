#!/usr/bin/gnuplot

set term png
set out "cwnd.png"

#set multiplot layout 2,1

set xlabel "time (s)"
set ylabel "cwnd (packets)"
set key left


plot 'cwnd0.tr' u 1:($2/1500) t 'IW=10' w l

#plot 'bq_iw10.tr' u 1:($2/1500) w l, \
#	 'bq_iw20.tr' u 1:($2/1500) w l, \
#    'bq_iw100.tr' u 1:($2/1500) w l, \
#     'bq_iw200.tr' u 1:($2/1500) w l



