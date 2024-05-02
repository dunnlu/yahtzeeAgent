import game_module  # Make sure this matches the name of your module

def main():
    # Create an instance of the Game class
    game = game_module.Game()
    
    # Print the results of the dice roll
    game.printDice() 

    return_value  = game.reset() 
    print(return_value ) 

if __name__ == "__main__":
    main()

