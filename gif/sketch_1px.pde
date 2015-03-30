//
// 1px animation image generator for Processing.org
//
size(200, 200);
background(0);

stroke(255, 0, 0);
point(100, 100);
save("pic01.png");

stroke(0, 255, 0);
point(100, 100);
save("pic02.png");

stroke(0, 0, 255);
point(100, 100);
save("pic03.png");

stroke(0, 0, 0);
point(100, 100);
save("pic04.png");

// create color map
stroke(255, 0, 0);
point(0, 0);
stroke(0, 255, 0);
point(1, 0);
stroke(0, 0, 255);
point(2, 0);
save("map.png");

try {
  Runtime.getRuntime().exec("convert -delay 25 -loop 0 -layers optimize -map map.png pic01.png pic02.png pic03.png pic04.png 1px.gif");
}
catch(IOException e) {
  println(e);
  e.printStackTrace();
}

