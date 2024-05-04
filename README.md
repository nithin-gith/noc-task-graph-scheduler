# Scheduling Task Graph Application for Network on Chip based Heterogeneous Multiprocessor System on Chip

This repo consists of simulation of Network on Chip (NoC) in C++.
Simulation implemented is used for scheduling a application task graph, for exact algorithm used for scheduling please refer to the [CLS](./communication_aware_list_scheduling.pdf)


## Local Installation and setup
1. clone the repo
```
git clone https://github.com/nithin-gith/noc-task-graph-scheduler.git
cd noc-task-graph-scheduler
```
2. install the standard gcc c++ compiler and c/c++ extenstion by microsoft in vs code

3. set the system parameters in [globals](./globals.cpp) and compile the code
```
g++ --std=c++14 main.cpp globals.cpp -o main
```

4. run the compiled binary
```
./main
```