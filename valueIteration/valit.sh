#!/bin/bash
#SBATCH -J valitsh						  # name of job
#SBATCH -p share								  # name of partition or queue
#SBATCH -o valitsh.o				  # name of output file for this submission script
#SBATCH -e valitsh.err				  # name of error file for this submission script
#SBATCH --constraint=el9
#SBATCH -t 3-00:00:00                   # time limit 3 days

# load any software environment module required for app (e.g. matlab, gcc, cuda)
module load gcc

# run my job (e.g. matlab, python)
./valit