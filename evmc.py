import test.game_module as game_module # Make sure this matches the name of your module
import random
class evmc_agent:
    def __init__(self, epsilon = 0.5, gamma = 0.9, alpha = 0.5): 
        self.epsilon = epsilon
        self.gamma = gamma
        self.alpha = alpha
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
            [ 1 , 1 , 1 , 1 , 0 , - 1 ]] 
    
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

        return possible_actions
    
    def get_epsilon_greedy_action(self, state):
        
        possible_actions = self.get_the_possible_actions(state)
        best_q = float("-inf")
        best_action = None
        for action in possible_actions: 
            # Check if the curr_hash is in the table. 
            curr_hash = ( tuple( state ) , tuple( action ) ) 
            if curr_hash not in self.q_table : 
                self.q_table[curr_hash] = 0 
            current_q = self.q_table[curr_hash]
            if current_q >= best_q:
                best_q = current_q
                best_action = action
        
        """
        n = len(possible_actions)
        probabilities = [self.epsilon / n] * n  # Initialize probabilities for all items
        best_index = possible_actions.index(best_action)  # Find the index of item_best
        probabilities[best_index] = 1 - self.epsilon + (self.epsilon / n)  # Set higher probability for item_best

        return random.choices(possible_actions, weights=probabilities, k=1)[0]
        """

        if random.random() > self.epsilon: 
            return best_action
        else: 
            return random.choice(possible_actions)
    
    def generate_an_epsiode(self): 
        game = game_module.Game() 
        state  = game.reset() 
        is_terminal = False
        episode = []
        # Loop for each step of the episode. 
        while is_terminal == False : 
            action = self.get_epsilon_greedy_action( state ) 
            next_state , reward , is_terminal = game.step( action )
            episode.append((state, action, reward))
            state = next_state
        
        return episode
    
    def train_on_episode(self, episode):
        for i in range(len(episode) - 1): # each step as a starting point to count total reward
            remaining = episode[i:] # remaining steps
            g = 0 # total reward for that (state, action) pair
            for i in range (len(remaining)):# count total reward
                g += remaining[i][2] * self.gamma**i # reward * gamma^i
            curr_start_state = remaining[0][0]
            curr_start_action = remaining[0][1]
            if ( tuple( curr_start_state ) , tuple( curr_start_action ) ) not in self.q_table: # if not in q_table, add it
                self.q_table[( tuple( curr_start_state ) , tuple( curr_start_action ) )] = g # store total reward for that (state, action) pair
            else: # if in q_table, update it
                #next_state = remaining[1][0]
                #next_action = remaining[1][1]
                next_state = remaining[1][0]
                next_action = self.get_epsilon_greedy_action( next_state )

                self.q_table[( tuple( curr_start_state ) , tuple( curr_start_action ) )] += \
                 self.alpha * (g + self.gamma * self.q_table[( tuple( next_state ) , tuple( next_action ) )] - self.q_table[( tuple( curr_start_state ) , tuple( curr_start_action ) )])
    
    def save_q_table_pickle(self, filename):
        import pickle
        with open(filename, 'wb') as f:
            pickle.dump(self.q_table, f)
    

        

                

if __name__ == "__main__":
    agent = evmc_agent()
    counter = 0
    while True:
        episode = agent.generate_an_epsiode()
        agent.train_on_episode(episode)
        counter += 1
        if counter % 100 == 0:
            print(f"counter={counter}")
            agent.save_q_table_pickle(f"q_table{counter}.pkl")
        if counter == 100000:
            break
