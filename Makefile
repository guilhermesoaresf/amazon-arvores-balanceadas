CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2
all: encomendas
encomendas: src/main.cpp src/arvores.hpp
	$(CXX) $(CXXFLAGS) src/main.cpp -o encomendas
testes: tests/testes.cpp src/arvores.hpp
	$(CXX) $(CXXFLAGS) tests/testes.cpp -o testes
test: testes
	./testes
clean:
	rm -f encomendas testes
.PHONY: all test clean
