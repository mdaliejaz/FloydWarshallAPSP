CC=cilk++
CFLAGS=-O3 
#-I $(HOME)/cilk/include/cilk++
all:	FW_iter_p FW_rec1 FW_rec2 FW_rec3 FW_iter_omp FW_rec1_omp FW_rec2_omp FW_rec3_omp
FW_iter_p: 
	$(CC) $(CFLAGS) -o FWiterp FW_iter_p.cilk -lcilkutil
FW_rec1: 
	$(CC) $(CFLAGS) -o FWrec1 FW_rec1.cilk -lcilkutil
FW_rec2: 
	$(CC) $(CFLAGS) -o FWrec2 FW_rec2.cilk -lcilkutil
FW_rec3: 
	$(CC) $(CFLAGS) -o FWrec3 FW_rec3.cilk -lcilkutil
FW_iter_omp:
	icc $(CFLAGS) -fopenmp -o FWiteromp FW_iter_omp.cpp
FW_rec1_omp:
	icc $(CFLAGS) -fopenmp -o FWrec1omp FW_rec1_omp.cpp
FW_rec2_omp:
	icc $(CFLAGS) -fopenmp -o FWrec2omp FW_rec2_omp.cpp
FW_rec3_omp:
	icc $(CFLAGS) -fopenmp -o FWrec3omp FW_rec3_omp.cpp

clean:
	rm -rf FWiter FWiterp FWrec1 FWrec2 FWrec3 FWiteromp FWrec1omp FWrec2omp FWrec3omp  core*
