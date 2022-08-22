#version 430 core
out vec4 color;

in vec2 textureCoord;

uniform sampler2D screenText;

void main()
{
  vec3 tcolor = texture(screenText, textureCoord).rgb;
  color = vec4(tcolor,1.0);
}
