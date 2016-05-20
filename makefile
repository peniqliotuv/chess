all:
	@echo "Building Chess!"
	g++ -g -Wall main.cpp init.cpp hashkey.cpp bitboard.cpp board.cpp pieces.cpp -o chess

clean:
	@echo "~All Clean~"
	rm chess
