# Labyrinth: Fire and Aliens

Here implemented an interactive Labyrinth game, where the user should get out of the labyrinth 20x20, which may have one or two exits. The game should be represented as a Console Application, using ASCII characters to draw the labyrinth. "." is an empty space and "#" is a wall. Each turn, the player can move to one of the 4 neighboring cells if they are empty. Game should support 2 modes:
1. User vs Fire - When the game starts, there should also be 1 to 3 cells on fire ("@"). After each turn, fires spread to neighboring empty cells if they are not already on fire. If a player escapes the Labyrinth without stepping into the fire, they win. Otherwise the game is considered a loss.
2. User vs Aliens - When the game starts, there should also be 3 to 5 aliens("&") on the labyrinth cells. After each player's move, each alien choses the shortest path to the player and moves one cell in that direction. If there are multiple shortest paths, the alien gives priority to the left cell, then the top cell, then the right cell and only then to the bottom cell.

General Requirements:
1. Each initial state of a game should be "valid". Game is considered "valid" if:
  1. The game is winnable.
  2. The fastest winning strategy contains at least 5 moves.
  3. The game is losable, i.e. If the player is just moving around, they will either be burnt by fire or catched by aliens at some point.
2. When a player loses, the game should provide a "Show winning moves" option. When choosing it, the game should simulate any winning strategy. showing the moves one by one.
