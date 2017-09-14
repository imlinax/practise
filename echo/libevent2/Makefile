CFLAGS +=-I./libevent2-dist/include
CXXFLAGS +=-I./libevent2-dist/include
LDFLAGS +=-L./libevent2-dist/lib
LIBS=-levent

echo:echo.cpp 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LIBS) -o $@
