
#	$Id: Makefile,v 1.1 2007-07-29 23:03:26 mayhewn Exp $
#
#	Makefile for township-roads program
#
#	Neil Mayhew - 2007-07-29

CPPFLAGS = -DSVG
CXXFLAGS = -g -O3

.DELETE_ON_ERROR:

all: township-roads

test: township-roads.svg

township-roads.svg: township-roads
	./township-roads 1.1,0.2:6.3,-0.8 6.1 -0.2,0.2:-0.5,9.3 5.8 >$@

clean:
	$(RM) township-roads township-roads.svg *.[od]

