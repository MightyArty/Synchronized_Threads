CXXFLAGS=-std=$(CXXVERSION) -Werror -Wsign-conversion -I$(SOURCE_PATH)
CXX=g++
CXXVERSION=c++2a
SOURCE_PATH=.
SOURCES=$(wildcard $(SOURCE_PATH)/*.cpp)
HEADERS=$(wildcard $(SOURCE_PATH)/*.hpp)
TIDY_FLAGS=-extra-arg=-std=$(CXXVERSION) -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory --warnings-as-errors=*
VALGRIND_FLAGS=-v --leak-check=full --show-leak-kinds=all  --error-exitcode=99
SOURCES=$(wildcard $(SOURCE_PATH)/*.cpp)
OBJECTS=$(subst .cpp,.o,$(SOURCES))
# run: iclient server test
run: iclient server test

iclient:iclient.o
	$(CXX) $(CXXFLAGS) $^ -o $@

server:server.o
	$(CXX) $(CXXFLAGS) $^ -o $@ -pthread

test: TestCounter.o Test.o 
	$(CXX) $(CXXFLAGS) $^ -o test -pthread

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) --compile $< -o $@

$(OBJECT_PATH)/%.o: $(SOURCE_PATH)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) --compile $< -o $@

tidy:
	clang-tidy $(SOURCES) $(TIDY_FLAGS) --

valgrind: server
	valgrind --tool=memcheck $(VALGRIND_FLAGS) ./server 

clean:
	rm -f $(OBJECTS) *.o iclient server test
	
client:
	./iclient

iserver:
	./server