#!/usr/bin/gnuplot


set term pngcairo dashed
set out "comp_leo.png"

#set multiplot layout 2,1

set xlabel "transfer size (packets)"
set ylabel "completion time (RTTs)"
set key left box
set grid

set title "Bandwidth=100Mb/s, RTT=50ms"
#set yrange [0:1.2]
plot 'leo_cr_cwnd200.tr'  u ($1/1.5):($3/0.05) t 'CR, cwnd=200' w l, \
	 'leo_cubic_iw10.tr'  u ($1/1.5):($3/0.05) t 'Cubic, IW=10' w l, \
     'leo_cubic_iw200.tr' u ($1/1.5):($3/0.05) t 'Cubic, IW=200' w l

#     x/12.5+0.05 t 'ideal' w l dt 7 lw 1.5 lc 8


