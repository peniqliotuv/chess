all:
	@echo "Building Chess!"
	g++ -g -Wall src/main.cpp src/validate.cpp src/init.cpp src/hashkey.cpp src/bitboard.cpp src/board.cpp src/pieces.cpp src/io.cpp src/threats.cpp src/move.cpp src/movelist.cpp src/movegenerator.cpp -o bin/chess

run:
	@echo "Running and building chess"
	g++ -g -Wall src/main.cpp src/validate.cpp src/init.cpp src/hashkey.cpp src/bitboard.cpp src/board.cpp src/pieces.cpp src/io.cpp src/threats.cpp src/move.cpp src/movelist.cpp src/movegenerator.cpp -o bin/chess
	./bin/chess

clean:
	@echo "~All Clean~"
	rm ./bin/chess
