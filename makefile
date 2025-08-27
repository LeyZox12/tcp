CXXFLAGS = -std=c++23 -g -I"../../../SFML-3.0.0/include" -Iinclude -I"../../class"

final : main.o Card.o
	g++ $(CXXFLAGS) Card.o main.o -o poker -L"../../../SFML-3.0.0/lib" -lsfml-graphics-d -lsfml-window-d -lsfml-network-d -lsfml-system-d

main.o : main.cpp
	g++ $(CXXFLAGS) -c main.cpp 

Card.o : src/Card.cpp
	g++ $(CXXFLAGS) -c "src/Card.cpp"

clean : main.o
	del main.o


