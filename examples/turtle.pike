/*
 * This is a very simple turtle
 * grapics engine. It demonstrates
 * how to use pike, gtk and cairo
 * together to display some
 * graphics.
 */

#ifndef WIDTH
# define WIDTH 400
#endif

#ifndef HEIGHT
# define HEIGHT 400
#endif

#define ROOT "/home/davve/svn/pike/cairo/examples/"

#define M_PI Math.pi

GTK2.Label g_label;
GTK2.Label g_status;

array(function) g_sample_functions = ({ });

int g_sample_functions_index = 0;

void snippet_normalize(Cairo.Context ctx, int width, int height)
{
  ctx->identity_matrix();

  Cairo.Matrix transform = Cairo.Matrix();
  transform->init_identity();
  transform->translate(width*0.05, height*0.05);
  transform->scale(width*0.9, height*0.9);

  ctx->transform(transform);

  ctx->move_to(0.0, 0.0);
  ctx->line_to(1.0, 0);
  ctx->line_to(1.0, 1.0);
  ctx->line_to(0.0, 1.0);
  ctx->close_path();
  ctx->clip();
}

class State
{
  float angle;
  float x;
  float y;
  float lw;

  static void create(float _a, float _x, float _y) { angle = _a; x = _x; y = _y; lw = 0.01; }

  State Copy() {
    State s = State(angle, x, y);
    s->lw = lw;
    return s;
  }

  void Set(State s) {
    angle = s->angle;
    x = s->x;
    y = s->y;
    lw = s->lw;
  }
};

class Param
{
  static void create(string _ax, float _l, int _i, float _an) { dlen = _l; iterations = _i; dangle = _an; axiom = _ax; }

  float dlen;
  int iterations;
  float dangle;
  string axiom;
};

class RuleSystem
{
  string name;
  string description;
  State state;
  Param param;
  ADT.Stack stack;
  mapping(int:array(mapping(string:string|function(State,Param:void)))) rules;

  function setup_draw;
  function finalize_draw;
};

RuleSystem rs_koch_snowflake_1()
{
  RuleSystem rs = RuleSystem();
  rs->name = "Koch's snowflake (stroked)";
  rs->description = #"A classic! This pattern starts with"
    "a triangle and adds to each edge a"
    "triangle, and so on.";
  rs->state = State(- M_PI / 3.0, 0.10, 0.73);
  rs->param = Param("F++F++F", 0.010, 4, M_PI / 3.0);
  rs->rules = ([ 'F' : ({ ([ "expand" : "F-F++F-F",
                             "closure" : lambda(Cairo.Context ctx, State s, Param p, ADT.Stack stack) {
                                           ctx->move_to(s->x,s->y);
                                           s->x += cos(s->angle) * p->dlen;
                                           s->y += sin(s->angle) * p->dlen;
                                           ctx->line_to(s->x,s->y);
                                           ctx->stroke();
                                         }
                          ]) }),
                 '+' : ({ ([ "closure" : lambda(Cairo.Context ctx, State s, Param p, ADT.Stack stack)
                                         { s->angle += p->dangle; } ]) }),
                 '-' : ({ ([ "closure" : lambda(Cairo.Context ctx, State s, Param p, ADT.Stack stack)
                                         { s->angle -= p->dangle; } ]) }) ]);
  rs->setup_draw = lambda(Cairo.Context ctx, int width, int height, Param p) {
                     ctx->set_line_width(0.002);
                   };
  rs->stack = ADT.Stack();
  rs->finalize_draw = lambda(Cairo.Context ctx, int width, int height, Param p) {
                      };
  return rs;
}

RuleSystem rs_koch_snowflake_2()
{
  RuleSystem rs = RuleSystem();
  rs->name = "Koch's snowflake (filled)";
  rs->description = #"Koch again! This pattern is drawn as one path and both filled and stroked";
  rs->state = State(- M_PI / 3.0, 0.10, 0.73);
  rs->param = Param("F++F++F", 0.010, 4, M_PI / 3.0);
  rs->rules = ([ 'F' : ({ ([ "expand" : "F-F++F-F",
                             "closure" : lambda(Cairo.Context ctx, State s, Param p, ADT.Stack stack) {
                                           s->x += cos(s->angle) * p->dlen;
                                           s->y += sin(s->angle) * p->dlen;
                                           ctx->line_to(s->x,s->y);
                                         }
                          ]) }),
                 '+' : ({ ([ "closure" : lambda(Cairo.Context ctx, State s, Param p, ADT.Stack stack)
                                         { s->angle += p->dangle; } ]) }),
                 '-' : ({ ([ "closure" : lambda(Cairo.Context ctx, State s, Param p, ADT.Stack stack)
                                         { s->angle -= p->dangle; } ]) }) ]);
  rs->stack = ADT.Stack();
  rs->setup_draw = lambda(Cairo.Context ctx, int width, int height, Param p) {
                     ctx->set_line_width(0.002);
                   };
  rs->finalize_draw = lambda(Cairo.Context ctx, int width, int height, Param p) {
                        ctx->set_source(0.1,0.3,0.5,0.5);
                        ctx->fill_preserve();
                        ctx->set_source(0.0,0.0,0.0,1.0);
                        ctx->stroke();
                      };
  return rs;
}

