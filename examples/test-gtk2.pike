/*
 * One of the renderers from examples/samples.pike
 * used in a GTK2 window
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

void draw(Cairo.Context ctx, int width, int height)
{
	ctx->rectangle(0,0,width,height); ctx->fill(); //Black background
	ctx->move_to(5,16);
	ctx->select_font_face("Courier",0,1);
	ctx->set_font_size(16);
	ctx->set_source(1,1,1); //White text
	ctx->show_text("Hello, world!");
}

void handle_expose(GTK2.Widget widget, array events, int dummy)
{
  mixed e = catch {
      Cairo.Context cr = Cairo.GTK2Context(widget);
      draw(cr, widget->allocation()->width, widget->allocation()->height);
    };
  
  if (e)
    {
      write("Success. There is no way to create a cairo context from a GTK2.TextBuffer(): %O", e);
    }
}

int main()
{
  GTK2.Widget drawing_area;
  GTK2.setup_gtk();
  GTK2.Widget window = GTK2.Window( GTK2.WindowToplevel );

  window->set_default_size(WIDTH, HEIGHT);
  window->set_title("cairo demo");

  window->signal_connect("destroy", GTK2.main_quit);

  drawing_area = GTK2.DrawingArea();
  window->add(drawing_area);

  drawing_area->signal_connect("expose-event", handle_expose);
  window->show_all();

  GTK2.main();

  return 0;
}
