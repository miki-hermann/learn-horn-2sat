.PHONY all compile install

all: compile install

compile:
	$(MAKE) -C horn
	$(MAKE) -C 2sat

install:
	$(MAKE) -C horn install
	$(MAKE) -C 2sat install

clean:
	$(MAKE) -C horn clean
	$(MAKE) -C 2sat clean

scratch:
	$(MAKE) -C horn scratch
	$(MAKE) -C 2sat scratch
