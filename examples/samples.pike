/*
 * These are the samples from "http://cairographics.org/samples/"
 * packages together in an application.
 *
 * The samples that depend on librsvg with cairo as backend has been
 * modified to not use librsvg.
 */

#ifndef WIDTH
# define WIDTH 400
#endif

#ifndef HEIGHT
# define HEIGHT 400
#endif

#define ROOT "/home/davve/svn/pike/cairo/examples/"

#define M_PI Math.pi

array(function) g_sample_functions = ({ });
int g_sample_functions_index = 0;


void freedesktop_org(Cairo.Context ctx, int width, int height)
{
//   ctx->scale(1 / (111.0 *(float)width), 1 / (96.0*(float)height));
  ctx->translate(14.09647,16.85030);
  /* style="fill:#ffffff;stroke:#3b80ae;stroke-width:2.4588001;" */
  /*<path
    d="M 85.277000,40.796000 C 87.335000,48.680000 82.610000,56.738000 74.726000,58.795000 L 27.143000,71.210000 C 19.259000,73.267000 11.200000,68.543000 9.1430000,60.658000 L 1.6950000,32.108000 C -0.36200000,24.224000 4.3620000,16.166000 12.246000,14.109000 L 59.830000,1.6950000 C 67.714000,-0.36200000 75.772000,4.3620000 77.829000,12.246000 L 85.278000,40.796000 L 85.277000,40.796000 z "
    style="stroke:#bababa;"
    id="path866" /> */

  ctx->move_to(85.277000,40.796000);
  ctx->curve_to(87.335000,48.680000,
                82.610000,56.738000,
                74.726000,58.795000);
  ctx->line_to(27.143000,71.210000);
  ctx->curve_to(19.259000,73.267000,
                11.200000,68.543000,
                9.1430000,60.658000);
  ctx->line_to(1.6950000,32.108000);
  ctx->curve_to(-0.36200000,24.224000,
                4.3620000,16.166000,
                12.246000,14.109000);
  ctx->line_to(59.830000,1.6950000);
  ctx->curve_to(67.714000,-0.36200000,
                75.772000,4.3620000,
                77.829000,12.246000);
  ctx->line_to(85.278000,40.796000);
  ctx->close_path();
  ctx->set_line_width(1.0);
  ctx->set_source(0.73, 0.73, 0.73);
  ctx->stroke();

//   "M 80.444000,39.778000 C 82.193000,47.632000 78.628000,53.399000 70.940000,55.225000 L 28.704000,66.245000 C 21.135000,68.641000 14.615000,65.064000 12.866000,57.409000 L 6.5300000,33.127000 C 4.7810000,24.982000 7.2390000,20.238000 16.033000,17.680000 L 58.270000,6.6610000 C 66.414000,4.8350000 72.359000,8.0240000 74.108000,15.496000 L 80.444000,39.778000 L 80.444000,39.778000 z "
//          style="fill:#3b80ae;stroke:none;"
//          id="path868" />
//     </g>
//     <path
//        d="M 45.542000,51.793000 L 24.104000,31.102000 L 62.204000,26.709000 L 45.542000,51.793000 z "
//        style="opacity:0.49999997;fill:none;stroke:#ffffff;"
//        id="path871" />
//     <path
//        d="M 72.325000,28.769000 C 72.730000,30.319000 71.800000,31.905000 70.250000,32.310000 L 57.919000,35.527000 C 56.368000,35.931000 54.782000,35.002000 54.377000,33.451000 L 52.082000,24.650000 C 51.677000,23.099000 52.606000,21.513000 54.158000,21.108000 L 66.488000,17.891000 C 68.039000,17.486000 69.625000,18.416000 70.030000,19.967000 L 72.325000,28.768000 L 72.325000,28.769000 z "
//        id="path873" />
//     <path
//        d="M 36.510000,33.625000 C 37.006000,35.525000 35.865000,37.469000 33.965000,37.965000 L 18.853000,41.908000 C 16.952000,42.404000 15.008000,41.264000 14.513000,39.364000 L 11.699000,28.578000 C 11.203000,26.677000 12.343000,24.734000 14.243000,24.238000 L 29.356000,20.296000 C 31.257000,19.800000 33.201000,20.939000 33.696000,22.840000 L 36.510000,33.626000 L 36.510000,33.625000 z "
//        id="path875" />
//     <path
//        d="M 52.493000,53.208000 C 52.771000,54.273000 52.133000,55.362000 51.068000,55.640000 L 42.600000,57.848000 C 41.536000,58.125000 40.447000,57.488000 40.169000,56.422000 L 38.592000,50.379000 C 38.315000,49.315000 38.952000,48.226000 40.017000,47.947000 L 48.485000,45.738000 C 49.549000,45.461000 50.639000,46.099000 50.916000,47.164000 L 52.493000,53.207000 L 52.493000,53.208000 z "
//        id="path877" />
//   </g>
// </svg>
  
}

