/*
 *	roadswidget.cc
 *
 *	gtkmm widget for the township-roads visualizer
 *
 *	Neil Mayhew - 2007-12-08
 *
 *	$Id: roadswidget.cc,v 1.2 2007-12-10 03:27:45 mayhewn Exp $
 */

#include "roadswidget.h"

#include <gdkmm/drawable.h>
#include <gdkmm/general.h>

#include <iostream>
#include <sstream>

#include "geometry2d/streamio.hpp"

using namespace geometry2d;

RoadsWidget::RoadsWidget() :
	Glib::ObjectBase("roadswidget")
{
	set_flags(Gtk::NO_WINDOW);
}

void RoadsWidget::on_size_request(Gtk::Requisition* requisition)
{
	*requisition = Gtk::Requisition();

	requisition->height = 400;
	requisition->width = 400;
}

void RoadsWidget::on_size_allocate(Gtk::Allocation& allocation)
{
	// Use the offered allocation for this container:
	set_allocation(allocation);

	if (m_refGdkWindow)
		m_refGdkWindow->move_resize( allocation.get_x(), allocation.get_y(),
			allocation.get_width(), allocation.get_height() );
}

void RoadsWidget::on_realize()
{
	Gtk::Widget::on_realize();

	ensure_style();

	h_ = Line(Point( 1.1,0.2), Point( 6.3,-0.8));
	dh_ = 6.1;
	v_ = Line(Point(-0.2,0.2), Point(-0.5, 9.3));
	dv_ = 5.8;

	if (!m_refGdkWindow)
	{
		// Create the GdkWindow:

		GdkWindowAttr attributes;
		memset(&attributes, 0, sizeof(attributes));

		Gtk::Allocation allocation = get_allocation();

		// Set initial position and size of the Gdk::Window:
		attributes.x = allocation.get_x();
		attributes.y = allocation.get_y();
		attributes.width = allocation.get_width();
		attributes.height = allocation.get_height();

		attributes.event_mask = get_events () | Gdk::EXPOSURE_MASK; 
		attributes.window_type = GDK_WINDOW_CHILD;
		attributes.wclass = GDK_INPUT_OUTPUT;

		m_refGdkWindow = Gdk::Window::create(get_window(), &attributes, GDK_WA_X|GDK_WA_Y);
		unset_flags(Gtk::NO_WINDOW);
		set_window(m_refGdkWindow);

		// make the widget receive expose events
		m_refGdkWindow->set_user_data(gobj());
	}
}

void RoadsWidget::on_unrealize()
{
	m_refGdkWindow.clear();

	Gtk::Widget::on_unrealize();
}

bool RoadsWidget::on_expose_event(GdkEventExpose* event)
{
	if (m_refGdkWindow)
	{
		Cairo::RefPtr<Cairo::Context> cr = m_refGdkWindow->create_cairo_context();

		if (event)
		{
			// Clip to the area that needs to be drawn
			cr->rectangle(
				event->area.x, event->area.y,
				event->area.width, event->area.height);
			cr->clip();
		}

		// paint the background
		cr->set_source_rgb(1.0, 1.0, 1.0);
		cr->paint();

		// draw the foreground
		drawRoads(cr);
	}

	return true;
}

// Problem - dh, dv are perpendicular distances from lines h, v respectively

inline Point solve(const Line& h, double dh, const Line& v, double dv)
{
	// Note: -dv because we measure distance to the *E* of v, and to the N of h,
	// and assuming h is specified W-E and v, S-N
	return (h || dh) & (v || -dv);
}

Cairo::RefPtr<Cairo::Context> operator<<(Cairo::RefPtr<Cairo::Context> cr, const Line& l)
{
	cr->move_to(l.first().x(), -l.first().y());
	cr->line_to(l.second().x(), -l.second().y());
	cr->stroke();
	return cr;
}

void RoadsWidget::drawRoads(Cairo::RefPtr<Cairo::Context> cr)
{
	Line h = h_;
	double dh = dh_;
	Line v = v_;
	double dv = dv_;
	Point p = solve(h, dh, v, dv);

	Line hp = h ||  dh;
	Line vp = v || -dv;
	
	Point min(-2, -2);
	Point max(10, 10);

	Line xaxis(Point(min.x(), 0), Point(max.x(), 0));
	Line yaxis(Point(0, min.y()), Point(0, max.y()));

	Line left(min, Point(min.x(), max.y()));
	Line right(Point(max.x(), min.y()), max);
	Line bottom(min, Point(max.x(), min.y()));
	Line top(Point(min.x(), max.y()), max);

	Line hfull(h & left, h & right);
	Line vfull(v & bottom, v & top);
	Line hpfull(hp & left, hp & right);
	Line vpfull(vp & bottom, vp & top);

	Line hperp(p - normalize(rotate(h)) *  dh, p); 
	Line vperp(p - normalize(rotate(v)) * -dv, p);

	double scale_x = (double)get_allocation().get_width()  / (max - min).x();
	double scale_y = (double)get_allocation().get_height() / (max - min).y();
	double scale = std::min(scale_x, scale_y);
	
	cr->translate(0, get_allocation().get_height());
	cr->scale(scale, scale);
	cr->translate(-min.x(), min.y());

	cr->set_source_rgb(1.0, 0.0, 0.0);
	cr->set_line_width(0.1);
	cr->arc(p.x(), -p.y(), 0.2, 0.0, 2 * M_PI);
	cr->fill();

	std::ostringstream s; s << p;
	cr->move_to(p.x() + 0.5, -p.y() - 0.25);
	cr->set_source_rgb(0, 0, 0);
	cr->set_font_size(0.5);
	cr->show_text(s.str());

	cr->set_source_rgb(0.25, 0.25, 0.867);
	cr->set_line_width(0.02);

	cr << xaxis << yaxis;
	
	int count = 0;
	for (double x = min.x() + 1.0; x < max.x(); x += 1.0)
	{
		if (x == 0.0)
			continue;
		Line tick(Point(x, -0.1), Point(x, 0.1));
		cr << tick;
	}
	
	for (double y = min.y() + 1.0; y < max.y(); y += 1.0)
	{
		if (y == 0.0)
			continue;
		Line tick(Point(-0.1, y), Point(0.1, y));
		cr << tick;
	}

	cr->set_source_rgba(0.0, 0.0, 0.0, 0.2);
	cr->set_line_width(0.1);
	cr << hfull;
	cr << vfull;
	cr->set_source_rgba(0.25, 0.867, 0.25, 0.2);
	cr->set_line_width(0.1);
	cr << hpfull;
	cr << vpfull;
	cr->set_source_rgba(0.25, 0.867, 0.25, 0.6);
	cr->set_line_width(0.02);
	cr << hperp;
	cr << vperp;
	cr->set_source_rgb(0.0, 0.0, 0.0);
	cr->set_line_width(0.1);
	cr << h;
	cr << v;
	cr->set_source_rgb(0.25, 0.867, 0.25);
	cr->set_line_width(0.1);
	cr << hp;
	cr << vp;
}
