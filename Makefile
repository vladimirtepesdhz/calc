DEBUG = 0

all : genm runm

genm : genmake.sh files.list
	@if [ ! -f genmake.sh ] ; then \
		echo 'ERROR: cannot find gemake.sh';\
		exit 1;\
	fi
	@if [ ! -f files.list ] ; then \
		echo 'ERROR: cannot find files.list';\
		exit 1;\
	fi
	@if [ ! -f mk ] ; then \
		bash genmake.sh files.list mk;\
	fi
runm :
	@make DEBUG=$(DEBUG) -f mk

.PHONY:clean
clean:
	@make DEBUG=$(DEBUG) clean -f mk
