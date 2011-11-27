
#ifndef WIDTH
# define WIDTH 400
#endif

#ifndef HEIGHT
# define HEIGHT 400
#endif

void handle_expose(GTK2.Widget widget, array events, int dummy)
{
  Cairo.Context ctx = Cairo.GTK2Context(widget);

  ctx->move_to(0,13);
  ctx->show_text("(Empty. The interesting stuff is in the console)");

  mixed e = catch {
      ctx->restore(); // Ooops! Didn't call save() before!
    };

  // if-else-omania here we come
  if (!e)
    {
      write("The expected error 'save/restore-error' was not thrown!\n");
    }
  else if (!objectp(e))
    {
      write("The expected error object was not found! Got %O\n", e);
    }
  else if (!e->is_cairo_error)
    {
      write("Expected Cairo.Error. Couldn't find mark.");
    }
  else if (e->cairo_status != Cairo.CAIRO_STATUS_INVALID_RESTORE)
    {
      write("The expected error code wasn't found. Got %d\n", e->code);
    }
  else if (e->cairo_message != "cairo_restore without matching cairo_save")
    {
      write("Unknown message attached. (This could very well be Cairo-version-dependant and not a real error)\n");
    }
  else
    {
      write("Success!\n");
    }
}

int main()
{
  GTK2.Widget drawing_area;
  GTK2.setup_gtk();
  GTK2.Widget window = GTK2.Window(GTK2.GTK_WINDOW_TOPLEVEL);
  window->set_default_size(WIDTH, HEIGHT);
  window->set_title("cairo samples from http://cairographics.org/samples/");
  window->signal_connect("destroy", GTK2.main_quit);

  drawing_area = GTK2.DrawingArea();
  window->add(drawing_area);

  drawing_area->signal_connect("expose-event", handle_expose);
  window->show_all();

  GTK2.main();

  return 0;
}
