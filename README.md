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

---

<p align="center"><img src="http://i.imgur.com/oiWiEvK.png"></p>
