all:
	@echo "Building Chess!"
	g++ -g -Wall main.cpp init.cpp bitboard.cpp -o chess

clean:
	@echo "~All Clean~"
	rm chess
