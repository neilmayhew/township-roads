/*
 *	roadswindow.cc
 *
 *	gtkmm window for the township-roads visualizer
 *
 *	Neil Mayhew - 2007-12-08
 *
 *	$Id: roadswindow.cc,v 1.3 2008-04-05 15:56:07 mayhewn Exp $
 */

#include "roadswindow.h"

RoadsWindow::RoadsWindow() :
	m_Button_Quit("Quit")
{
	set_title("Township Roads Visualizer");
	set_default_size(600, 600);

	add(m_VBox);
	
	m_VBox.set_spacing(6);
	m_VBox.set_border_width(6);

	m_VBox.pack_start(m_Frame, Gtk::PACK_EXPAND_WIDGET);
	m_VBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK);
	
	m_Frame.add(m_RoadsWidget);
	m_Frame.set_shadow_type(Gtk::SHADOW_IN);

	m_ButtonBox.pack_start(m_Button_Quit, Gtk::PACK_SHRINK);
	m_ButtonBox.set_layout(Gtk::BUTTONBOX_END);

	m_Button_Quit.signal_clicked().connect(
		sigc::mem_fun(*this, &RoadsWindow::hide));

	show_all_children();
}
