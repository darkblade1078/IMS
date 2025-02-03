SRCS	= $(wildcard *.cpp) $(wildcard ./code/*.cpp)
OBJS	= $(addprefix objects/,$(notdir $(SRCS:.cpp=.o)))
HEADER	= ./headers/*.h
OUT	= inventory.out
FLAGS	= -g -c -Wall
LFLAGS	= -lsqlite3
CC	= g++

all:	objects inventory.out

objects:
	mkdir -p objects

inventory.out: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

objects/%.o: %.cpp $(HEADER)
	$(CC) $(FLAGS) $< -o $@

objects/%.o: code/%.cpp $(HEADER)
	$(CC) $(FLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(OUT)
	rm -rf objects