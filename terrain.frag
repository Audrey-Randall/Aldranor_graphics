//Bump map fragment shader

uniform sampler2D mapTex;
varying float h;
varying vec4 v;

void main(void)
{
   gl_FragColor = vec4(h, h, h, 1);
}
