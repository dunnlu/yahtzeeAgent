### State and Action Representation. 

1. We can always change and modify it, let's not worry about it currently, if different people want to use different state and action representations for their algorithms, we can write multiple step functions that can use different state-action representations. 

### Note: 

1. I think it make sense for everyone to have the option of choosing what language they can use to implement their assigned algorithm. Using the current game_bindings.cpp template, it is easy to create the bindings. 

2. Since there are 32 + X actions, if the remaining rolls is 0, the action space reduce by 32, therefore during certain states, we can decide between a subset of the 32 + X actions. 

