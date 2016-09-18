COMP = g
CXX = ++

# Warnings frequently signal eventual errors:
CXXFLAGS=`sdl-config --cflags` -g -W -Wall -Weffc++ -Wextra -pedantic -O0

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS = -framework Foundation -framework GLUT -framework OpenGL -lm
else
  ifeq ("$(shell uname)", "Linux")
  LDFLAGS = `sdl-config --libs` -lm -lSDL_ttf -lSDL_image -lexpat
  endif
endif

OBJS = \
	main.o
EXEC = run

%.o: %.cpp
	reset 
	$(COMP)$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXEC): $(OBJS)
	$(COMP)$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

main.o: main.cpp

clean:
	rm -rf $(OBJS)
	rm -rf $(EXEC)
