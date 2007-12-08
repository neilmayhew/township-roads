/*
 *	roadswindow.h
 *
 *	gtkmm window for the township-roads visualizer
 *
 *	Neil Mayhew - 2007-12-08
 *
 *	$Id: roadswindow.h,v 1.1 2007-12-08 18:58:23 mayhewn Exp $
 */

#ifndef ROADSWINDOW_H
#define ROADSWINDOW_H

#include <gtkmm.h>

#include "roadswidget.h"

class RoadsWindow : public Gtk::Window
{
public:
	RoadsWindow();
protected:
	Gtk::VBox m_VBox;
	Gtk::Frame m_Frame;
	Gtk::HButtonBox m_ButtonBox;
	Gtk::Button m_Button_Quit;

	RoadsWidget m_RoadsWidget;
};

#endif //ROADSWINDOW_H
