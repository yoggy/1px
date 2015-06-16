PShader shader;

void setup() {
  size(200, 200, P2D);
  shader = loadShader("1px.glsl");
}

void draw() {
  shader.set("idx", (millis() / 250) % 4);
  shader.set("resolution", width, height); // ivec2
  filter(shader);
}

