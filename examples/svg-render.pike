/* A very, very simple pike-program capable of rendering the pike.svg
 * logo but that's about it.
 *
 * Author: David Vest (C) 2006
 *
 * This program is in the Public Domain.
 *
 * Dependencies: Public.Parser.XML2 (tested with 1.38)
 *		 PiCairo (under development)
 *
 * TODO:
 *
 * - Parse more transform lists, paths, colors.
 */

constant NS_SVG = "http://www.w3.org/2000/svg";

class Path
{
  class Segment
  {
    constant ABS_MOVE_TO = 0;
    constant REL_MOVE_TO = 1;
    constant ABS_LINE_TO = 2;
    constant REL_LINE_TO = 3;
    constant ABS_CURVE_TO = 4;
    constant REL_CURVE_TO = 5;
    constant CLOSE = 6;

    void create(int|void t, float|void a,
                float|void b, float|void c,
                float|void d, float|void e,
                float|void f)
    {
      data = ([]);
      if (t)
        {
          type = t;
          switch(type)
            {
            case ABS_CURVE_TO:
            case REL_CURVE_TO:
              data["x2"] = e;
              data["y2"] = f;
              data["x1"] = c;
              data["y1"] = d;
            case ABS_LINE_TO:
            case REL_LINE_TO:
              data["y"] = b;
              data["x"] = a;
            }
        }
    }

    int type;
    mapping data;
  };

  void create() { segments = ({}); }
  void abs_move_to(float x, float y) { segments += Segment(Segment.ABS_MOVE_TO, x, y); }
  void abs_line_to(float x, float y) { segments += Segment(Segment.ABS_LINE_TO, x, y); }
  void abs_curve_to(float x, float y, float x1, float y1, float x2, float y2) {
    segments += Segment(Segment.ABS_CURVE_TO, x, y, x1, y1, x2, y2); }
  void rel_move_to(float x, float y) { segments += Segment(Segment.REL_MOVE_TO, x, y); }
  void rel_line_to(float x, float y) { segments += Segment(Segment.REL_LINE_TO, x, y); }
  void rel_curve_to(float x, float y, float x1, float y1, float x2, float y2) {
    segments += Segment(Segment.REL_CURVE_TO, x, y, x1, y1, x2, y2); }
  string _sprintf() { return sprintf("Path(%d segments)", sizeof(segments)); }

  array(Segment) segments;
};

class AttributeParser
{
  float parse_float(string s)
  {
    float f;
    sscanf(s, "%f", f);
    return f;
  }

  array parse_viewbox(string s)
  {
    float x, y, w, h;
    sscanf(s, "%f %f %f %f", x, y, w, h);
    return ({ x, y, w, h });
  }

  array(float) parse_color_tuple(string c)
  {
    /* only black supported */
    if (c == "black")
      return ({ 0.0, 0.0, 0.0, 1.0 });
    else
      {
        int r,g,b;
        if (sscanf(c, "#%02x%02x%02x", r, g, b))
          return ({ r / 255.0, g / 255.0, b / 255.0, 1.0 });
      }
    return 0;
  }

  Cairo.Matrix parse_transform_list(string t)
  {
    if (t == "translate(0,203) scale(0.011412,-0.011412)")
      {
        Cairo.Matrix m = Cairo.Matrix();
        m->init_identity();
        m->translate(0, 203);
        m->scale(0.011412, -0.011412);
        return m;
      }
    else
      return 0;
  }

