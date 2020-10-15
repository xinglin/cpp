* pipeline.cpp

A sample program, to demonstrate how to build a three-stage data processing pipeline, 
using mutex and condition variables. Each stage could have multiple threads for concurrent
processing. 
 
Data (chunks) flows from file_read_thread to compression threads to encryption threads.

* sequential-children-execution.cpp

Create a pool of 10 threads, with each thread executing sequentially based on its order(id).