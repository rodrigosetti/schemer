#! /bin/make

SRCDIR=src
SRCFILES=$(wildcard $(SRCDIR)/*.cpp)
OBJFILES=$(patsubst %.cpp,%.o,$(wildcard $(SRCDIR)/*.cpp))

schemer: $(OBJFILES)
	$(CXX) -o schemer $(OBJFILES)

tags: $(SRCFILES)
	ctags -R $(SRCDIR)

.PHONY: clean
clean:
	$(RM) $(patsubst %.cpp,%.o,$(wildcard $(SRCDIR)/*.cpp))