  Path parse_path(string p) {

    int i;
    int end = sizeof(p);
    Path path = Path();
    Path.Segment current_segment;

    int(0..1) is_wsp(int c)
    {
      /* (#x20 | #x9 | #xD | #xA) */
      return (c == '\n' || c == '\t' || c == ' ' || c == '\r');
    };

    int(0..1) is_number(int c)
    {
      return (c >= '0' && c <= '9');
    };

    int(0..1) is_number_start(int c)
    {
      return (c >= '0' && c <= '9') ||
        c == '-' || c == '.';
    };

    int(0..1) is_drawing_command(int c)
    {
      return (c == 'c' || c == 'C' || c == 'l' || c == 'L' || c == 'z');
    };

    int(0..1) next_char_is_number(int a)
    {
      return is_number(p[a]);
    };

    int(0..1) next_is_number()
    {
      int a = i;
      while (next_char_is_number(a) || p[a] == '.' || (a == i && p[a] == '-'))
        a++;
      return a - i;
    };

    float next_number(int l)
    {
      string substr_number = p[i..(i+l)];
      float a;
      sscanf(substr_number, "%f", a);
      i+=l;
      return a;
    };

    void eat_wsp()
    {
      while (i<end && is_wsp(p[i]))
        i++;
    };

    void rule_comma_wsp()
    {
      while (i<end && is_wsp(p[i]))
        i++;

      if (i<end && p[i] == ',')
        i++;

      while (i<end && is_wsp(p[i]))
        i++;
    };

    int next_non_wsp()
    {
      int a = i;
      while (a<end && is_wsp(p[a]))
        a++;
      return p[a];
    };

    void rule_moveto()
    {
      /* ( "M" | "m" ) wsp* moveto-argument-sequence */

      current_segment = Path.Segment();

      if (p[i] == 'M')
        {
          i++;
          current_segment->type = Path.Segment.ABS_MOVE_TO;
        }
      else if (p[i] == 'm')
        {
          i++;
          current_segment->type = Path.Segment.REL_MOVE_TO;
        }
      else
        throw(sprintf("Syntax error in string '%s' at char %d : Expected '[mM]', got '%c' in '%s'-context", p, i, p[i], p[i-10..i+10]));

      eat_wsp();

      int a;
      if ((a = next_is_number()) <= 0)
        throw("1: Syntax error!");
      float x = next_number(a);

      rule_comma_wsp();

      if ((a = next_is_number()) <= 0)
        throw("2: Syntax error!");
      float y = next_number(a);
      current_segment->data["x"] = x;
      current_segment->data["y"] = y;

      path->segments += ({ current_segment });
      /* TODO: Support implicit line-to after move-to */
    };

    void rule_closepath()
    {
      /* ("Z" | "z") */
      current_segment = Path.Segment();

      if (p[i] == 'Z' || p[i] == 'z')
        {
          i++;
          current_segment->type = Path.Segment.CLOSE;
        }
      else
        throw("Internal look-ahead error");

      path->segments += ({ current_segment });
    };

    void rule_curveto()
    {
      /*  ( "C" | "c" ) wsp* curveto-argument-sequence */
      int type;
      if (p[i] == 'C')
        {
          i++;
          type = Path.Segment.ABS_CURVE_TO;
        }
      else if (p[i] == 'c')
        {
          i++;
          type = Path.Segment.REL_CURVE_TO;
        }
      else
        throw("5: Syntax error");

      while(i<end && is_number_start(next_non_wsp()))
        {
          current_segment = Path.Segment(type);

          int a;
          array v = allocate(6);
          for (int x=0;x<6;x++)
            {
              if (i > 0)
                rule_comma_wsp();
              else
                eat_wsp();
              if ((a = next_is_number()) <= 0)
                throw(sprintf("Syntax error in string '%s' at char %d [%d]: Expected number, got '%c'", p, i, x, p[i]));
              v[x] = next_number(a);
            }

          current_segment->data["x"] = v[0];
          current_segment->data["y"] = v[1];
          current_segment->data["x1"] = v[2];
          current_segment->data["y1"] = v[3];
          current_segment->data["x2"] = v[4];
          current_segment->data["y2"] = v[5];

          path->segments += ({ current_segment });
        }
    };

    void rule_lineto()
    {
      /*  ( "L" | "l" ) wsp* lineto-argument-sequence */
      int type;
      if (p[i] == 'L')
        {
          i++;
          type = Path.Segment.ABS_LINE_TO;
        }
      else if (p[i] == 'l')
        {
          i++;
          type = Path.Segment.REL_LINE_TO;
        }
      else
        throw("6: Syntax error");

      while(i<end && is_number_start(next_non_wsp()))
        {
          current_segment = Path.Segment(type);

          int a;
          array v = allocate(2);
          for (int x=0;x<2;x++)
            {
              if (i > 0)
                rule_comma_wsp();
              else
                eat_wsp();
              if ((a = next_is_number()) <= 0)
                throw(sprintf("Syntax error in string '%s' at char %d : Expected number, got '%c'", p, i, p[i]));
              v[x] = next_number(a);
            }

          current_segment->data["x"] = v[0];
          current_segment->data["y"] = v[1];

          path->segments += ({ current_segment });
        }
    };

    void rule_drawto_command()
    {
      /* closepath
         | lineto
         | horizontal-lineto
         | vertical-lineto
         | curveto
         | smooth-curveto
         | quadratic-bezier-curveto
         | smooth-quadratic-bezier-curveto
         | elliptical-arc */
      /* some look-ahead magic is performed here */

      int cmd = next_non_wsp();
      if (cmd == 'z' || cmd == 'Z')
        rule_closepath();
      else if (cmd == 'c' || cmd == 'C')
        rule_curveto();
      else if (cmd == 'l' || cmd == 'L')
        rule_lineto();

      /* more here, of course. */
    };

    void rule_drawto_commands()
    {
      /*   drawto-command
         | drawto-command wsp* drawto-commands */

      while(i < end && is_drawing_command(next_non_wsp()))
        {
          rule_drawto_command();
          eat_wsp();
        }
    };

    void rule_moveto_drawto_command_group()
    {
      /* moveto wsp* drawto-commands? */
      rule_moveto();
      eat_wsp();
      rule_drawto_commands();
    };

    void rule_moveto_drawto_command_groups()
    {
      /* moveto-drawto-command-group
         | moveto-drawto-command-group wsp* moveto-drawto-command-groups */
      while(i < end)
        {
          rule_moveto_drawto_command_group();
          eat_wsp();
        }
    };

    void rule_svg_path() {
      /* wsp* moveto-drawto-command-groups? wsp* */
      eat_wsp();
      if (i < end)
        rule_moveto_drawto_command_groups();
      eat_wsp();
    };

    rule_svg_path();
    return path;
  }
};

