
## **Stage 1: Rule-based / Greedy AI (Baseline)**

**Purpose:** Have a working AI that can play legally but is not smart. This helps you **test your environment, reward functions, and step mechanics**.

* **Decision-making:**

    * Chooses a move randomly from legal moves
    * Or picks the move that flips the most opponent discs (greedy)

* **Components to implement:**

    * Legal move generator (already in `GameEngine`)
    * Reward function (number of flipped discs, win/loss)
    * Step function to apply moves and update state

* **Output:** Basic AI that plays valid Othello moves.

---

## **Stage 2: Tabular / Value-based AI (Intermediate)**

**Purpose:** Introduce simple **state evaluation**. You’re not using neural networks yet, but the AI can start learning **which moves are better** over time.

* **Approach:**

    * Maintain a **table of board states → value**

        * Can be simplified by encoding board as 64-length vector and storing only visited states
    * Use **Critic-like updates**:

        * After a move, update the value of the state using TD(0):

          ```text
          V(s) = V(s) + alpha * (reward + gamma * V(s') - V(s))
          ```
    * Action selection: greedy or ε-greedy policy

* **Components to implement:**

    * State hashing / encoding
    * Value table (map<state_vector, float>)
    * TD update rule
    * Policy (epsilon-greedy)

* **Output:** AI that **improves over games** and can beat Stage 1 AI consistently.

---

## **Stage 3: Actor-Critic Neural Network AI (Advanced / Full RL)**

**Purpose:** Full Actor-Critic RL implementation. The AI learns **both a policy (Actor)** and **a value function (Critic)** from scratch using neural networks.

* **Approach:**

    * Actor outputs action probabilities over all legal moves
    * Critic outputs estimated value of current state
    * Sample action according to Actor, apply move, receive reward, update both networks
    * Use advantage for Actor update:

      ```text
      advantage = reward + gamma * V(next_state) - V(current_state)
      ```
    * Use MSE loss for Critic

* **Components to implement:**

    * Neural network forward pass (Actor & Critic)
    * Backward pass / manual gradient computation
    * Weight & bias storage in files (for saving and loading models)
    * Gradient descent updates
    * Reward shaping and masking illegal moves
    * Training loop (episodes & self-play)

* **Output:** AI that **learns strategy, can beat Stages 1 & 2**, and improves over time.
