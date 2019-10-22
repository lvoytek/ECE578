gnuplot <<- EOF
 set xlabel "Rate λ (frames/sec)"
 set ylabel "Throughput (Kbps)"
 set datafile separator ","
 set term png
 set key left top
 set grid
 set output "1A.png"
 set title "Node A Throughput Versus Rate (λ_A = λ_C)"
 plot "<(sed -n '1,6p' build/A1simout.csv)" using 1:4 with line title "A1", \
      "<(sed -n '1,6p' build/A2simout.csv)" using 1:4 with line title "A2" \
 
 set output "1B.png"
 set title "Node C Throughput Versus Rate (λ_A = λ_C)"
 plot "<(sed -n '1,6p' build/A1simout.csv)" using 1:5 with line title "A1", \
      "<(sed -n '1,6p' build/A2simout.csv)" using 1:5 with line title "A2"

 set output "1C.png"
 set title "Node A Throughput Versus Rate (λ_A = 2λ_C)"
 plot "<(sed -n '7,11p' build/A1simout.csv)" using 1:4 with line title "A1", \
      "<(sed -n '7,11p' build/A2simout.csv)" using 1:4 with line title "A2" \
 
 set output "1D.png"
 set title "Node C Throughput Versus Rate (λ_A = 2λ_C)"
 plot "<(sed -n '7,11p' build/A1simout.csv)" using 1:5 with line title "A1", \
      "<(sed -n '7,11p' build/A2simout.csv)" using 1:5 with line title "A2" \
 



 set ylabel "Collisions"


 set output "2A.png"
 set title "Node A Collisions Versus Rate (λ_A = λ_C)"
 plot "<(sed -n '1,6p' build/A1simout.csv)" using 1:6 with line title "A1", \
      "<(sed -n '1,6p' build/A2simout.csv)" using 1:6 with line title "A2"


 set output "2B.png"
 set title "Node C Collisions Versus Rate (λ_A = λ_C)"
 plot "<(sed -n '1,6p' build/A1simout.csv)" using 1:7 with line title "A1", \
      "<(sed -n '1,6p' build/A2simout.csv)" using 1:7 with line title "A2"


 set output "2C.png"
 set title "Node A Throughput Versus Rate (λ_A = 2λ_C)"
 plot "<(sed -n '7,11p' build/A1simout.csv)" using 1:6 with line title "A1", \
      "<(sed -n '7,11p' build/A2simout.csv)" using 1:6 with line title "A2" \
 
 set output "2D.png"
 set title "Node C Throughput Versus Rate (λ_A = 2λ_C)"
 plot "<(sed -n '7,11p' build/A1simout.csv)" using 1:7 with line title "A1", \
      "<(sed -n '7,11p' build/A2simout.csv)" using 1:7 with line title "A2" \
 


 set ylabel "Fairness Index"


 set output "3A.png"
 set title "Fairness Index Versus Rate (λ_A = λ_C)"
 plot "<(sed -n '1,6p' build/A1simout.csv)" using 1:8 with line title "A1", \
      "<(sed -n '1,6p' build/A2simout.csv)" using 1:8 with line title "A2"

 set output "3B.png"
 set title "Fairness Index Versus Rate (λ_A = 2λ_C)"
 plot "<(sed -n '7,11p' build/A1simout.csv)" using 1:8 with line title "A1", \
      "<(sed -n '7,11p' build/A2simout.csv)" using 1:8 with line title "A2" \
 



EOF
