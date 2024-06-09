### Value Iteration
This function runs value iteration. The valit.sh bash script is a script to be run on slurm on the OSU HPC. 
Output policy is saved every iteration in valitFiles as 'politN.txt'.
Output value is saved every iteration in valitFiles as 'valitN.txt'.

These files are used in functions such as evaluate. 

### Execution
To compile: 

    make

To run on local machine:

    ./valit

To run on HPC from a submit node:

    module load slurm
    sbatch valit.sh

### Settings
(parameters to the function call in main within valit.cpp)

valit(s,e) 

WHERE:
s = starting iteration (as stored in /valitFiles, or -1 to start from scratch)
e = ending iteration (where it will stop)

