/*
 *	roadswidget.h
 *
 *	gtkmm widget for the township-roads visualizer
 *
 *	Neil Mayhew - 2007-12-08
 *
 *	$Id: roadswidget.h,v 1.3 2008-04-05 15:56:07 mayhewn Exp $
 */

#ifndef ROADSWIDGET_H
#define ROADSWIDGET_H

#include <gtkmm/drawingarea.h>
#include "geometry2d/line.hpp"

class RoadsWidget : public Gtk::DrawingArea
{
public:
	RoadsWidget();
protected:
	geometry2d::Line h_;
	double dh_;
	geometry2d::Line v_;
	double dv_;

	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
};

#endif //ROADSWIDGET_H
