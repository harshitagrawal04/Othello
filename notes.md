### 🗺️ Tabular RL Theory Roadmap

Here is the structured roadmap we will follow. Each numbered item is a main concept, and the bullet points are the small topics we will cover one at a time.

1.  **Introduction to Reinforcement Learning (RL) Basics**
    * The RL Framework: Agent, Environment, States, Actions, and Rewards.
    * The Goal of the Agent: Maximizing Expected Cumulative Reward.
    * The Concept of **Policy** ($\pi$).

2.  **The Markov Decision Process (MDP)**
    * What is an MDP? The Markov Property.
    * **State Space** ($\mathcal{S}$) and **Action Space** ($\mathcal{A}$).
    * Transition Probabilities ($P$) and Reward Function ($R$).
    * The Role of the **Discount Factor** ($\gamma$).

3.  **Value Functions**
    * The **State-Value Function** ($V(s)$): How good is a state?
    * The **Action-Value Function** ($Q(s, a)$): How good is an action in a state?
    * The Relationship between $V(s)$ and $Q(s, a)$.

4.  **The Bellman Equations**
    * Bellman Expectation Equation (for a fixed policy $\pi$).
    * Bellman Optimality Equation (for the optimal value functions $V^*$ and $Q^*$).

5.  **Tabular Solution Methods (Policy Evaluation)**
    * **Dynamic Programming (DP):** Policy Iteration and Value Iteration.
    * The need for **Tabular** methods (small state/action spaces).

6.  **Tabular Solution Methods (Control: Learning the Optimal Policy)**
    * **Monte Carlo (MC) Learning:** Basic idea and concepts (Returns).
    * **Temporal-Difference (TD) Learning:** The core idea of "bootstrapping."
    * **$TD(0)$** and the **TD Target**.

7.  **On-Policy and Off-Policy Control**
    * **Sarsa (On-Policy TD Control):** The update rule.
    * **Q-Learning (Off-Policy TD Control):** The update rule and its significance.

8.  **Exploration vs. Exploitation**
    * The Trade-off and its importance in RL.
    * $\epsilon$-**Greedy** strategy.

***