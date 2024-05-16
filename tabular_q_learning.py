import test.game_module as game_module # Make sure to change this according to where your agent file is, mine is currently in the main repository. 
import random

class TQ_agent: 
    
    def __init__( self , epsilon = 0.05 , alpha = 0.05 , gamma = 1 , batch_size = 1000 , batch_count = 1000 , trial_count = 10000 ) : 

        # 1. Initialise the hyper parameters. 
        self.epsilon = epsilon  
        self.alpha = alpha  
        self.gamma = gamma 
        self.batch_size = batch_size 
        self.batch_count = batch_count 
        self.trial_count = trial_count 
        self.reward_list = [] 
        self.q_table = {} # { ( tuple( state ) , tuple( action ) ) : value } 

        self.possible_if_there_is_remaining_rolls = [ 

            [ 0 , 0 , 0 , 0 , 0 , -1 ] , # roll all 
            [ 0 , 0 , 0 , 0 , 1 , -1 ] , 
            [ 0 , 0 , 0 , 1 , 0 , - 1 ] , 
            [ 0 , 0 , 0 , 1 , 1 , - 1 ] , 
            [ 0 , 0 , 1 , 0 , 0 , - 1 ] , 
            [ 0 , 0 , 1 , 0 , 1 , - 1 ] , 
            [ 0 , 0 , 1 , 1 , 0 , - 1 ] , 
            [ 0 , 0 , 1 , 1 , 1 , - 1 ] , 
            [ 0 , 1 , 0 , 0 , 0 , - 1 ] , 
            [ 0 , 1 , 0 , 0 , 1 , - 1 ] , 
            [ 0 , 1 , 0 , 1 , 0 , - 1 ] , 
            [ 0 , 1 , 0 , 1 , 1 , - 1 ] , 
            [ 0 , 1 , 1 , 0 , 0 , - 1 ] , 
            [ 0 , 1 , 1 , 0 , 1 , - 1 ] , 
            [ 0 , 1 , 1 , 1 , 0 , - 1 ] , 
            [ 0 , 1 , 1 , 1 , 1 , - 1 ] , 
            [ 1 , 0 , 0 , 0 , 0 , - 1 ] , 
            [ 1 , 0 , 0 , 0 , 1 , - 1 ] , 
            [ 1 , 0 , 0 , 1 , 0 , - 1 ] , 
            [ 1 , 0 , 0 , 1 , 1 , - 1 ] , 
            [ 1 , 0 , 1 , 0 , 0 , - 1 ] , 
            [ 1 , 0 , 1 , 0 , 1 , - 1 ] , 
            [ 1 , 0 , 1 , 1 , 0 , - 1 ] , 
            [ 1 , 0 , 1 , 1 , 1 , - 1 ] , 
            [ 1 , 1 , 0 , 0 , 0 , - 1 ] , 
            [ 1 , 1 , 0 , 0 , 1 , - 1 ] , 
            [ 1 , 1 , 0 , 1 , 0 , - 1 ] , 
            [ 1 , 1 , 0 , 1 , 1 , - 1 ] , 
            [ 1 , 1 , 1 , 0 , 0 , - 1 ] , 
            [ 1 , 1 , 1 , 0 , 1 , - 1 ] , 
            [ 1 , 1 , 1 , 1 , 0 , - 1 ] , 

        ] 
    
    def get_epsilon_greedy_action(self, state): # TODO: Not sure. 
        
        possible_actions = self.get_the_possible_actions(state)
        best_q = float("-inf")
        best_action = None
        for action in possible_actions: 
            # Check if the curr_hash is in the table. 
            curr_hash = ( tuple( state ) , tuple( action ) ) 
            if curr_hash not in self.q_table : 
                self.q_table[curr_hash] = 0 
            current_q = self.q_table[curr_hash]
            if current_q > best_q:
                best_q = current_q
                best_action = action
        if random.random() > self.epsilon: 
            return best_action 
        else: 
            return random.choice(possible_actions) 

    def get_the_best_action( self , state ) : 

        possible_actions = self.get_the_possible_actions( state ) 
        best_q = float("-inf") 
        best_action = None 
        for action in possible_actions : 
            # Check if the curr_hash is in the table. 
            curr_hash = ( tuple( state ) , tuple( action ) ) 
            if curr_hash not in self.q_table : 
                self.q_table[curr_hash] = 0 
            current_q = self.q_table[curr_hash]
            if current_q > best_q : 
                best_q = current_q 
                best_action = action 
        return best_action 

    def update_the_q_table( self , state , action , reward , next_state , best_action ) : 
        
        if best_action == None : # TODO: this is weird 
            self.q_table[ ( tuple(state) , tuple( action ) ) ] = self.q_table[ ( tuple(state) , tuple( action ) ) ] + self.alpha * ( reward + self.gamma * 0 - self.q_table[ ( tuple(state) , tuple( action ) ) ] )  

        else : 
            if ( tuple(state) , tuple( action ) ) not in self.q_table : 
                self.q_table[ ( tuple(state) , tuple( action ) ) ] = 0 
            if ( tuple( next_state ) , tuple( best_action ) ) not in self.q_table : 
                self.q_table[ ( tuple( next_state ) , tuple( best_action ) ) ] = 0 
            self.q_table[ ( tuple(state) , tuple( action ) ) ] = self.q_table[ ( tuple(state) , tuple( action ) ) ] + self.alpha * ( reward + self.gamma * self.q_table[ ( tuple(next_state) , tuple( best_action ) ) ] - self.q_table[ ( tuple(state) , tuple( action ) ) ] ) 

    def get_the_possible_actions( self , state ) : 

        possible_actions = [] 
        remaining_rolls = state[ 5 ] 
        if remaining_rolls != 0 : 
            for action in self.possible_if_there_is_remaining_rolls : 
                possible_actions.append( action ) 
        remaining_categories = [] 
        for idx in range( 13 ) : 
            if state[ idx + 6 ] != 1 : 
                remaining_categories.append( idx ) 
        for category_idx in remaining_categories : 
            possible_actions.append( [ 1 , 1 , 1 , 1 , 1 , category_idx ] ) 

        #print(f"len(possible_actions)={len(possible_actions)}")

        return possible_actions 

    def train( self ) : 

        # Loop each episode. 
        for _ in range( self.batch_size ) : 
            game = game_module.Game() 
            state  = game.reset() 
            is_terminal = False 
            # Loop for each step of the episode. 
            while is_terminal == False : 
                action = self.get_epsilon_greedy_action( state ) 
                # print(f"state={state},action={action}")
                next_state , reward , is_terminal = game.step( action ) 
                best_action = self.get_the_best_action( next_state ) 
                self.update_the_q_table( state , action , reward , next_state , best_action ) 
                state = next_state 

    def test( self ) : 
        
        sum_reward = 0 
        # Loop each trial count. 
        for _ in range( self.trial_count ) : 
            #print(f"test_start")
            game = game_module.Game() 
            state  = game.reset() 
            is_terminal = False 
            # Loop for each step of the episode. 
            while is_terminal == False : 
                action = self.get_the_best_action( state ) 
                #print("test did not take action")
                next_state , reward , is_terminal = game.step( action ) 
                #print("test did not take action")
                state = next_state 
                sum_reward += reward # TODO: might not be correct. 
        
        return sum_reward / self.trial_count 
    
    def just_train( self ) : 
        idx = 0 
        for _ in range( self.batch_count ) : 

            idx += 1 
            print(idx)
            # print(f"test_count={test_count}")
            self.train() 

    def train_test( self ) : 

        for _ in range( self.batch_count ) : 
            # print(f"test_count={test_count}")
            self.train() 
            average_reward = self.test() 
            print( average_reward ) 
            self.reward_list.append( average_reward ) 

    def test_random_agent( self ) : 

        sum_reward = 0 
        # Loop each trial count. 
        for _ in range( self.trial_count ) : 
            #print(f"test_start")
            game = game_module.Game() 
            state  = game.reset() 
            is_terminal = False 
            # Loop for each step of the episode. 
            while is_terminal == False : 
                action = self.get_random_action( state ) 
                #print("test did not take action")
                next_state , reward , is_terminal = game.step( action ) 
                #print("test did not take action")
                state = next_state 
                sum_reward += reward # TODO: might not be correct. 
        
        return sum_reward / self.trial_count 
    
    def save_q_table_pickle(self, filename):
        import pickle
        with open(filename, 'wb') as f:
            pickle.dump(self.q_table, f) 

    def get_random_action( self , state ) : 
        possible_actions = self.get_the_possible_actions( state ) 
        # Sample an action from this list, each action has equal probability. 
        action = random.choice( possible_actions ) 
        return action 

agent = TQ_agent() 
# agent.train_test() 
agent.just_train() 
print(f"policy={agent.test()}") 
print(f"random={agent.test_random_agent()}") 
