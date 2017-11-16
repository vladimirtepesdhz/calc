DEBUG = 0

all : genm runm

genm : genmake.sh files.list
	@bash genmake.sh files.list mk
runm :
	@make DEBUG=$(DEBUG) -f mk

.PHONY:clean
clean:
	@make DEBUG=$(DEBUG) clean -f mk
