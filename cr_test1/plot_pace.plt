#!/usr/bin/gnuplot


set term pngcairo dashed
set out "comp_pace.png"

#set multiplot layout 2,1

set xlabel "transfer size (packets)"
set ylabel "completion time (RTTs)"
set key left box
set grid

set title "Bandwidth=100Mb/s, RTT=50ms"
#set yrange [0:1.2]
plot 'leo_cr_cwnd200_pace.tr'  u ($1/1.5):($3/0.05) t 'pace' w l, \
	 'leo_cr_cwnd200_nopace.tr'  u ($1/1.5):($3/0.05) t 'nopace' w l, \
     'leo_cr_cwnd200_prog.tr' u ($1/1.5):($3/0.05) t 'prog' w l lw 3 lt 6

#     x/12.5+0.05 t 'ideal' w l dt 7 lw 1.5 lc 8


