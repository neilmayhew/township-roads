DEFS = @DEFS@ \
	-DPANGOMM_DISABLE_DEPRECATED \
	-DATKMM_DISABLE_DEPRECATED \
	-DGDKMM_DISABLE_DEPRECATED \
	-DGTKMM_DISABLE_DEPRECATED

INCLUDES = @GTKMM_CFLAGS@ -I$(srcdir)/..
LIBS = @GTKMM_LIBS@

noinst_PROGRAMS = visualizer
visualizer_SOURCES = roadswidget.h roadswidget.cc roadswindow.h roadswindow.cc main.cc
