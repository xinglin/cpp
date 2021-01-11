default: atomic pipeline sequential timer binarysearch stringstream readerwriterlock

clean:
	rm atomic sequential pipeline hashmap timer binarysearch readerwriterlock

atomic:
	g++ -std=c++11 -o atomic atomic.cpp
pipeline:
	g++ -std=c++11 -o pipeline pipeline.cpp
sequential:
	g++ -std=c++11 -o sequential sequential-children-execution.cpp 
hashmap:
	g++ -std=c++11 -o concurrenthashhap ConcurrentHashMap.cpp
timer:
	g++ -std=c++11 -o timer timer.cpp

binarysearch:
	g++ -std=c++11 -o binarysearch binarysearch.cpp

stringstream:
	g++ -std=c++11 -o stringstream stringstream.cpp

readerwriterlock:
	g++ -std=c++11 -o readerwriterlock readerwriterlock.cpp

concurrentlinkedlist:
	g++ -std=c++11 -g -o concurrentlinkedlist concurrentlinkedlist.cpp