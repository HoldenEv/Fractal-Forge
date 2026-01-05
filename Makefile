CXX = g++
CXXFLAGS = -std=c++17 -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

main: main.cpp mandelbrot.cpp
	$(CXX) $(CXXFLAGS) main.cpp mandelbrot.cpp -o main $(LDFLAGS) $(LIBS)

clean:
	rm -f main