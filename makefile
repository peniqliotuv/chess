all:
	@echo "Building Chess!"
	g++ -g -std=c++11 -Wall src/main.cpp src/evaluate.cpp src/pvt.cpp src/search.cpp src/undo.cpp src/test.cpp src/makemove.cpp src/validate.cpp src/init.cpp src/hashkey.cpp src/bitboard.cpp src/board.cpp src/pieces.cpp src/io.cpp src/threats.cpp src/move.cpp src/movelist.cpp src/movegenerator.cpp -o bin/chess

run:
	@echo "Running chess"
	./bin/chess

clean:
	@echo "~All Clean~"
	rm ./bin/chess
