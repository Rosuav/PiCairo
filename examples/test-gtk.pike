/*
 * This program verifies that we can gracefully resist trying to
 * create Cairo.GTK2Context from a non-GObject.
 */

#ifndef WIDTH
# define WIDTH 400
#endif

#ifndef HEIGHT
# define HEIGHT 400
#endif

#if !constant(Cairo)
# error This program requires the Cairo module
#endif

void
draw (Cairo.Context cr, int width, int height)
{
  werror("Not reached\n");
}

void handle_expose(int dummy, GTK.Widget widget, array events)
{
  mixed e = catch {
      Cairo.Context cr = Cairo.GTK2Context(widget);
      draw(cr, widget->allocation()->width, widget->allocation()->height);
    };
  
  if (e)
    {
      write("Success. There is no way to create a cairo context from a GTK.Widget:\n %O", e);
    }
}

int main()
{
  GTK.Widget drawing_area;
  GTK.setup_gtk();
  GTK.Widget window = GTK.Window( GTK.WindowToplevel );

  window->set_default_size(WIDTH, HEIGHT);
  window->set_title("cairo demo");

  window->signal_connect("destroy", GTK.main_quit);

  drawing_area = GTK.DrawingArea();
  window->add(drawing_area);

  drawing_area->signal_connect("expose-event", handle_expose);
  window->show_all();

  GTK.main();

  return 0;
}
