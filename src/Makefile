WARNFLAGS := -pedantic -Wall -Warray-bounds -Wextra -Winit-self \
             -Wpointer-arith -Wredundant-decls -Wuninitialized -Weffc++ \
             -Wold-style-cast -Woverloaded-virtual -Wsign-promo \
             -Wstrict-null-sentinel -Wctor-dtor-privacy

CXXFLAGS := -O3 -march=native -fomit-frame-pointer -pipe -I../inc $(WARNFLAGS)
#CXXFLAGS := -g -pipe -I../inc

OBJS := exception.o loader.o point.o vector.o
MAIN := main.o

LIBS := -lboost_regex-mt

OUT := raytracer

.PHONY: all clean

all: $(OUT)

clean:
	$(RM) $(MAIN) $(OBJS) $(OUT)

$(OUT): $(MAIN) $(OBJS)
	$(CXX) -o $(OUT) $(MAIN) -Wl,--as-needed $(OBJS) $(LIBS)
