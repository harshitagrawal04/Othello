# Roadmap — Phase 3 (AI design & implementation), detailed & educational

---

## Phase 3.0 — Setup & API contract (very small)

**Goal:** define the AI interface so the rest of the engine can call bots consistently.

### Tasks

* Design a minimal `AIEngine` interface:

  ```cpp
  enum class AIDifficulty { Easy, Medium, Hard };

  struct AIParams {
    int max_depth;               // search depth
    double randomness;           // 0..1 (for Easy/Medium)
    vector<double> weights;      // heuristic weights (positional, mobility, etc.)
  };

  class AIEngine {
  public:
    AIEngine(AIDifficulty difficulty, const AIParams& params);
    Move choose_move(const CellState board[BOARD_SIZE][BOARD_SIZE], CellState player);
  private:
    AIParams params_;
    // any caches, transposition tables, rng, etc.
  };
  ```

### Deliverable

* Header file with `AIEngine` class and method signatures implemented as stubs.

### Why

* Keeps UI/game loop stable while you iterate AI internals.
* Forces you to think about parameters and persistence early.

---

## Phase 3.1 — Foundations: Trees, Minimax concept, and recursion

**Goal:** understand game-tree search fundamentals before coding minimax.

### Learn

* What a game tree is: nodes = game states, edges = legal moves.
* Turn-taking: maximizing player vs minimizing player.
* Depth-limited search (why we limit depth).
* Terminal states and utility (evaluation) functions.

### Exercises (paper + code)

1. **Paper exercise:** draw a small tree for Tic-Tac-Toe up to depth 2 and manually apply minimax values.
2. **Code exercise:** write a tiny recursive function (separate toy program) that generates all move sequences for depth N for a simplified 3×3 board (no heuristics). Just return number of leaf nodes. Verify branching counts.

### Deliverable

* Short document or comments explaining minimax, plus the tiny test program that counts leaf nodes for small depth.

### Why

* If recursion/traversal isn’t clear it will be much harder once alpha-beta and move-ordering are introduced.

---

## Phase 3.2 — Easy AI: Random (with optional weighting)

**Goal:** implement a baseline bot you can play against immediately.

### Learn

* Random selection, seeding RNG.
* Simple heuristics (corner bias): corners are extremely valuable in Othello.

### Implement

* `Move choose_move_random(const unordered_map<Move, vector<Move>>& legal_moves, double randomness, rng)`

    * If `randomness==1.0` pick uniform random.
    * If `randomness < 1.0` pick weighted: e.g., if corner present, with probability `p` pick corner; else random.
* Hook into `AIEngine` for `AIDifficulty::Easy`.

### Small tests

* Play several games vs random bot and count wins; check that with a corner bias the bot selects corners more often.

### Deliverable

* Working `Easy` mode in the game loop (play vs bot).

---

## Phase 3.3 — Heuristics design (essential for Minimax)

**Goal:** design the evaluation function — the heart of a shallow search.

### Learn & Think

Design a heuristic consisting of combineable features:

1. **Piece difference:** `#my - #opp` (simple but noisy).
2. **Mobility:** number of legal moves (current player mobility - opponent mobility).
3. **Corner occupancy:** corners owned.
4. **Corner closeness / X-squares risk:** penalty for squares adjacent to corner when corner empty.
5. **Stability / frontier discs** (advanced): discs that cannot be flipped.
6. **Positional weight matrix:** static weights per square (classic Othello weight table — corners +100, bad squares -50, edges +10, interior small values).

### Implement

* Create `Evaluation` module:

  ```cpp
  double evaluate(const CellState board[BOARD_SIZE][BOARD_SIZE], CellState player, const vector<double>& weights);
  ```

    * `weights` map to features: e.g., `[w_piece_diff, w_mobility, w_corner, w_positional]`.
    * Implement the features modularly so you can tune/test weights easily.

### Exercises

* Print evaluation values for several board positions, e.g., initial board, a hand-crafted midgame position, corner captured vs not.

### Deliverable

* `evaluate(...)` function fully documented, with a set of default weights (tweakable).

---

## Phase 3.4 — Medium AI: Minimax (depth-limited) with static heuristic

**Goal:** implement basic minimax recursion, alternate players, depth limit, and integrate `evaluate()`.

### Learn

* Minimax recursion pattern.
* Depth-limited evaluation: when depth==0 or terminal state → call `evaluate()`.

### Implement (step-by-step)

1. **Pseudocode (to implement yourself):**

   ```
   function minimax(board, player, depth):
       if depth == 0 or terminal(board):
           return evaluate(board, player)
       if player == maximizing_player:
           best = -INF
           for each move in legal_moves(board, player):
               apply_move -> new_board
               val = minimax(new_board, opponent(player), depth - 1)
               best = max(best, val)
           return best
       else:
           best = +INF
           for move in legal_moves:
               ...
               best = min(best, val)
           return best
   ```

2. Implement `minimax_value(board, player, depth, maximizing_player)` returning `(score, best_move)` or separate function to return best move.

3. Integrate into `AIEngine::choose_move` for `Medium` with shallow depth (e.g., depth = 3 or 4 plies). For Othello “ply” usually counts half-moves.

