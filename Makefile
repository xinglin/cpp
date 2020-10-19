default: atomic pipeline sequential

clean:
	rm atomic sequential pipeline hashmap

atomic:
	g++ -std=c++11 -o atomic atomic.cpp
pipeline:
	g++ -std=c++11 -o pipeline pipeline.cpp
sequential:
	g++ -std=c++11 -o sequential sequential-children-execution.cpp 
hashmap:
	g++ -std=c++11 -o concurrenthashhap ConcurrentHashMap.cpp
