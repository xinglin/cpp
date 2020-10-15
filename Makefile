pipeline:
	g++ -std=c++11 -o pipeline pipeline.cpp
sequential:
	g++ -std=c++11 -o sequential sequential-children-execution.cpp 
clean:
	rm sequential pipeline