void example_arc(Cairo.Context ctx, int width, int height)
{
  float angle1 = 45.0  * (M_PI/180.0);  /* angles are specified */
  float angle2 = 180.0 * (M_PI/180.0);  /* in radians           */
  float xc = 0.5;
  float yc = 0.5;
  float radius = 0.4;

  snippet_normalize (ctx, width, height);

  ctx->arc(xc, yc, radius, angle1, angle2);
  ctx->stroke();

  /* draw helping lines */
  ctx->set_source(1.0, 0.2, 0.2, 0.6);
  ctx->arc(xc, yc, 0.05, 0, 2*M_PI);
  ctx->fill();
  ctx->set_line_width(0.03);
  ctx->arc(xc, yc, radius, angle1, angle1);
  ctx->line_to(xc, yc);
  ctx->arc(xc, yc, radius, angle2, angle2);
  ctx->line_to(xc, yc);
  ctx->stroke();
}

void example_arc_negative(Cairo.Context ctx, int width, int height)
{
  float angle1 = 45.0  * (M_PI/180.0);  /* angles are specified */
  float angle2 = 180.0 * (M_PI/180.0);  /* in radians           */
  float xc = 0.5;
  float yc = 0.5;
  float radius = 0.4;

  snippet_normalize (ctx, width, height);

  ctx->arc_negative(xc, yc, radius, angle1, angle2);
  ctx->stroke();

  /* draw helping lines */
  ctx->set_source(1.0, 0.2, 0.2, 0.6);
  ctx->arc(xc, yc, 0.05, 0, 2*M_PI);
  ctx->fill();
  ctx->set_line_width(0.03);
  ctx->arc(xc, yc, radius, angle1, angle1);
  ctx->line_to(xc, yc);
  ctx->arc(xc, yc, radius, angle2, angle2);
  ctx->line_to(xc, yc);
  ctx->stroke();
}

void example_clip(Cairo.Context ctx, int width, int height)
{
  snippet_normalize (ctx, width, height);

  ctx->arc(0.5, 0.5, 0.3, 0, 2 * M_PI);
  ctx->clip();

  ctx->new_path();  /* current path is not
                       consumed by ctx->clip() */
  ctx->rectangle(0, 0, 1, 1);
  ctx->fill();
  ctx->set_source(0, 1, 0);
  ctx->move_to(0, 0);
  ctx->line_to(1, 1);
  ctx->move_to(1, 0);
  ctx->line_to(0, 1);
  ctx->stroke();
}

void example_clip_image(Cairo.Context ctx, int width, int height)
{
  int              w, h;

  snippet_normalize (ctx, width, height);

  ctx->arc(0.5, 0.5, 0.3, 0, 2*M_PI);
  ctx->clip();
  ctx->new_path(); /* path not consumed by clip()*/

  Image.Image romedalen = Image.load("examples/data/romedalen.png");
  Cairo.Surface image = Cairo.ImageSurface(romedalen);
  w = image->get_width();
  h = image->get_height();

  if (w != 0 && h != 0)
    {
      ctx->scale(1.0/w, 1.0/h);
      ctx->set_source(image, 0, 0);
      ctx->paint();
    }
}

