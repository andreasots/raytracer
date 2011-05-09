CXXFLAGS := -O3 -march=native -fomit-frame-pointer -pipe -I../inc

OBJS := exception.o loader.o
MAIN := main.o

LIBS := -lboost_regex-mt

OUT := raytracer

.PHONY: all clean

all: $(OUT)

clean:
	$(RM) $(MAIN) $(OBJS) $(OUT)

$(OUT): $(MAIN) $(OBJS)
	$(CXX) -o $(OUT) $(MAIN) -Wl,--as-needed $(OBJS) $(LIBS)