class Traverser
{
  private Cairo.Context ctx;
  AttributeParser parser;

  class State
  {
    int(0..1) enable_stroke;
    int(0..1) enable_fill;

    array(float) fill_color;
    array(float) stroke_color;
    Cairo.Matrix matrix;

    float stroke_width;

    void create()
    {
      stroke_width = 1.0;
    }

    void cascade(State other)
    {
      enable_fill = other->enable_fill;
      enable_stroke = other->enable_stroke;
      fill_color = other->fill_color;
      stroke_color = other->stroke_color;
      stroke_width = other->stroke_width;
    }
  };

  array(State) states;
  State state;
  mapping attributes;

  void create(Cairo.Context c)
  {
    ctx = c;
    parser = AttributeParser();
    states = ({});
  }

  void apply_attribute_to_state()
  {
    if (!attributes)
      return;

    if (attributes["fill"])
      {
        if (attributes["fill"] == "none")
          {
            state->enable_fill = 0;
            state->fill_color = 0;
          }
        else
          {
            mixed e = catch {
                state->fill_color = parser->parse_color_tuple(attributes["fill"]);
                state->enable_fill = 1;
              };
            if (e) { write("fill: %O", e); }
          }
      }

    if (attributes["stroke"])
      {
        if (attributes["stroke"] == "none")
          {
            state->enable_stroke = 0;
            state->stroke_color = 0;
          }
        else
          {
            mixed e = catch {
                state->stroke_color = parser->parse_color_tuple(attributes["stroke"]);
                state->enable_stroke = 1;
              };
            if (e) { write("stroke: %O", e); }
          }
      }

    if (attributes["transform"])
      {
        mixed e = catch {
            state->matrix = parser->parse_transform_list(attributes["transform"]);
          };
        if (e) { write("transform: %O", e); }
      }

    if (attributes["stroke-width"])
      {
        mixed e = catch {
            state->stroke_width = parser->parse_float(attributes["stroke-width"]);
          };
        if (e) { write("stroke-width: %O", e); }
      }

    if (attributes["viewBox"])
      {
        mixed e = catch {
            array box = parser->parse_viewbox(attributes["viewBox"]);
            werror("viewBox not implemented\n");
          };
        if (e) { write("viewBox: %O", e); }
      }
  }

  void apply_state_to_context()
  {
    /* nothing yet */
    if (state->matrix)
      {
        ctx->transform(state->matrix);
      }
  }

  int(0..1) enter(Public.Parser.XML2.Node node)
  {
    if (node->get_ns() != NS_SVG)
      return 0;

    attributes = node->get_attributes();
    ctx->save();

    State new_state = State();

    if (state)
      new_state->cascade(state);

    states += ({ new_state });
    state = new_state;

    apply_attribute_to_state();
    apply_state_to_context();

    return 1;
  }

