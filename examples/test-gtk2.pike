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

void snippet_normalize(Cairo.Context ctx, int width, int height)
{
  ctx->identity_matrix();

  Cairo.Matrix transform = Cairo.Matrix();
  transform->init_identity();
  transform->translate(width*0.05, height*0.05);
  transform->scale(width*0.9, height*0.9);

  ctx->set_line_width(0.03);
  ctx->transform(transform);

  ctx->move_to(0.0, 0.0);
  ctx->line_to(1.0, 0);
  ctx->line_to(1.0, 1.0);
  ctx->line_to(0.0, 1.0);
  ctx->close_path();
  ctx->clip();
}

void draw(Cairo.Context ctx, int width, int height)
{
  snippet_normalize (ctx, width, height);

  Cairo.Gradient grad = Cairo.LinearGradient (0.0, 0.0,  0.0, 1.0);
  grad->add_color_stop_rgba (1.0, 0.0, 0.0, 0.0, 1.0);
  grad->add_color_stop_rgba (0.0, 1.0, 1.0, 1.0, 1.0);
  ctx->rectangle( 0, 0, 1, 1);
  ctx->set_source(grad);
  ctx->fill ();

  grad = Cairo.RadialGradient(0.45, 0.4, 0.1,
                              0.4,  0.4, 0.5);
  grad->add_color_stop_rgba (0.0, 1.0, 1.0, 1.0, 1.0);
  grad->add_color_stop_rgba (1.0, 0.0, 0.0, 0.0, 1.0);
  ctx->set_source(grad);
  ctx->arc ( 0.5, 0.5, 0.3, 0, 2 * Math.pi);
  ctx->fill ();
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
