include common.mk
SUBDIRS := inc src 
.PHONY: all clean deps.mk

all: deps.mk
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir all || exit 1; done
  
clean:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir clean || exit 1; done
	
deps.mk:
	$(MAKE) -C src deps.mk
