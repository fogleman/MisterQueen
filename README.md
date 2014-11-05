## Chess

Over ten years ago, I wrote my own chess engine. It was really fast
(performance-wise) but it didn't play well because the search and evaluation
techniques were not very good.

Recently, my coworkers and I have been playing some chess, and I decided to
try writing a chess engine again to see if I can do better this time.

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
