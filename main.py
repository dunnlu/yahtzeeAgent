import game_module  # Make sure this matches the name of your module

def main():
    # Create an instance of the Game class
    game = game_module.Game()
    
    # Roll the dice
    game.rollDice()
    
    # Print the results of the dice roll
    game.printDice()

if __name__ == "__main__":
    main()
