#!/usr/bin/gnuplot

set term pngcairo dashed
set out "comp_geo.png"

#set multiplot layout 2,1

set xlabel "transfer size (packets)"
set ylabel "completion time (RTTs)"
set key left box
set grid

set title "Bandwidth=50Mb/s, RTT=500ms"
#set yrange [0:1.2]
plot 'geo_cr_cwnd900.tr'  u ($1/1.5):($3/0.5) t 'CR, cwnd=900' w l, \
	 'geo_cubic_iw10.tr'  u ($1/1.5):($3/0.5) t 'Cubic, IW=10' w l, \
     'geo_cubic_iw900.tr' u ($1/1.5):($3/0.5) t 'Cubic, IW=900' w l

#     x/6.25+0.5 t 'ideal' w l dt 7 lw 1.5 lc 8


