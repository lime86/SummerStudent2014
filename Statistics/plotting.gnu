set term pngcairo size 1280,960 enhanced font 'Verdana,20'
set termopt enhanced
set output "output.png"
unset key
set view map
set xtics border in scale 0,0 mirror norotate  offset character 0, 0, 0 autojustify
set ytics border in scale 0,0 mirror norotate  offset character 0, 0, 0 autojustify
set ztics border in scale 0,0 nomirror norotate  offset character 0, 0, 0 autojustify
set xlabel "Mass of DM particle [GeV]"
set xlabel  offset character 0, 0, 0 font "" textcolor lt -1 norotate
set ylabel "Mass of mediator particle [GeV]"
set ylabel  offset character 0, 0, 0 font "" textcolor lt -1 rotate by -270
set rtics axis in scale 0,0 nomirror norotate  offset character 0, 0, 0 autojustify
set title "Limits on coupling"
set xrange [ -0.5 : 1300.5 ] noreverse nowriteback
set yrange [ -0.5 : 6000.5 ] noreverse nowriteback
set cblabel "95% CL upper limit on coupling"
set palette defined (-1.0 "black", -0.1 "blue", 5 "yellow", 10 "green", 15 "red")
#set cbrange [ 0.00000 : 15.00000 ]
plot 'testfile' using 1:2:3 with image