### Tests / Sanity checks

* Verify minimax returns the same move when trivial choices (single legal move).
* Small search count test: instrument node visit counts to make sure recursion depth behaves as expected.

### Deliverable

* `Medium` bot that uses depth-limited minimax and your evaluation function.
* Playable in CLI immediately.

---

## Phase 3.5 — Optimization 1: Alpha-Beta pruning

**Goal:** drastically reduce nodes searched by pruning.

### Learn

* How alpha-beta maintains upper/lower bounds (alpha = best max so far, beta = best min so far).
* Why move ordering matters for pruning effectiveness.

### Implement

* Modify minimax to alpha-beta:

  ```cpp
  double alphabeta(board, depth, alpha, beta, maximizingPlayer)
  ```
* Ensure it still returns best move.

### Exercise

* Run a comparison: node visits with minimax vs alpha-beta for same depth and position. You should see fewer nodes with alpha-beta.

### Deliverable

* `Hard` bot uses alpha-beta at depth >= medium depth.

---

## Phase 3.6 — Optimization 2: Move ordering & iterative deepening

**Goal:** build components to make deeper searches practical.

### Learn

* Move ordering heuristics (try moves likely to be best first). Examples:

    * Order by heuristic evaluation of resulting state (shallow evaluate the child, sort descending).
    * Prioritize capturing corners.
* Iterative deepening: repeatedly run depth=1,2,3,... so you always have a best-move candidate if time runs out; also helps move-ordering (use best move from shallower depth to order next depth).
* Time limit handling: how to abort search gracefully (use a `stop_time` flag checked in recursion).

### Implement

* Add `iterative_deepening()` wrapper:

    * Loop `d = 1..max_depth`:

        * run alpha-beta with time check
        * save best move found
* Add simple move ordering: for each child generate quick `evaluate()` and sort children by descending score before recursion.

### Deliverable

* Iterative deepening loop in `Hard` bot (optionally with a time limit parameter).

---

## Phase 3.7 — Transposition Table (Zobrist hashing) — optional but powerful

**Goal:** cache evaluated positions to avoid re-evaluating identical board states reached via different move orders.

### Learn

* What Zobrist hashing is: XOR-combination of random 64-bit numbers assigned to (square, piece).
* Transposition table: `unordered_map<uint64_t, TTEntry>` storing depth, value, flag (exact/lowerbound/upperbound), best move.

### Implement (advanced)

1. Implement Zobrist table:

    * `uint64_t zobrist[BOARD_SIZE][BOARD_SIZE][2]` (two piece types).
    * Generate random 64-bit values at program start (use fixed seed for reproducibility).
2. `uint64_t compute_hash(board)` using XOR of occupied squares and possibly side-to-move random.
3. Add `unordered_map<uint64_t, TTEntry>` where `TTEntry` contains `(value, depth, flag, best_move)`.
4. Before exploring a node check the table; after computing store result.
5. Integrate with alpha-beta and iterative deepening.

### Exercises

* Measure speedup on mid-game positions.

### Deliverable

* Transposition table integrated and working, configurable through `AIParams`.

---

## Phase 3.8 — Putting it together: AIEngine, difficulty parameters, and persistence

**Goal:** parameterize and persist learned/tuned weights and engine configuration.

### Tasks

* `AIParams` should include:

    * `max_depth`, `time_limit_ms`, `randomness`, `positional_weights[]`, `use_zobrist`, `use_iterative_deepening`, `use_tt`.
* Implement functions to **serialize/deserialize** `AIParams` and positional weights to/from JSON or simple text:

    * e.g., `save_ai_params("ai_easy.json")` and `load_ai_params("ai_hard.json")`.
* Make `AIEngine` load weights at construction.

### Deliverable

* `AIEngine` config files and load/save functions so you can store tuning results and reuse them.

---

## Phase 3.9 — Self-play testing and evaluation metrics

**Goal:** systematically check that difficulty levels behave differently and improve via tuning.

### Experiments to run

1. **Random vs Random** baseline test (100 games).
2. **Easy vs Medium**, **Medium vs Hard** (100+ games each), track win/draw/loss stats.
3. **ELO-like measurement** to see differences.
4. **Ablation tests**: turn off mobility or corner in the heuristic and measure impact.

### Metrics to record

* Win percentage, average search nodes per move, average move time, distribution of depths reached (for iterative deepening).

### Deliverable

* Small script or program to run matches (AI vs AI) and log results to CSV. Use these results to tweak heuristic weights.

---

## Phase 3.10 — Final polish for Phase 3

**Goal:** integrate AI into game loop, ensure non-blocking UI and responsiveness.

### Tasks

* **CLI:** add a play-vs-bot command line option; let user choose difficulty.
* **Non-blocking:** if moving to UI later, run AI on a background thread, passing a copy of board to AI.
* **Safety:** limit AI time; if time exceeded, choose best move found so far (iterative deepening helps here).

### Deliverable

* A playable CLI where you can pick Easy / Medium / Hard and play against each. Optionally, background-threaded AI for later UI.

---
