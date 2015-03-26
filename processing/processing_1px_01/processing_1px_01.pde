void setup() {
  size(200, 200);
  frameRate(4);
}

void draw() {
  background(0, 0, 0);
  
  switch(frameCount % 4) {
    case 0:
      stroke(255, 0, 0);
      break;
    case 1:
      stroke(0, 255, 0);
      break;
    case 2:
      stroke(0, 0, 255);
      break;
    case 3:
      stroke(0, 0, 0);
      break;
  }
  strokeWeight(1);
  point(100, 100);
}


