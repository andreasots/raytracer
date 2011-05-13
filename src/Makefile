include ../common.mk
include deps.mk

SOURCES := exception.cpp loader.cpp main.cpp point.cpp vector.cpp
OBJS := exception.o loader.o point.o vector.o
MAIN := main.o

LIBS := -lboost_regex-mt

OUT := raytracer

.PHONY: all clean deps

all: deps $(OUT)

clean:
	$(RM) $(MAIN) $(OBJS) $(OUT) deps.mk

$(OUT): $(MAIN) $(OBJS)
	$(CXX) -o $(OUT) $(MAIN) -Wl,--as-needed $(OBJS) $(LIBS)

deps.mk: $(SOURCES) $(HEADERS)
	$(RM) deps.mk
	for file in $(SOURCES); do $(CXX) $(CXXFLAGS) -M $$file >> deps.mk; done
