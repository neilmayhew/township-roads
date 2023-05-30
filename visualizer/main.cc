/*
 *	main.cc
 *
 *	Main program for township-roads visualizer
 *
 *	Neil Mayhew - 2007-12-08
 *
 *	$Id: main.cc,v 1.1 2007-12-08 18:58:23 mayhewn Exp $
 */

#include <gtkmm/main.h>
#include "roadswindow.h"

int main(int argc, char *argv[])
{
  Glib::OptionContext opts;
  Gtk::Main kit(argc, argv, opts);

  RoadsWindow window;
  Gtk::Main::run(window);

  return 0;
}
