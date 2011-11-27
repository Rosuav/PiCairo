

int main()
{
//   Image.Image im = Image.load("examples/lena.png");

//   Image.Image blur = im->apply_matrix(({({-1,-1,-1}),
//                                         ({-1,8,-1}),
//                                         ({-1,-1,-1})}));

  Cairo.Surface surface = Cairo.ImageSurface(Cairo.CAIRO_FORMAT_ARGB32, 100, 100);
  Cairo.Context cr = Cairo.Context(surface);

  cr->move_to(0,0)->line_to(100,100)->line_to(0,100)->close_path();

  cr->set_source(0.4, 0.5, 0.6, 1.0)->fill_preserve();
  cr->set_source(1.0, 1.0, 1.0, 1.0)->set_line_width(1.0)->stroke();

  mapping(string:Image.Image) image = surface->get_image();
  Image.Image im = image["image"];
  Image.Image alpha = image["alpha"];
  Stdio.write_file("examples/output.png",
                   Image.PNG.encode(im, ([ "alpha" : alpha ])));
}