void example_curve_rectangle(Cairo.Context ctx, int width, int height)
{
  /* a custom shape, that could be wrapped in a function */
  float x0	   = 0.1,   /*< parameters like cairo_rectangle */
    y0	   = 0.1,
    rect_width  = 0.8,
    rect_height = 0.8,
    radius = 0.4;   /*< and an approximate curvature radius */

  float x1,y1;

  snippet_normalize (ctx, width, height);

  x1=x0+rect_width;
  y1=y0+rect_height;
  if (!rect_width || !rect_height)
    return;
  if (rect_width/2<radius) {
    if (rect_height/2<radius) {
      ctx->move_to  (x0, (y0 + y1)/2);
      ctx->curve_to (x0 ,y0, x0, y0, (x0 + x1)/2, y0);
      ctx->curve_to (x1, y0, x1, y0, x1, (y0 + y1)/2);
      ctx->curve_to (x1, y1, x1, y1, (x1 + x0)/2, y1);
      ctx->curve_to (x0, y1, x0, y1, x0, (y0 + y1)/2);
    } else {
      ctx->move_to  (x0, y0 + radius);
      ctx->curve_to (x0 ,y0, x0, y0, (x0 + x1)/2, y0);
      ctx->curve_to (x1, y0, x1, y0, x1, y0 + radius);
      ctx->line_to (x1 , y1 - radius);
      ctx->curve_to (x1, y1, x1, y1, (x1 + x0)/2, y1);
      ctx->curve_to (x0, y1, x0, y1, x0, y1- radius);
    }
  } else {
    if (rect_height/2<radius) {
      ctx->move_to  (x0, (y0 + y1)/2);
      ctx->curve_to (x0 , y0, x0 , y0, x0 + radius, y0);
      ctx->line_to (x1 - radius, y0);
      ctx->curve_to (x1, y0, x1, y0, x1, (y0 + y1)/2);
      ctx->curve_to (x1, y1, x1, y1, x1 - radius, y1);
      ctx->line_to (x0 + radius, y1);
      ctx->curve_to (x0, y1, x0, y1, x0, (y0 + y1)/2);
    } else {
      ctx->move_to  (x0, y0 + radius);
      ctx->curve_to (x0 , y0, x0 , y0, x0 + radius, y0);
      ctx->line_to (x1 - radius, y0);
      ctx->curve_to (x1, y0, x1, y0, x1, y0 + radius);
      ctx->line_to (x1 , y1 - radius);
      ctx->curve_to (x1, y1, x1, y1, x1 - radius, y1);
      ctx->line_to (x0 + radius, y1);
      ctx->curve_to (x0, y1, x0, y1, x0, y1- radius);
    }
  }
  ctx->close_path();

  ctx->set_source (0.5,0.5,1);
  ctx->fill_preserve();
  ctx->set_source (0.5, 0, 0, 0.5);
  ctx->stroke();
}

void example_curve_to(Cairo.Context ctx, int width, int height)
{
  float x=0.1,  y=0.5;
  float x1=0.4, y1=0.9,
    x2=0.6, y2=0.1,
    x3=0.9, y3=0.5;

  snippet_normalize (ctx, width, height);

  ctx->move_to (  x, y);
  ctx->curve_to ( x1, y1, x2, y2, x3, y3);

  ctx->stroke ();

  ctx->set_source ( 1,0.2,0.2,0.6);
  ctx->set_line_width ( 0.03);
  ctx->move_to (x,y);   ctx->line_to (x1,y1);
  ctx->move_to (x2,y2); ctx->line_to (x3,y3);
  ctx->stroke ();
}

void example_fill_and_stroke_2(Cairo.Context ctx, int width, int height)
{
  snippet_normalize (ctx, width, height);

  ctx->move_to ( 0.5, 0.1);
  ctx->line_to ( 0.9, 0.9);
  ctx->rel_line_to ( -0.4, 0.0);
  ctx->curve_to ( 0.2, 0.9, 0.2, 0.5, 0.5, 0.5);
  ctx->close_path ();

  ctx->move_to ( 0.25, 0.1);
  ctx->rel_line_to ( 0.2, 0.2);
  ctx->rel_line_to ( -0.2, 0.2);
  ctx->rel_line_to ( -0.2, -0.2);
  ctx->close_path ();

  ctx->set_source ( 0, 0, 1);
  ctx->fill_preserve ();
  ctx->set_source ( 0, 0, 0);
  ctx->stroke ();
}

void example_fill_and_stroke(Cairo.Context ctx, int width, int height)
{
  snippet_normalize (ctx, width, height);

  ctx->move_to ( 0.5, 0.1);
  ctx->line_to ( 0.9, 0.9);
  ctx->rel_line_to ( -0.4, 0.0);
  ctx->curve_to ( 0.2, 0.9, 0.2, 0.5, 0.5, 0.5);
  ctx->close_path ();

  ctx->set_source ( 0, 0, 1);
  ctx->fill_preserve ();
  ctx->set_source ( 0, 0, 0);
  ctx->stroke ();
}

