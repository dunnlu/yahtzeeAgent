import game_module as game_module # Make sure to change this according to where your agent file is, mine is currently in the main repository. 
import random 
import time 
from tabular_q_learning import TQ_agent 

# These just don't matter in the evaluation, in order to read the q table you are required to initialize the agent again. 
# It is bad implementation. 

epsilon = 0.3 
alpha = 0.3 
batch_size = 100 
batch_count = 10 
trial_count = 100 

agent = TQ_agent( epsilon  , alpha , batch_size , batch_count , trial_count ) 
agent.create_demonstration( "not_just_lower" , 1 , just_lower= False ) 

