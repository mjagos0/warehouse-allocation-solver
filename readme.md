# Warehouse Allocation Solver

## Overview
The **Warehouse Allocation Solver** is a solver written in C++ designed to solve the **Warehouse Allocation Problem**. It implements several optimization algorithms, including **Local Search**, **Evolutionary Algorithm**, and **Memetic Algorithm**, to efficiently allocate warehouses to customers while minimizing costs.

## Warehouse Allocation Problem
The Warehouse Allocation Problem involves assigning geographically distributed customers to warehouses in such a way that the total cost, including warehouse setup costs and transportation costs, is minimized. Each warehouse has a limited capacity, and each customer has a specific demand.
- $N$ warehouses, each w arehouse $w$ has capacity $cap_w$ and set-up cost $s_w$
- $M$ customers, each customer $c$ demands certain amount of goods $d_c$
- For each pair $<c,w>$ a cost $t_{cw}$ for delivering the goods from $w$ to $c$ is defined.

## Methodology
[Read paper](documentation/warehouse-allocation.pdf)

## Prerequisites

## Usage
1. Clone the repository:
   ```bash
   git clone https://github.com/mjagos0/warehouse-allocation-solver.git
   cd warehouse-allocation-solver
   ```
2. Compile
    ```bash
    g++ src/main.cpp -o main.o
    ```

3. Run and feed input file
    ```bash
    ./main.o < inputs/wl_2000_1.in
    ```

## Configuring parameters
Access the main.cpp.
- To update which solver (Local Search, Evolutionary Algorithm, Memetic Algorithm) is used, update the APPROACH variable appropriately
- To update parameters, change values in GlobalConfig, EvolutionaryAlgorithmConfig and MemeticAlgorithmConfig


## License
This project is licensed under the MIT License.

## Author
**Marek Jagoš** (jagos.marek@outlook.cz)