void example_gradient(Cairo.Context ctx, int width, int height)
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
  ctx->arc ( 0.5, 0.5, 0.3, 0, 2 * M_PI);
  ctx->fill ();
}

void example_image(Cairo.Context ctx, int width, int height)
{
  snippet_normalize (ctx, width, height);

  int              w, h;

  Image.Image romedalen = Image.load("examples/data/romedalen.png");
  Cairo.Surface image = Cairo.ImageSurface(romedalen);
  w = image->get_width();
  h = image->get_height();

  ctx->translate ( 0.5, 0.5);
  ctx->rotate ( 45* M_PI/180);
  ctx->scale  ( 1.0/w, 1.0/h);
  ctx->translate ( -0.5*w, -0.5*h);

  ctx->set_source ( image, 0, 0);
  ctx->paint ();
}

void example_image_pattern(Cairo.Context ctx, int width, int height)
{
  int              w, h;

  snippet_normalize (ctx, width, height);

  Image.Image romedalen = Image.load("examples/data/romedalen.png");
  Cairo.Surface image = Cairo.ImageSurface(romedalen);
  w = image->get_width ();
  h = image->get_height ();

  Cairo.Pattern pattern = Cairo.SurfacePattern(image);
  pattern->set_extend(Cairo.CAIRO_EXTEND_REPEAT);

  ctx->translate ( 0.5, 0.5);
  ctx->rotate ( M_PI / 4);
  ctx->scale ( 1 / sqrt (2), 1 / sqrt (2));
  ctx->translate ( - 0.5, - 0.5);

  Cairo.Matrix matrix = Cairo.Matrix();
  matrix->init_scale(w * 5.0, h * 5.0);
  pattern->set_matrix(matrix);

  ctx->set_source ( pattern);

  ctx->rectangle ( 0, 0, 1.0, 1.0);
  ctx->fill ();
}

void example_operator_add(Cairo.Context ctx, int width, int height)
{
  freedesktop_org(ctx, width, height);
  snippet_normalize (ctx, width, height);

  ctx->set_operator (Cairo.CAIRO_OPERATOR_ADD);
  ctx->set_source (1,0,0,0.5);
  ctx->rectangle (0.2,0.2, 0.5,0.5);
  ctx->fill ();
  ctx->set_source (0,1,0);
  ctx->rectangle (0.4,0.4, 0.4,0.4);
  ctx->fill ();
  ctx->set_source (0,0,1);
  ctx->rectangle (0.6,0.6, 0.3,0.3);
  ctx->fill ();
}

void example_koch(Cairo.Context ctx, int width, int height)
{
  string generate_string(string axiom, mapping rules, int iterations)
  {
    string expand_string(string axiom, mapping rules)
    {
      string res = "";
      foreach(axiom;int i;int c)
        if (rules[c])
          res += rules[c];
        else
          res += sprintf("%c", axiom[i]);
      return res;
    };

    string res = axiom;
    for (int i=0;i<iterations;++i)
      {
        res = expand_string(res, rules);
      }
    return res;
  };

  void draw_commands(float x, float y, string cmd, float dangle, float dlen)
  {
    float angle = - M_PI / 3.0;

    ctx->move_to(x, y);

    foreach(cmd;;int c)
      {
        switch(c)
          {
          case 'F':
            ctx->rel_line_to(cos(angle) * dlen, sin(angle) * dlen);
            break;
          case '+':
            angle += dangle;
            break;
          case '-':
            angle -= dangle;
            break;
          }
      }

    ctx->set_line_width(0.002);
    ctx->set_source(0.0,0.5,0.0,0.4);
    ctx->fill_preserve();
    ctx->set_source(1.0,1.0,1.0,1.0);
    ctx->stroke();
  };

  string axiom = "F++F++F";
  int iterations = 4;
  mapping(int:string) rules = ([ 'F' : "F-F++F-F" ]);
  string res = generate_string(axiom, rules, iterations);

  snippet_normalize (ctx, width, height);
  float dangle = M_PI / 3.0;
  float dlen = 0.1;
  float x = 0.10;
  float y = 0.73;
  draw_commands(x, y, res, dangle, 0.01);
}

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

