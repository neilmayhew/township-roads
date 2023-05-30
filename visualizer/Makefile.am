DEFINES = @DEFS@ \
	-DPANGOMM_DISABLE_DEPRECATED \
	-DATKMM_DISABLE_DEPRECATED \
	-DGDKMM_DISABLE_DEPRECATED \
	-DGTKMM_DISABLE_DEPRECATED

LDADD = @GTKMM_LIBS@

AM_CPPFLAGS = @GTKMM_CFLAGS@ -I$(srcdir)/.. $(DEFINES)

bin_PROGRAMS = visualizer

visualizer_SOURCES = \
	drawingwidget.h drawingwidget.cc \
	roadswidget.h roadswidget.cc \
	roadswindow.h roadswindow.cc \
	main.cc
