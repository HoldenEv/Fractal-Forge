CXX = g++
CXXFLAGS = -std=c++17 -I. -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

SOURCES = \
    main.cpp \
    Sets/mandelbrot.cpp

main: $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o main $(LDFLAGS) $(LIBS)

clean:
	rm -f main
