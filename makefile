all:
	@echo "Building Chess!"
	g++ -g -Wall src/main.cpp src/init.cpp src/hashkey.cpp src/bitboard.cpp src/board.cpp src/pieces.cpp src/threats.cpp -o bin/chess

clean:
	@echo "~All Clean~"
	rm chess
