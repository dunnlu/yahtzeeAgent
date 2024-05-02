import game_module  # Make sure this matches the name of your module


def test_1() : 
    """ 
    1. If all dices are initialized to keep, the first 3 states have the same values for the dice. 
    """ 
    # Create an instance of the Game class
    game = game_module.Game() 
    # Print the results of the dice roll
    # game.printDice() 
    state_0  = game.reset() 
    # print( state_0 ) 
    state_1 , reward_1 , terminal_1 = game.step( [ 1 , 1 , 1 , 1 , 1  , - 1 ] ) 
    # print( state_1 , reward_1 , terminal_1 ) 
    state_2 , reward_2 , terminal_2 = game.step( [ 1 , 1 , 1 , 1 , 1 , - 1 ] ) 
    # print( state_2 , reward_2 , terminal_2 ) 

    if state_0[:5] == state_1[:5] == state_2[:5] : 
        print( "----------------------------- Test 1 has passed. ---------------------------- " ) 
    else : 
        print( "----------------------------- Test 1 has failed. ---------------------------- " ) 


def test_2() : 
    """ 
    1. If a dice is set to not keep, not all of the dice values are the same for the first 2 states. 
    """ 
    # Create an instance of the Game class
    game = game_module.Game() 
    # Print the results of the dice roll
    # game.printDice() 
    state_0  = game.reset() 
    # print( state_0 ) 
    state_1 , reward_1 , terminal_1 = game.step( [ 0 , 0 , 0 , 0 , 0 , - 1 ] ) 
    # print( state_1 , reward_1 , terminal_1 ) 
    state_2 , reward_2 , terminal_2 = game.step( [ 0 , 0 , 0 , 0 , 0 , - 1 ] ) 
    # print( state_2 , reward_2 , terminal_2 ) 

    test_pass = False 
    for i in range( 5 ) : 
        if state_0[ i ] != state_1[ i ] : 
            test_pass = True 
        if state_1[ i ] != state_2[ i ] : 
            test_pass = True 
        if state_0[ i ] != state_2[ i ] : 
            test_pass = True 
            
    if test_pass == True: 
        print( "----------------------------- Test 2 has passed. ---------------------------- " ) 
    else : 
        print( "----------------------------- Test 2 has failed. ---------------------------- " ) 

def test_3() : 
    """ 
    1. If a dice is rolled, the remaining dice is decremented. 
    """ 
    # Create an instance of the Game class
    game = game_module.Game() 
    # Print the results of the dice roll
    # game.printDice() 
    state_0  = game.reset() 
    # print( state_0 ) 
    state_1 , reward_1 , terminal_1 = game.step( [ 0 , 0 , 0 , 0 , 0 , - 1 ] ) 
    # print( state_1 , reward_1 , terminal_1 ) 
    state_2 , reward_2 , terminal_2 = game.step( [ 0 , 0 , 0 , 0 , 0 , - 1 ] ) 
    # print( state_2 , reward_2 , terminal_2 ) 

    test_pass = False 
    if state_0[ 5 ] == 2 and state_1[ 5 ] == 1 and state_2[ 5 ] == 0 : 
        test_pass = True 

    if test_pass == True : 
        print( "----------------------------- Test 3 has passed. ---------------------------- " ) 

    else : 
        print( "----------------------------- Test 3 has failed. ---------------------------- " ) 

def test_4() : 
    """
    1. If action is not to store while the ramining rolls is equal to 0, get a negative reward. (same state) 
    """ 
    # Create an instance of the Game class
    game = game_module.Game() 
    # Print the results of the dice roll
    # game.printDice() 
    state_0  = game.reset() 
    # print( state_0 ) 
    state_1 , reward_1 , terminal_1 = game.step( [ 0 , 0 , 0 , 0 , 0 , - 1 ] ) 
    # print( state_1 , reward_1 , terminal_1 ) 
    state_2 , reward_2 , terminal_2 = game.step( [ 0 , 0 , 0 , 0 , 0 , - 1 ] ) 
    # print( state_2 , reward_2 , terminal_2 ) 
    # Expect to get negative reward. (same state) 
    state_3 , reward_3 , terminal_3 = game.step( [ 0 , 0 , 0 , 0 , 0 , - 1 ] ) 
    # Expect to get negative reward. (same state) 
    state_4 , reward_4 , terminal_4 = game.step( [ 0 , 0 , 0 , 0 , 0 , - 1 ] ) 
    # Expect to get negative reward. (same state) 
    state_5 , reward_5 , terminal_5 = game.step( [ 0 , 0 , 0 , 0 , 0 , - 1 ] ) 
    # Expect to get negative reward. (same state) 
    state_6 , reward_6 , terminal_6 = game.step( [ 0 , 0 , 0 , 0 , 0 , - 1 ] ) 
    # Expect to get negative reward. (same state) 
    state_7 , reward_7 , terminal_7 = game.step( [ 0 , 0 , 0 , 0 , 0 , - 1 ] ) 

    if ( state_2 == state_3 == state_4 == state_5 == state_5 == state_6 == state_7 ) and ( reward_3 == reward_4 == reward_5 == reward_6 == reward_7 == -10) : 
        print( "----------------------------- Test 4 has passed. ---------------------------- " ) 
    else : 
        print( "----------------------------- Test 4 has failed. ---------------------------- " ) 




def main(): 
    test_1() 
    test_2() 
    test_3() 
    test_4() 

    


    
if __name__ == "__main__":
    main()

