output : loadBalancer.o worker.o presenter.o
	g++ loadBalancer.o  -o loadBalancer
	g++ worker.o  -o worker
	g++ presenter.o  -o presenter

loadBalancer.o:loadBalancer.cpp 
	g++ -c loadBalancer.cpp
client.o:worker.cpp 
	g++ -c worker.cpp
presenter.o:presenter.cpp 
	g++ -c presenter.cpp

