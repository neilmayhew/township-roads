/*
 *	drawingwidget.h
 *
 *	gtkmm abstract widget for cairo drawing
 *
 *	Neil Mayhew - 2008-04-05
 *
 *	$Id: drawingwidget.h,v 1.1 2008-04-05 15:56:07 mayhewn Exp $
 */

#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <gtkmm/widget.h>

class DrawingWidget : public Gtk::Widget
{
public:
	DrawingWidget(int preferred_width = 400, int preferred_height = 400);
protected:
	virtual void on_size_request(Gtk::Requisition* requisition);
	virtual void on_size_allocate(Gtk::Allocation& allocation);
	virtual void on_realize();
	virtual void on_unrealize();
	virtual bool on_expose_event(GdkEventExpose*);

	Glib::RefPtr<Gdk::Window> m_refGdkWindow;
	
	int m_preferred_width;
	int m_preferred_height;

	virtual void draw(Cairo::RefPtr<Cairo::Context>) = 0;
};

#endif //DRAWINGWIDGET_H
