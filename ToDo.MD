### To Do: 

1. Create a function, called step, that can take in an action, and return the s', R(s,a), and done. (1 if it is the terminal state, 0 if it is not the terminal state) 
2. Create a function, called render, that can render the required information using the terminal, the reason for this is so that we can call this (from the step function) only in the final evaluation, and not in the training. (save the compute) 
3. Create a function, called reset, that can reset the game to the initial state, and then return the initial state. 

### State and Action Representation. 

1. We can always change and modify it, let's not worry about it currently, if different people want to use different state and action representations for their algorithms, we can write multiple step functions that can use different state-action representations. 

### Note: 

1. I think it make sense for everyne to have the option of choosing what language they can use to implement their assigned algorithm. Using the current game_bindings.cpp template, it is easy to create the bindings. 

2. Since there are 32 + X actions, if the remaining rolls is 0, the action space reduce by 32, therefore during certain states, we can decide between a subset of the 32 + X actions. 

