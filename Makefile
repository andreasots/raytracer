.PHONY: all src/

all: src/

*/:
	$(MAKE) -C $@
