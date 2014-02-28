all: liveness.so reaching-definitions.so loop-invariant-code-motion.so

CXXFLAGS = -rdynamic $(shell llvm-config --cxxflags) -g -O0

dataflow.o: dataflow.cpp dataflow.h
util.o: util.cpp util.h

%.so: %.o dataflow.o util.o
	$(CXX) -dylib -flat_namespace -shared $^ -o $@

clean:
	rm -f *.o *~ *.so

.PHONY: clean all
