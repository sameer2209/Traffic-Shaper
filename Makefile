CC = g++

PTHREAD_LIBRARY_FLAG = -pthread
COMPILE_FLAG = -c

trafficshaper: trafficShaper.o inputParser.o request.o requestToken.o requestServer.o
	${CC} ${PTHREAD_LIBRARY_FLAG} trafficShaper.o inputParser.o request.o requestToken.o requestServer.o -o trafficshaper

trafficShaper.o: trafficShaper.cpp trafficShaper.h
	${CC} ${PTHREAD_LIBRARY_FLAG} ${COMPILE_FLAG} trafficShaper.cpp

inputParser.o: inputParser.cpp inputParser.h
	${CC} ${PTHREAD_LIBRARY_FLAG} ${COMPILE_FLAG} inputParser.cpp

request.o: request.cpp request.h
	${CC} ${PTHREAD_LIBRARY_FLAG} ${COMPILE_FLAG} request.cpp

requestToken.o: requestToken.cpp requestToken.h
	${CC} ${PTHREAD_LIBRARY_FLAG} ${COMPILE_FLAG} requestToken.cpp

requestServer.o: requestServer.cpp requestServer.h
	${CC} ${PTHREAD_LIBRARY_FLAG} ${COMPILE_FLAG} requestServer.cpp

clean:
	rm -rf *.o trafficshaper