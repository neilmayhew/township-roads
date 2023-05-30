
#	$Id: Makefile,v 1.5 2007-11-16 05:42:00 mayhewn Exp $
#
#	Makefile for township-roads programs
#
#	Neil Mayhew - 2007-07-29

CPPFLAGS = -DSVG -MMD -I.
CXXFLAGS = -g -O3

.DELETE_ON_ERROR:

all: township-roads

check: township-roads.svg
	diff regression.svg $<

%: %.cpp
	$(LINK.cc) $< $(LOADLIBES) $(LDLIBS) -o $@

%.svg: %
	./$< 1.1,0.2:6.3,-0.8 6.1 -0.2,0.2:-0.5,9.3 5.8 >$@

clean:
	$(RM) township-roads township-roads.svg
	$(RM) *.[od]

-include *.d
