#!/bin/bash
#SBATCH --job-name=batch_dqn     
#SBATCH --output=batch_dqn.log   
#SBATCH --error=batch_dqn.log       
#SBATCH --time=02:00:00           

# Load the module. 
module load python/3.9  

# Activate the virtual environment. 
source /nfs/stak/users/davrans/cs499/venv_2/bin/activate

# Run the script. 
python /nfs/stak/users/davrans/cs499/batch_dqn.py 
