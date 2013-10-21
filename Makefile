TOPDIR = .
DIRS = src train

all:
	@cd src; make all
	@cd train; make all

clean:
	@cd src; make clean
	@cd train; make clean
