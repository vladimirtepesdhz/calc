DEBUG = 0

all : mk runm

mk :
	@if [ ! -f genmake.sh ] ; then \
		echo 'ERROR: cannot find gemake.sh';\
		exit 1;\
	fi
	@if [ ! -f files.list ] ; then \
		echo 'ERROR: cannot find files.list';\
		exit 1;\
	fi
	bash genmake.sh files.list mk;

runm :
	@make DEBUG=$(DEBUG) -f mk

.PHONY:clean
clean:
	@make DEBUG=$(DEBUG) clean -f mk