RuleSystem rs_square()
{
  RuleSystem rs = RuleSystem();
  rs->name = "Squarey";
  rs->description = "Found this on the internet";
  rs->state = State(0.0, 0.23, 0.23);
  rs->param = Param("F+F+F+F", 0.008, 3, M_PI / 2.0);
  rs->rules = ([ 'F' : ({ ([ "expand" : "F+F-F-FF+F+F-F",
                             "closure" : lambda(Cairo.Context ctx, State s, Param p, ADT.Stack stack) {
                                           s->x += cos(s->angle) * p->dlen;
                                           s->y += sin(s->angle) * p->dlen;
                                           ctx->line_to(s->x,s->y);
                                         }
                          ]) }),
                 '+' : ({ ([ "closure" : lambda(Cairo.Context ctx, State s, Param p, ADT.Stack stack)
                                         { s->angle += p->dangle; } ]) }),
                 '-' : ({ ([ "closure" : lambda(Cairo.Context ctx, State s, Param p, ADT.Stack stack)
                                         { s->angle -= p->dangle; } ]) }) ]);
  rs->stack = ADT.Stack();
  rs->setup_draw = lambda(Cairo.Context ctx, int width, int height, Param p) {
                     ctx->set_line_width(0.002);
                   };
  rs->finalize_draw = lambda(Cairo.Context ctx, int width, int height, Param p) {
                        ctx->set_source(0.1,0.3,0.5,0.5);
                        ctx->fill_preserve();
                        ctx->set_source(0.0,0.0,0.0,1.0);
                        ctx->stroke();
                      };
  return rs;
}

RuleSystem rs_branch()
{
  RuleSystem rs = RuleSystem();
  rs->name = "Branchy";
  rs->description = "This is the first and simplest branching thigny";
  rs->state = State(0.0, 0.5, 0.95);
  rs->param = Param("---F", 0.028, 4, M_PI / 6.0);
  rs->rules = ([ 'F' : ({ ([ "expand" : "F[-F]F[+F][F]",
                             "closure" : lambda(Cairo.Context ctx, State s, Param p, ADT.Stack stack) {
                                           ctx->set_line_width(s->lw);
                                           ctx->move_to(s->x,s->y);
                                           s->lw *= 0.90;
                                           s->x += cos(s->angle) * p->dlen;
                                           s->y += sin(s->angle) * p->dlen;
                                           ctx->line_to(s->x,s->y);
                                           ctx->stroke();
                                         }
                          ]) }),
                 '+' : ({ ([ "closure" : lambda(Cairo.Context ctx, State s, Param p, ADT.Stack stack)
                                         { s->angle += p->dangle; } ]) }),
                 '-' : ({ ([ "closure" : lambda(Cairo.Context ctx, State s, Param p, ADT.Stack stack)
                                         { s->angle -= p->dangle; } ]) }),
                 '[' : ({ ([ "closure" : lambda(Cairo.Context ctx, State s, Param p, ADT.Stack stack)
                                         { stack->push(s->Copy()); } ]) }),
                 ']' : ({ ([ "closure" : lambda(Cairo.Context ctx, State s, Param p, ADT.Stack stack)
                                         { s->Set(stack->pop()); } ]) })  ]);
  rs->stack = ADT.Stack();
  rs->setup_draw = lambda(Cairo.Context ctx, int width, int height, Param p) {
                     ctx->set_line_width(0.002);
                   };
  rs->finalize_draw = lambda(Cairo.Context ctx, int width, int height, Param p) {
                        ctx->set_source(0.1,0.3,0.5,0.5);
                        ctx->fill_preserve();
                        ctx->set_source(0.0,0.0,0.0,1.0);
                        ctx->stroke();
                      };
  return rs;
}

