/*
 *	roadswidget.cc
 *
 *	gtkmm widget for the township-roads visualizer
 *
 *	Neil Mayhew - 2007-12-08
 *
 *	$Id: roadswidget.cc,v 1.1 2007-12-08 18:58:23 mayhewn Exp $
 */

#include "roadswidget.h"

#include <gdkmm/drawable.h>
#include <gdkmm/general.h>

#include <iostream>

RoadsWidget::RoadsWidget() :
	// The GType name will actually be gtkmm__CustomObject_roadswidget
	Glib::ObjectBase("roadswidget"),
	Gtk::Widget(),
	m_scale(1000)
{
	set_flags(Gtk::NO_WINDOW);

	// This shows the GType name, which must be used in the RC file.
	std::cout << "GType name: " << G_OBJECT_TYPE_NAME(gobj()) << std::endl;

	// Install a style so that an aspect of this widget may be themed via an RC file: 
	gtk_widget_class_install_style_property(
			GTK_WIDGET_CLASS(G_OBJECT_GET_CLASS(gobj())), 
			g_param_spec_int("example_scale",
				"Scale of Example Drawing",
				"The scale to use when drawing. This is just a silly example.",
				G_MININT,
				G_MAXINT,
				0,
				G_PARAM_READABLE) );

	gtk_rc_parse("custom_gtkrc");
}

void RoadsWidget::on_size_request(Gtk::Requisition* requisition)
{
	// Initialize the output parameter:
	*requisition = Gtk::Requisition();

	// Discover the total amount of minimum space needed by this widget.

	// Let's make this simple example widget always need 100 by 100:
	requisition->height = 100;
	requisition->width = 100;
}

void RoadsWidget::on_size_allocate(Gtk::Allocation& allocation)
{
	// Do something with the space that we have actually been given:
	// (We will not be given heights or widths less than we have requested, though
	// we might get more)

	// Use the offered allocation for this container:
	set_allocation(allocation);

	if (m_refGdkWindow)
	{
		m_refGdkWindow->move_resize( allocation.get_x(), allocation.get_y(),
			allocation.get_width(), allocation.get_height() );
	}
}

void RoadsWidget::on_map()
{
	// Call base class:
	Gtk::Widget::on_map();
}

void RoadsWidget::on_unmap()
{
	// Call base class:
	Gtk::Widget::on_unmap();
}

void RoadsWidget::on_realize()
{
	// Call base class:
	Gtk::Widget::on_realize();

	ensure_style();

	// Get the themed style from the RC file:
	int scale;
	get_style_property("example_scale", scale);
	if (scale)
	{
		std::cout << "setting m_scale to "
			<< scale
			<< " (example_scale from the theme/rc-file)"
			<< std::endl;
		m_scale = scale;
	}

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


		m_refGdkWindow = Gdk::Window::create(get_window() /* parent */, &attributes,
						GDK_WA_X | GDK_WA_Y);
		unset_flags(Gtk::NO_WINDOW);
		set_window(m_refGdkWindow);

		// set colors
		modify_bg(Gtk::STATE_NORMAL , Gdk::Color("red"));
		modify_fg(Gtk::STATE_NORMAL , Gdk::Color("blue"));

		// make the widget receive expose events
		m_refGdkWindow->set_user_data(gobj());
	}
}

void RoadsWidget::on_unrealize()
{
	m_refGdkWindow.clear();

	// Call base class:
	Gtk::Widget::on_unrealize();
}

bool RoadsWidget::on_expose_event(GdkEventExpose* event)
{
	if (m_refGdkWindow)
	{
		double scale_x = (double)get_allocation().get_width() / m_scale;
		double scale_y = (double)get_allocation().get_height() / m_scale;
		
		if (scale_x > scale_y)
			scale_x = scale_y;
		else
			scale_y = scale_x;

		Cairo::RefPtr<Cairo::Context> cr = m_refGdkWindow->create_cairo_context();
		if (event)
		{
			//  clip to the area that needs to be re-exposed so we don't draw any
			//  more than we need to.
			cr->rectangle(event->area.x, event->area.y,
							event->area.width, event->area.height);
			cr->clip();
		}

		//  paint the background
		Gdk::Cairo::set_source_color(cr, get_style()->get_bg(Gtk::STATE_NORMAL));
		cr->paint();

		//  draw the foreground
		Gdk::Cairo::set_source_color(cr, get_style()->get_fg(Gtk::STATE_NORMAL));
		cr->move_to(155.*scale_x, 165.*scale_y);
		cr->line_to(155.*scale_x, 838.*scale_y);
		cr->line_to(265.*scale_x, 900.*scale_y);
		cr->line_to(849.*scale_x, 564.*scale_y);
		cr->line_to(849.*scale_x, 438.*scale_y);
		cr->line_to(265.*scale_x, 100.*scale_y);
		cr->line_to(155.*scale_x, 165.*scale_y);
		cr->move_to(265.*scale_x, 100.*scale_y);
		cr->line_to(265.*scale_x, 652.*scale_y);
		cr->line_to(526.*scale_x, 502.*scale_y);
		cr->move_to(369.*scale_x, 411.*scale_y);
		cr->line_to(633.*scale_x, 564.*scale_y);
		cr->move_to(369.*scale_x, 286.*scale_y);
		cr->line_to(369.*scale_x, 592.*scale_y);
		cr->move_to(369.*scale_x, 286.*scale_y);
		cr->line_to(849.*scale_x, 564.*scale_y);
		cr->move_to(633.*scale_x, 564.*scale_y);
		cr->line_to(155.*scale_x, 838.*scale_y);
		cr->stroke();
	}
	return true;
}
