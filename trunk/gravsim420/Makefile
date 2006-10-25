CPP = g++
OPTS =  -I"../../include" -I"/usr/X11R6/include" -L"/usr/X11R6/lib" -L"../../lib/Linux" -lIrrlicht -lGL -lGLU -lXxf86vm -lXext -lX11

all:
	$(CPP) main.cpp -o example $(OPTS)
	echo "use make release to build tarballs and astyle the source"
clean:
	rm example
release:
	astyle *.cpp
	astyle *.h
	rm *.cpp.orig
	rm *.h.orig
	touch example
	rm example
	touch GravSim.tar.bz2
	rm GravSim.tar.bz2
	tar -cf GravSim.tar *
	bzip2 -9 GravSim.tar
