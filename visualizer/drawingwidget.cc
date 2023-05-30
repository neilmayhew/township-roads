/*
 *	drawingwidget.cc
 *
 *	gtkmm abstract widget for cairo drawing
 *
 *	Neil Mayhew - 2008-04-05
 *
 *	$Id: drawingwidget.cc,v 1.2 2008-11-09 03:27:53 mayhewn Exp $
 */

#include "drawingwidget.h"

#include <gdkmm/drawable.h>
#include <gdkmm/general.h>

#include <cstring>

DrawingWidget::DrawingWidget(int w, int h) :
	Glib::ObjectBase("drawingwidget"),
	m_preferred_width(w),
	m_preferred_height(h)
{
	set_has_window(false);
}

void DrawingWidget::on_size_request(Gtk::Requisition* requisition)
{
	*requisition = Gtk::Requisition();

	requisition->width  = m_preferred_width;
	requisition->height = m_preferred_height;
}

void DrawingWidget::on_size_allocate(Gtk::Allocation& allocation)
{
	// Use the offered allocation for this container:
	set_allocation(allocation);

	if (m_refGdkWindow)
		m_refGdkWindow->move_resize( allocation.get_x(), allocation.get_y(),
			allocation.get_width(), allocation.get_height() );
}

void DrawingWidget::on_realize()
{
	Gtk::Widget::on_realize();

	ensure_style();

	if (!m_refGdkWindow)
	{
		// Create the GdkWindow:

		GdkWindowAttr attributes;
		std::memset(&attributes, 0, sizeof(attributes));

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
		set_has_window(true);
		set_window(m_refGdkWindow);

		// make the widget receive expose events
		m_refGdkWindow->set_user_data(gobj());
	}
}

void DrawingWidget::on_unrealize()
{
	m_refGdkWindow.clear();

	Gtk::Widget::on_unrealize();
}

bool DrawingWidget::on_expose_event(GdkEventExpose* event)
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

		// draw the content
		draw(cr);
	}

	return true;
}