RuleSystem rs_bush()
{
  RuleSystem rs = RuleSystem();
  rs->name = "Bushy";
  rs->description = "This is the another find on the net";
  rs->state = State(0.0, 0.5, 0.95);
  rs->param = Param("---F", 0.018, 4, M_PI / 6.0);
  rs->rules = ([ 'F' : ({ ([ "expand" : "FF-[-F+F+F]+[+F-F-F]",
                             "closure" : lambda(Cairo.Context ctx, State s, Param p, ADT.Stack stack) {
                                           ctx->set_line_width(s->lw);
                                           s->lw *= 0.95;

                                           ctx->move_to(s->x,s->y);
                                           s->x += cos(s->angle) * p->dlen;
                                           s->y += sin(s->angle) * p->dlen;
                                           ctx->line_to(s->x,s->y);
                                           ctx->stroke();
                                         }
                          ]) }),
                 '+' : ({ ([ "closure" : lambda(Cairo.Context ctx, State s, Param p, ADT.Stack stack)
                                         { s->angle += p->dangle; } ]) }),
                 '-' : ({ ([ "closure" : lambda(Cairo.Context ctx, State s, Param p, ADT.Stack stack)
                                         { s->angle -= p->dangle; } ]) }),
                 '[' : ({ ([ "closure" : lambda(Cairo.Context ctx, State s, Param p, ADT.Stack stack)
                                         { stack->push(s->Copy()); } ]) }),
                 ']' : ({ ([ "closure" : lambda(Cairo.Context ctx, State s, Param p, ADT.Stack stack)
                                         { s->Set(stack->pop()); } ]) })  ]);
  rs->stack = ADT.Stack();
  rs->setup_draw = lambda(Cairo.Context ctx, int width, int height, State s, Param p) {
                     s->lw = 0.01;
                   };
  rs->finalize_draw = lambda(Cairo.Context ctx, int width, int height, State s, Param p) {
                        ctx->set_source(0.1,0.3,0.5,0.5);
                        ctx->fill_preserve();
                        ctx->set_source(0.0,0.0,0.0,1.0);
                        ctx->stroke();
                      };
  return rs;
}

void do_turtle(Cairo.Context ctx, RuleSystem rs, int width, int height)
{
  string generate_string()
  {
    string expand_string(string axiom)
    {
      string res = "";
      foreach(axiom;int i;int c)
        {
          if (rs->rules[c])
            {
              array(mapping(string:string|function)) c_rules = rs->rules[c];
              mapping(string:string|function) rule = c_rules[random(sizeof(c_rules))];
              if (stringp(rule["expand"]))
                {
                  res += rule["expand"];
                  continue;
                }
            }
      
          // default action
          res += sprintf("%c", c);
        }
      return res;
    };

    string res = rs->param->axiom;

    for (int i=0;i<rs->param->iterations;++i)
        res = expand_string(res);
    return res;
  };

  void draw(string cmd)
  {
    rs->setup_draw(ctx, width, height, rs->state, rs->param);
    ctx->move_to(rs->state->x, rs->state->y);
    foreach(cmd;;int c)
      {
        array(mapping(string:string|function)) c_rules = rs->rules[c];
        mapping(string:string|function) rule = c_rules[random(sizeof(c_rules))];
        if (rule && functionp(rule["closure"]))
          {
            c_rules["closure"](ctx, rs->state, rs->param, rs->stack);
          }
      }
    rs->finalize_draw(ctx, width, height, rs->state, rs->param);
  };

  string res = generate_string();

  snippet_normalize (ctx, width, height);
  draw(res);

  g_label->set_text(rs->name);
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
  g_status->set_text(txt);
}

int main()
{
  GTK2.Widget drawing_area;
  GTK2.setup_gtk();
  GTK2.Widget window = GTK2.Window(GTK2.WINDOW_TOPLEVEL);
  window->set_title("Turtle graphics");

  foreach(indices(this_program()); ; mixed o)
    {
      if (has_prefix(o, "rs_"))
        {
          write("Adding function %s\n", o);

          function f = lambda(string fun_name) {
                         return lambda(Cairo.Context ctx, int width, int height)
                                {
                                  do_turtle(ctx, this_program()[fun_name](), width, height);
                                };
                           };
          g_sample_functions += ({ f(o) });
        }
    }

  window->signal_connect("destroy", GTK2.main_quit);

  drawing_area = GTK2.DrawingArea();

  drawing_area->set_size_request(400,400);

  GTK2.Vbox vbox = GTK2.Vbox(0,0);

  g_status = GTK2.Label();
  g_status->set_size_request(400,20);
  g_status->set_width_chars(30);
  g_status->set_max_width_chars(40);

  g_status->set_text("");

  g_label = GTK2.Label();
  g_label->set_size_request(400,20);
  g_label->set_width_chars(30);
  g_label->set_max_width_chars(40);
  g_label->set_text("");

  vbox->pack_end(g_status, 0, 1, 0);
  vbox->pack_end(g_label, 0, 1, 0);
  vbox->pack_end(drawing_area, 1, 1, 0);

  window->add(vbox);

  drawing_area->signal_connect("expose-event", handle_expose);
  window->signal_connect("key-press-event", handle_key_press);
  window->show_all();

  GTK2.main();

  return 0;
}