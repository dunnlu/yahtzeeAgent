import game_module as game_module
import random
import pickle
class evmc_agent:
    def __init__(self, epsilon = 0.5, gamma = 0.99): 
        self.epsilon = epsilon
        self.gamma = gamma
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
    
    # Function to get the possible actions for a given state.
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
    
    # Function to get the epsilon greedy action for a given state.(uses self.epsilon)
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

        if random.random() > self.epsilon: 
            return best_action
        else: 
            return random.choice(possible_actions)
    
    # Function to generate an episode (lower section only). [(state, action, reward), ...]
    def generate_an_epsiode(self): 
        game = game_module.Game() 
        state  = game.reset()
        bottom_only_state = state[0:5] + [2,1,1,1,1,1,1,0,0,0,0,0,0,0]
        state = bottom_only_state
        game.goToState(bottom_only_state)
        is_terminal = False
        episode = []
        # Loop for each step of the episode. 
        while is_terminal == False : 
            action = self.get_epsilon_greedy_action( state ) 
            next_state , reward , is_terminal = game.step( action )
            episode.append((state, action, reward))
            state = next_state
        
        return episode
    
    # Function to update Q(s,a) thoughout an epsiode.
    def train_on_episode(self, episode):
        for i in range(len(episode) - 1): # each step as a starting point to count total reward
            remaining = episode[i:] # remaining steps
            g = 0 # total reward for that (state, action) pair
            for i in range (len(remaining)):# count total reward
                g += remaining[i][2] * self.gamma**i # reward * gamma^i
            curr_start_state = remaining[0][0]
            curr_start_action = remaining[0][1]

            self.q_table[( tuple( curr_start_state ) , tuple( curr_start_action ) )] = g # store total reward for that (state, action) pair
            
    # Function to save the q_table to a pickle file.
    def save_q_table_pickle(self, filename):
        with open(filename, 'wb') as f:
            pickle.dump(self.q_table, f)
    
    # Function to load the q_table from a pickle file.
    def load_q_table_pickle(self, filename):
        with open(filename, 'rb') as f:
            self.q_table = pickle.load(f)
    
if __name__ == "__main__":
    num_episodes = 1_000_000
    epsilons = [0.8, 0.9, 1]
    gammas = [0.6, 0.65, 0.7]

    for epsilon in epsilons:
        for gamma in gammas:
            agent = evmc_agent(epsilon=epsilon, gamma=gamma)
            agent.q_table = {}
            game = game_module.Game()
            for i in range(num_episodes):
                episode = agent.generate_an_epsiode()
                agent.train_on_episode(episode)
                if i % 1000 == 0:
                    print(f"Episode {i} completed")
            print("epsilon: ", epsilon, ", gamma:", gamma, ", num_episodes:", num_episodes, "\n")

            exploit_final_scores = []
            agent.epsilon = 0
            for _ in range(1000):
                state  = game.reset()
                bottom_only_state = state[0:5] + [2,1,1,1,1,1,1,0,0,0,0,0,0,0]
                state = bottom_only_state
                game.goToState(bottom_only_state)
                is_terminal = False
                total_score = 0

                while is_terminal == False:
                    action = agent.get_epsilon_greedy_action(state)
                    next_state, reward, is_terminal = game.step(action)
                    total_score += reward
                    state = next_state
                exploit_final_scores.append(total_score)
            avg_exploit_score = sum(exploit_final_scores) / len(exploit_final_scores)
            print(f"Average Exploit Score: {avg_exploit_score}")
            with open("final_test_out.csv", "a") as f:
                f.write(f"{epsilon},{gamma},{avg_exploit_score}\n")
            agent.save_q_table_pickle(f"q_table_epsilon_{epsilon}_gamma_{gamma}_{num_episodes}.pkl")

