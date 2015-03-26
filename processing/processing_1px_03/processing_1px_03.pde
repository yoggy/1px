PImage img;

void setup() {
  size(200, 200);
  frameRate(4);
  
  // https://processing.org/reference/createImage_.html
  img = createImage(200, 200, ARGB);
}

void draw() {
  img.loadPixels();

  // clear image
  for (int i = 0; i < img.pixels.length; i++) {
    img.pixels[i] = 0xff000000;
  }

  // draw 1px
  int x = 100;
  int y = 100;
  int w = img.width;

  int idx = x + y * w;

  switch(frameCount % 4) {
    case 0:
      img.pixels[idx] = 0xffff0000;
      break;
    case 1:
      img.pixels[idx] = 0xff00ff00;
      break;
    case 2:
      img.pixels[idx] = 0xff0000ff;
      break;
    case 3:
      img.pixels[idx] = 0xff000000;
      break;
  }
  img.updatePixels();

  image(img, 0, 0);
}


