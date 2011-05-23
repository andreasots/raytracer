SUBDIRS := inc src 
.PHONY: all clean deps.mk

all: common.mk deps.mk
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir all || exit 2; done
  
clean:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir clean || exit 2; done
	
deps.mk:
	$(MAKE) -C src deps.mk || exit 2

common.mk: common.mk.in
	@echo "Run ./configure before make."
	@exit 1