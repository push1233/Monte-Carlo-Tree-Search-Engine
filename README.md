# 🚀 Lock-Free Parallel Monte Carlo Tree Search Engine

A high-performance Monte Carlo Tree Search (MCTS) engine written in C++ with lock-free tree parallelization, atomic synchronization, custom memory allocators, and a full-stack interactive Tic-Tac-Toe application powered by React, Node.js, and Socket.IO.

This project explores the intersection of:

- Concurrent systems programming
- Game tree search algorithms
- Lock-free data structures
- Performance engineering
- Cross-language software architecture

---

# Demo Architecture

```text
React Frontend
       │
       ▼
 Socket.IO
       │
       ▼
 Node.js Bridge
       │
 stdin/stdout IPC
       ▼
 Parallel MCTS Engine (C++)
       │
       ├── UCT Selection
       ├── Lock-Free Expansion
       ├── Rollout Simulation
       ├── Atomic Backpropagation
       ├── Virtual Loss
       └── Thread-Local Allocators
```

---

# Features

## Parallel Monte Carlo Tree Search

Implements the complete MCTS pipeline:

1. Selection (UCT)
2. Expansion
3. Simulation
4. Backpropagation

Multiple worker threads simultaneously explore and update the same search tree.

---

## Lock-Free Tree Parallelization

Instead of protecting the tree with global mutexes, node statistics are updated using atomic operations:

```cpp
std::atomic<int> v;
std::atomic<double> s;
std::atomic<int> vl;
```

This allows multiple threads to traverse and update the search tree concurrently with minimal synchronization overhead.

---

## Virtual Loss

To reduce contention during tree traversal, the engine applies virtual loss while a thread is exploring a node.

This discourages other threads from repeatedly selecting the same path and improves exploration diversity.

---

## Cache-Aware Node Layout

Nodes are aligned to cache lines:

```cpp
struct alignas(64) Nd
```

This reduces false sharing between threads and improves scalability under contention.

---

## Custom Thread-Local Memory Allocator

The engine avoids allocator contention by using per-thread bump allocators.

```cpp
Ar<1024 * 1024 * 64> ta[9];
```

Benefits:

- Reduced heap allocations
- Improved memory locality
- Lower synchronization overhead

---

## Zobrist Hashing

The project includes Zobrist hashing utilities for efficient board state hashing and future transposition table support.

```cpp
uint64_t ch(int b[64]);
uint64_t uh(...);
```

---

## Full Stack Application

The search engine is integrated into a playable application.

### Frontend

- React
- Socket.IO

### Backend Bridge

- Node.js
- Express
- Child Process IPC

### Engine

- C++20
- Multi-threaded MCTS

---

# Search Algorithm

The engine uses the standard Upper Confidence Bound for Trees (UCT) policy:

```text
UCT = (W / N) + C * sqrt(ln(Np) / N)
```

Where:

- W = accumulated score
- N = node visits
- Np = parent visits
- C = exploration constant

This balances:

- Exploration
- Exploitation

during tree traversal.

---

# Concurrency Design

The core challenge of parallel MCTS is that many simulations may attempt to:

- Traverse the same node
- Expand the same node
- Update the same statistics

simultaneously.

This engine addresses those issues using:

- Atomic counters
- Compare-and-swap expansion
- Virtual loss
- Thread-local allocators
- Cache-aware node alignment

without relying on global locks.

---

# Project Structure

```text
.
├── include/
│   └── mcts.h
│
├── src/
│   ├── mcts.cpp
│   └── main.cpp
│
├── frontend/
│   ├── App.jsx
│   └── App.css
│
├── server/
│   └── srv.js
│
└── README.md
```

---

# Build

## C++ Engine

### Using CMake

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Using g++

```bash
g++ -O3 -std=c++20 -pthread src/main.cpp src/mcts.cpp -o mcts_engine
```

---

## Node Server

```bash
npm install
node srv.js
```

---

## React Frontend

```bash
npm install
npm run dev
```

---

# Example Gameplay Flow

1. User clicks a square in the React UI
2. Board state is sent through Socket.IO
3. Node.js forwards the state to the C++ engine
4. MCTS searches using multiple threads
5. Best move is returned
6. React updates the board

---

# Technical Highlights

- Lock-free concurrent tree search
- Atomic node statistics
- Virtual loss implementation
- Custom memory allocator
- Cache-line aligned nodes
- Zobrist hashing
- Cross-language IPC architecture
- Full-stack deployment

---

# Future Improvements

- Transposition table integration
- Tree reuse between moves
- Connect-4 implementation
- Chess support
- SIMD rollout optimization
- NUMA-aware allocation
- Work-stealing schedulers
- Neural-network-guided rollouts

---

# Why This Project Matters

Monte Carlo Tree Search is used in:

- AlphaGo
- AlphaZero
- Game engines
- Robotics planning
- Decision-making systems

This project focuses not only on implementing the algorithm, but also on solving the systems-level challenges involved in making it run efficiently in parallel.

---

# License

MIT License
