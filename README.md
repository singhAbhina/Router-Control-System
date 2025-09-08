# Router-Control-System


## 📄 Overview

This project simulates a **Router Network Control System** that allows users to manage and analyze a network of routers. It provides functionalities such as viewing network topology, computing shortest paths, finding the Minimum Spanning Tree (MST), detecting critical connections, adding/removing routers, and displaying routing tables.

The program is built using **C++** and demonstrates the application of graph algorithms such as **Dijkstra's algorithm**, **Prim's algorithm**, and **bridge-finding algorithms** in the context of network routing.

All outputs are saved in `network_output.txt`.

---

## 🟠 Features

✔ Display the current network topology  
✔ Compute shortest paths from a given router using **Dijkstra’s algorithm**  
✔ Find the Minimum Spanning Tree using **Prim’s algorithm**  
✔ Detect critical connections (bridges) that impact network connectivity  
✔ Add a new router dynamically with IP and MAC address generation  
✔ Remove an existing router safely, updating the network structure  
✔ Display routing tables showing next hops for each router  
✔ User-friendly command-line control panel interface  

---

## 📦 Dependencies

- C++ Standard Library (`<iostream>`, `<vector>`, `<queue>`, `<climits>`, `<algorithm>`, `<fstream>`, etc.)
- Works with any modern C++ compiler (supports C++11 or later)

---

## 📂 Files

- `main.cpp` – The source code implementing the router network system
- `network_output.txt` – File where the program logs all outputs and results

---

## 🚀 How to Run

1. Compile the code using `g++` or another C++ compiler:

   ```bash
   g++ -std=c++11 main.cpp -o router_network
   
