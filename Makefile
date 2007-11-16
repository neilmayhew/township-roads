
#	$Id: Makefile,v 1.4 2007-11-16 05:34:37 mayhewn Exp $
#
#	Makefile for township-roads programs
#
#	Neil Mayhew - 2007-07-29

CPPFLAGS = -DSVG -MMD -I.
CXXFLAGS = -g -O3

.DELETE_ON_ERROR:

all: township-roads geometry

test: township-roads.svg geometry.svg

%: %.cpp
	$(LINK.cc) $< $(LOADLIBES) $(LDLIBS) -o $@

%.svg: %
	./$< 1.1,0.2:6.3,-0.8 6.1 -0.2,0.2:-0.5,9.3 5.8 >$@

clean:
	$(RM) township-roads township-roads.svg
	$(RM) geometry geometry.svg
	$(RM) *.[od]

-include *.d
