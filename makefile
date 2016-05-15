all:
	@echo "Building Chess!"
	g++ -g -Wall main.cpp -o chess

clean:
	@echo "~All Clean~"
	rm chess