  void draw_path_segment(Path.Segment seg)
  {
    switch(seg->type)
      {
      case Path.Segment.ABS_MOVE_TO:
        ctx->move_to(seg->data["x"], seg->data["y"]);
        break;
      case Path.Segment.REL_MOVE_TO:
        ctx->rel_move_to(seg->data["x"], seg->data["y"]);
        break;
      case Path.Segment.ABS_LINE_TO:
        ctx->line_to(seg->data["x"], seg->data["y"]);
        break;
      case Path.Segment.REL_LINE_TO:
        ctx->rel_line_to(seg->data["x"], seg->data["y"]);
        break;
      case Path.Segment.ABS_CURVE_TO:
        ctx->curve_to(seg->data["x"], seg->data["y"],
                      seg->data["x1"], seg->data["y1"],
                      seg->data["x2"], seg->data["y2"]);
        break;
      case Path.Segment.REL_CURVE_TO:
        ctx->rel_curve_to(seg->data["x"], seg->data["y"],
                          seg->data["x1"], seg->data["y1"],
                          seg->data["x2"], seg->data["y2"]);
        break;
      }
  }

  void execute(Public.Parser.XML2.Node node)
  {
    switch(node->get_node_name())
      {
      case "path":
        if (attributes["d"])
          {
            mixed e = catch {
              Path p = parser->parse_path(attributes["d"]);

              foreach(p->segments;; Path.Segment seg)
                {
                  draw_path_segment(seg);
                }

              if (state->enable_fill && state->enable_stroke)
                {
                  array c = state->fill_color;
                  ctx->set_source(c[0], c[1], c[2], c[3]);
                  ctx->fill_preserve();
                  c = state->stroke_color;
                  ctx->set_source(c[0], c[1], c[2], c[3]);
                  ctx->set_line_width(state->stroke_width);
                  ctx->stroke();
                }
              else if (state->enable_fill)
                {
                  array c = state->fill_color;
                  ctx->set_source(c[0], c[1], c[2], c[3]);
                  ctx->fill();
                }
              else if (state->enable_stroke)
                {
                  array c = state->stroke_color;
                  ctx->set_source(c[0], c[1], c[2], c[3]);
                  ctx->stroke();
                }
            };
            if (e != 0) { write("Path: %O\n", e); }
          }
        break;
      }
  }

  void leave(Public.Parser.XML2.Node node)
  {
    ctx->restore();
    states = states[0..sizeof(states)-2];
  }

  void traverse_children(object o)
  {
    if (o->children())
      foreach(o->children();;object c)
        {
          traverse_node(c);
        }
  }

  void traverse_node(Public.Parser.XML2.Node node)
  {
    if (enter(node))
      {
        execute(node);
        traverse_children(node);
        leave(node);
      }
  }
};

array get_size(Public.Parser.XML2.Node root, float def_w, float def_h)
{
  mapping attributes = root->get_attributes();
  float w = def_w, h = def_h;
  if (attributes)
    {
      if (attributes["width"])
        {
          sscanf(attributes["width"], "%f", w);
        }

      if (attributes["height"])
        {
          sscanf(attributes["height"], "%f", h);
        }

      if (!attributes["width"] && !attributes["height"] && attributes["viewBox"])
        {
          float x, y;
          sscanf(attributes["viewBox"], "%f %f %f %f", x, y, w, h);
        }
    }

  return ({ w, h });
}

int main(int argc, array(string) argv)
{
  Public.Parser.XML2.Node root = Public.Parser.XML2.parse_xml(Stdio.read_file(argv[1]));

  [float width, float height] = get_size(root, 350.0, 350.0);

  Cairo.ImageSurface image_surface = Cairo.ImageSurface(Cairo.CAIRO_FORMAT_ARGB32, (int)(ceil(width)), (int)(ceil(height)));
  Cairo.Context ctx = Cairo.Context(image_surface);

  Traverser t = Traverser(ctx);
  t->traverse_node(root);

  mapping im = image_surface->get_image();
  Image.Image image = im["image"];
  Image.Image alpha = im["alpha"];
  Stdio.write_file("output.png",
                   Image.PNG.encode(image, ([ "alpha" : alpha ])));
}

