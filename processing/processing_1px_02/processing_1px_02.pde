void setup() {
  size(200, 200);
  frameRate(4);
}

void draw() {
  background(0, 0, 0);
  
  switch(frameCount % 4) {
    case 0:
      fill(255, 0, 0);
      break;
    case 1:
      fill(0, 255, 0);
      break;
    case 2:
      fill(0, 0, 255);
      break;
    case 3:
      fill(0, 0, 0);
      break;
  }
  noStroke();
  rect(100, 100, 2, 2); // <- mysterious
}


