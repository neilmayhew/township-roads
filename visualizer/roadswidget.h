/*
 *	roadswidget.h
 *
 *	gtkmm widget for the township-roads visualizer
 *
 *	Neil Mayhew - 2007-12-08
 *
 *	$Id: roadswidget.h,v 1.2 2007-12-10 03:27:45 mayhewn Exp $
 */

#ifndef ROADSWIDGET_H
#define ROADSWIDGET_H

#include <gtkmm/widget.h>

#include "geometry2d/line.hpp"

class RoadsWidget : public Gtk::Widget
{
public:
	RoadsWidget();
protected:
	virtual void on_size_request(Gtk::Requisition* requisition);
	virtual void on_size_allocate(Gtk::Allocation& allocation);
	virtual void on_realize();
	virtual void on_unrealize();
	virtual bool on_expose_event(GdkEventExpose* event);

	Glib::RefPtr<Gdk::Window> m_refGdkWindow;

	geometry2d::Line h_;
	double dh_;
	geometry2d::Line v_;
	double dv_;

	void drawRoads(Cairo::RefPtr<Cairo::Context> cr);
};

#endif //ROADSWIDGET_H
