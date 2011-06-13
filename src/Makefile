include ../common.mk
include deps.mk

OBJS := exception.o loader.o matrix.o point.o triangle.o vector.o
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
	for file in $(SOURCES); do $(CXX) $(INCLUDES) -M $$file >> deps.mk; done
