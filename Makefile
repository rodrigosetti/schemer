#! /bin/make

SRCDIR=src
OBJFILES=$(patsubst %.cpp,%.o,$(wildcard $(SRCDIR)/*.cpp))

schemer: $(OBJFILES)
	$(CXX) -o schemer $(OBJFILES)

.PHONY: clean
clean:
	$(RM) $(patsubst %.cpp,%.o,$(wildcard $(SRCDIR)/*.cpp))

