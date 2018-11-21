#version 400

in vec4 color;
in vec2 tex;
in float lighting;
in float ambience;

uniform int applyTexture;
uniform sampler2D texturer;

out vec4 frag_color;

void main () {
  if(applyTexture == 0)
  	frag_color = color;
  else{
  	frag_color = max(lighting, ambience) * texture(texturer, tex);
  }
}
