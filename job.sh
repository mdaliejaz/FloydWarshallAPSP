#!/bin/bash
#SBATCH -J Test        # Job Name
#SBATCH -o Test.o%j    # Output and error file name (%j expands to jobID)
#SBATCH -n 16          # Total number of  tasks requested
#SBATCH -p development  # Queue (partition) name -- normal, development, etc.
#SBATCH -t 02:00:00     # Run time (hh:mm:ss) - 1.5 hours

export CILK_NWORKERS=32
export OMP_NUM_THREADS=16

#rm *.csv *.plt Test*	
#./FWer > FWiter_output
cilkview -trials all ./FWrec3 8192 64 > cilkview_output
#./FWiterp 4096 32 > FWiterp_output
#./FWiterp 2048 32 > FWiterp_output
#./FWiterp 1024 32 > FWiterp_output

#./FWiterp 512 32 > FWiterp_output
#./FWiterp 256 32 > FWiterp_output
#./FWiterp 128 32 > FWiterp_output
#./FWiterp 64 32 > FWiterp_output
#./FWiterp 32 32 > FWiterp_output
#./FWiterp 16 32 > FWiterp_output
#./FWrec1 $1 $2 > FWrec1_output
#./FWrec2 $1 $2 > FWrec2_output
#./FWrec3 16 64 > FWrec3_output
#./FWrec3 32 64 > FWrec3_output
#./FWrec2 16 32 > FWrec2_output
#./FWiteromp 8192 8 > FWiteromp_output8
#./FWrec2omp $1 4 > FWrec2omp_output4
#./FWrec2omp $1 8 > FWrec2omp_output8
#./FWrec2omp $1 32 > FWrec2omp_output32
#./FWrec2omp $1 64 > FWrec2omp_output64
#./FWrec2omp $1 128 > FWrec2omp_output128
#./FWrec2omp $1 256 > FWrec2omp_output256
#./FWrec2omp $1 512 > FWrec2omp_output512
#./FWrec3omp $1 1024 > FWrec3omp_output1024
#./FWrec3omp $1 2048 > FWrec3omp_output2048
#./FWrec3omp $1 4096 > FWrec3omp_output4096
#./FWiteromp 4096 8 > FWiteromp_outputn4096
#./FWiteromp 2048 8 > FWiteromp_outputn2048
#./FWiteromp 1024 8 > FWiteromp_outputn1024
#./FWiteromp 512 8 > FWiteromp_outputn512
#./FWiteromp 256 8 > FWiteromp_outputn256
#./FWiteromp 64 8 > FWiteromp_outputn64
#./FWiteromp 128 8 > FWiteromp_outputn128
#./FWiteromp 32 8 > FWiteromp_outputn32
#./FWiteromp 16 8 > FWiteromp_outputn16

#./FWrec1omp 8192 8 > FWrec1omp_output8
#./FWrec1omp 8192 16 > FWrec1omp_output16
#./FWiteromp 8192 8 > FWiteromp_output
#./FWrec3omp 8192 16 > FWrec3omp_output16
#./FWrec1 4096 16 > FWrec1_output
#./FWrec1 2048 16 > FWrec1_output
#./FWrec1 1024 16 > FWrec1_output
#./FWrec1 512 16 > FWrec1_output
#./FWrec1 256 16 > FWrec1_output
#./FWrec1 128 16 > FWrec1_output
#./FWrec1 64 16 > FWrec1_output
#./FWrec1 32 16 > FWrec1_output
#./FWrec1 16 16 > FWrec1_output


