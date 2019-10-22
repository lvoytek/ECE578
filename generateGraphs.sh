gnuplot <<- EOF
	set title "Node A Throughput Versus Rate (λ = λ_A = λ_C)"
	set xlabel "Rate λ (frames/sec)"
	set ylabel "Throughput (Kbps)"
	set datafile separator ","
	set term png
	set output "1A.png"
	set key left top
	set grid
	plot "<(sed -n '1,6p' build/A1simout.csv)" using 1:4 with line title "A1", \
	     "<(sed -n '1,6p' build/A2simout.csv)" using 1:4 with line title "A2" \
	
	
	set output "1B.png"
	set title "Node C Throughput Versus Rate (λ = λ_A = λ_C)"
	plot "<(sed -n '1,6p' build/A1simout.csv)" using 1:5 with line title "A1", \
	     "<(sed -n '1,6p' build/A2simout.csv)" using 1:5 with line title "A2" \
EOF

