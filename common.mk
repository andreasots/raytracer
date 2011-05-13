WARNFLAGS := -pedantic -Wall -Warray-bounds -Wextra -Winit-self \
             -Wpointer-arith -Wredundant-decls -Wuninitialized -Weffc++ \
             -Wold-style-cast -Woverloaded-virtual -Wsign-promo \
             -Wstrict-null-sentinel -Wctor-dtor-privacy

CXXFLAGS := -O3 -march=native -fomit-frame-pointer -pipe -I../inc $(WARNFLAGS)

SOURCES := $(shell find -name "*.cpp")
HEADERS := $(shell find ../inc -name "*.h")
