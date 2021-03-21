CXX=g++
CXXFLAGS += -g -Wall -Wextra -pthread
CPPFLAGS += -isystem src -std=c++11

MKDIR_P = mkdir -p
OBJ_DIR = obj

all: threading threading_lock

${OBJ_DIR}:
	${MKDIR_P} ${OBJ_DIR}

obj/%.o: %.cpp ${OBJ_DIR}
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

threads: obj/threading.o
	$(CXX) -o $@ $^

threads_2: obj/threading_lock.o
	$(CXX) -o $@ $^

clean:
	rm -f *~ obj/*.o *.zip
	rm -f threading threading_lock