void
handle_key_press(GTK2.Widget widget, array events, int dummy)
{
  switch(events[0]["keyval"])
    {
    case 'z':
      g_sample_functions_index = (g_sample_functions_index - 1) % sizeof(g_sample_functions);
      break;
    case 'x':
      g_sample_functions_index = (g_sample_functions_index + 1) % sizeof(g_sample_functions);
      break;
    case 'q':
      GTK2.main_quit();
      break;
    }

  widget->get_child()->queue_draw();
}

void handle_expose(GTK2.Widget widget, array events, int dummy)
{
  Cairo.Context ctx = Cairo.GTK2Context(widget);

  int width = widget->allocation()->width;
  int height = widget->allocation()->height;

  int xwidth = 30;
  int yheight = 30;

   array(float) col1 = ({ 0.5, 0.5, 0.9 });
   array(float) col2 = ({ 0.4, 0.4, 0.8 });

  ctx->save();
  for(int x=0;x<width;x+=xwidth)
    for(int y=0;y<height;y+=yheight)
      {
        ctx->move_to(x, y);
        ctx->rel_line_to(xwidth, 0);
        ctx->rel_line_to(0, yheight);
        ctx->rel_line_to(-xwidth, 0);
        ctx->close_path();
        if ((x/xwidth+y/yheight) % 2)
          ctx->set_source(col1[0], col1[1], col1[2], 1.0);
        else
          ctx->set_source(col2[0], col2[1], col2[2], 1.0);
        ctx->fill();
      }

  ctx->identity_matrix();

  Cairo.Matrix transform = Cairo.Matrix();
  transform->init_identity();
  transform->translate(width*0.05, height*0.05);
  transform->scale(width*0.9, height*0.9);

  ctx->set_line_width(0.001);
  ctx->transform(transform);
  ctx->move_to(0.0, 0.0);
  ctx->line_to(1.0, 0);
  ctx->line_to(1.0, 1.0);
  ctx->line_to(0.0, 1.0);
  ctx->close_path();
  ctx->set_source(0.0, 0.0, 0.0, 1.0);
  ctx->stroke();
  ctx->restore();

  ctx->save();
  g_sample_functions[g_sample_functions_index](ctx, width, height);
  ctx->restore();

  transform = Cairo.Matrix();
  transform->init_identity();
  transform->translate(width*0.05, height*0.05);
  transform->scale(width*0.9, height*0.9);
  ctx->transform(transform);

  ctx->set_font_size(0.03);

  string txt = sprintf("Use [z] and [x] to cycle through examples (%d/%d)",
                       g_sample_functions_index+1, sizeof(g_sample_functions));

  ctx->move_to(0, 1.0);
  ctx->set_source(0.0, 0.0, 0.0, 1.0);
  ctx->show_text(txt);
  ctx->move_to(0.005, 0.995);
  ctx->set_source(1.0, 1.0, 1.0, 1.0);
  ctx->show_text(txt);

  txt = sprintf("Press [q] to quit");
  ctx->move_to(0, 1.04);
  ctx->set_source(0.0, 0.0, 0.0, 1.0);
  ctx->show_text(txt);
  ctx->move_to(0.005, 1.035);
  ctx->set_source(1.0, 1.0, 1.0, 1.0);
  ctx->show_text(txt);

}

int main()
{
  GTK2.Widget drawing_area;
  GTK2.setup_gtk();
  GTK2.Widget window = GTK2.Window(GTK2.GTK_WINDOW_TOPLEVEL);
  window->set_default_size(WIDTH, HEIGHT);
  window->set_title("cairo samples from http://cairographics.org/samples/");

  foreach(indices(this_program()); ; string o)
    {
      if (has_prefix(o, "example_"))
        {
          write("Adding function %s\n", o);
          function f = lambda(string fun_name) {
                         return lambda(Cairo.Context ctx, int width, int height)
                                { this_program()[fun_name](ctx, width, height); }; };
          g_sample_functions += ({ f(o) });
        }
    }

  window->signal_connect("destroy", GTK2.main_quit);

  drawing_area = GTK2.DrawingArea();
  window->add(drawing_area);

  drawing_area->signal_connect("expose-event", handle_expose);
  window->signal_connect("key-press-event", handle_key_press);
  window->show_all();

  GTK2.main();

  return 0;
}
