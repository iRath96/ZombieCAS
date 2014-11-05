ZOMBIE_CPP=$(wildcard Zombie/*/*.cpp) jsapi.cpp
ZOMBIE_O=$(ZOMBIE_CPP:.cpp=.o)

build: $(ZOMBIE_O)
	em++ -s ASSERTIONS=2 --bind -O2 $(ZOMBIE_O) -o Zombie.js

test: build
	node Zombie.js

clean:
	rm -f $(ZOMBIE_O)

%.o: %.cpp
	echo $? -> $@
	em++ -O2 $? -IZombie/Terms -IZombie/Definitions -IZombie/Parser -o $@ -std=c++0x