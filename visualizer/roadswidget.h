/*
 *	roadswidget.h
 *
 *	gtkmm widget for the township-roads visualizer
 *
 *	Neil Mayhew - 2007-12-08
 *
 *	$Id: roadswidget.h,v 1.1 2007-12-08 18:58:23 mayhewn Exp $
 */

#ifndef ROADSWIDGET_H
#define ROADSWIDGET_H

#include <gtkmm/widget.h>

class RoadsWidget : public Gtk::Widget
{
public:
	RoadsWidget();
protected:
	virtual void on_size_request(Gtk::Requisition* requisition);
	virtual void on_size_allocate(Gtk::Allocation& allocation);
	virtual void on_map();
	virtual void on_unmap();
	virtual void on_realize();
	virtual void on_unrealize();
	virtual bool on_expose_event(GdkEventExpose* event);

	Glib::RefPtr<Gdk::Window> m_refGdkWindow;

	int m_scale;
};

#endif //ROADSWIDGET_H
