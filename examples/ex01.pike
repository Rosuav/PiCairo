/* -*- Mode: pike; -*- */

#ifndef WIDTH
# define WIDTH 400
#endif

#ifndef HEIGHT
# define HEIGHT 400
#endif

#ifndef M_PI
# define M_PI 3.14159235
#endif

Cairo.Matrix transform;

void
handle_key_press(GTK2.Widget widget, array events, int dummy)
{
  int cx = WIDTH / 2;
  int cy = HEIGHT / 2;

  cx = widget->allocation()["width"] / 2;
  cy = widget->allocation()["height"] / 2;

  switch(events[0]["keyval"])
    {
    case 'i':
      transform->init_identity();
      break;
    case 'z':
      transform->translate(cx, cy);
      transform->scale(2.0,2.0);
      transform->translate(-cx, -cy);
      break;
    case 'x':
      transform->translate(cx, cy);
      transform->scale(0.5,0.5);
      transform->translate(-cx, -cy);
      break;
    case 'w':
      transform->translate(0, cy/8);
      break;
    case 's':
      transform->translate(0, -cy/8);
      break;
    case 'a':
      transform->translate(cx/8, 0);
      break;
    case 'd':
      transform->translate(-cx/8, 0);
      break;
    case 'q':
      transform->translate(cx, cy);
      transform->rotate(-M_PI/18);
      transform->translate(-cx, -cy);
      break;
    case 'e':
      transform->translate(cx, cy);
      transform->rotate(M_PI/18);
      transform->translate(-cx, -cy);
      break;
    case 'p':
      write("_sprintf(): %O\n", transform);
      break;
    }

  widget->get_child()->queue_draw();
}

void
draw (Cairo.Context cr, int width, int height)
{
  int radius;

  if (width < height)
    radius = width/2 - 4;
  else
    radius = height/2 - 4;

  cr->identity_matrix();
  cr->transform(transform);
  cr->move_to(width/2, height/2);
  cr->arc(width/2, height/2, radius,
          0.0, 2 * M_PI);
  cr->close_path();

  cr->set_source(0.6, 0.8, 1.0);
  cr->fill_preserve();
  cr->set_source(0.0, 0.0, 0.0, 0.4);
  cr->stroke();

  float cx = width/2.0;
  float rx = max(width/18.0, 60);
  float minx = cx - rx;
  float maxx = cx + rx;

  float cy = height/2.0;
  float ry = max(height/18.0, 20.0);
  float miny = cy - ry;
  float maxy = cy + ry;

  cr->move_to(minx, miny);
  cr->line_to(maxx, miny);
  cr->line_to(maxx, maxy);
  cr->line_to(minx, maxy);
  cr->close_path();

  {
    Cairo.Path p = cr->copy_path();
    foreach(p; int i; Cairo.PathElement e)
      {
        write("%O\n", e);
      }
  }

  cr->set_source(0.6, 0.8, 1.0);
  cr->fill_preserve();
  cr->set_source(0.0, 0.0, 0.0, 0.4);
  cr->stroke();

  cr->set_source(0.5, 0.0, 0.0, 0.8);
  cr->move_to(width/2 - width/18, height/2);

  Cairo.FontOptions options = cr->get_font_options();
  options->set_antialias(Cairo.CAIRO_ANTIALIAS_NONE);
  cr->set_font_options(options);
  cr->set_font_size(22.0);
  cr->show_text("This a sample");

  {
    Cairo.Path p = cr->copy_path();
    foreach(p; int i; Cairo.PathElement e)
      {
        write("%O\n", e);
      }
  }
}

void handle_expose(GTK2.Widget widget, array events, int dummy)
{
  Cairo.Context cr = Cairo.GTK2Context(widget);
  draw(cr, widget->allocation()->width, widget->allocation()->height);
}

int main()
{
  transform = Cairo.Matrix();
  transform->init_identity();
  GTK2.Widget drawing_area;
  GTK2.setup_gtk();
  GTK2.Widget window = GTK2.Window(GTK2.WINDOW_TOPLEVEL);

  window->set_default_size(WIDTH, HEIGHT);
  window->set_title("cairo demo");

  window->signal_connect("destroy", GTK2.main_quit);

  drawing_area = GTK2.DrawingArea();
  window->add(drawing_area);

  drawing_area->signal_connect("expose-event",
                               handle_expose);

  drawing_area->signal_connect("popup_menu",
                               handle_key_press);

  window->signal_connect("key-press-event",
                         handle_key_press);
  window->show_all();

  Cairo.Surface surface = Cairo.ImageSurface(Cairo.CAIRO_FORMAT_ARGB32, WIDTH, HEIGHT);
  Cairo.Context cr = Cairo.Context(surface);
  draw(cr, WIDTH, HEIGHT);

#if constant(Cairo.CAIRO_HAS_PNG_FUNCTIONS)
  surface->write_to_png("examples/output.png");
#endif

  mapping image = surface->get_image();

  Image.Image im = image["image"]->blur(2);
  Image.Image alpha = image["alpha"]->blur(2);
  Stdio.write_file("examples/blur.png",
                   Image.PNG.encode(im, ([ "alpha" : alpha ])));

  GTK2.main();

  return 0;
}
