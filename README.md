## Mister Queen

Mr. Queen is a chess engine written in C.

#### Features

* Bitboards and "magic" bitboards for board representation and super-fast move
  generation.
* Custom, hash-based opening book based on over three million games from the
  FICS database.
* Zobrist hashing and a transposition table storing exact values and the best
  move.
* Null move pruning, aspiration windows, iterative deepening, quiescence
  search.
* Supports the Universal Chess Interface (UCI).
* Passes all perft tests.

#### Limitations

* Unaware of draw by repetition.
* Basic evaluation includes only material and piece-square values.

#### Usage
* Use 'make' to build the program as is. This program gives you access to the UCI implementation. 
* When implementing your own programs, you can create objects of the different parts.
..* For example, if you need to create a board you can use 'Board board;'
..* Then you can use the functions in board.c such as 'board_print(&boar);' 

---
##Examples

###2PlayerNoFeatures
As the name suggests, this implemntation is of a chess game with 2 players but with absolutely no features. That means, the users have to account for everything including castling, checkmates, promotions, captures etc.

It is inteded to give you a basic understanding of how to use the library in an actual program. 

####Usage
Navigate into the examples folder. Then, in your command line, use the 'make' command to build the program. 

<p align="center"><img src="http://i.imgur.com/oiWiEvK.png"></p>
