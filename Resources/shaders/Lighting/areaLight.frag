#ifdef GL_ES
precision lowp float;
#endif
 
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
varying vec2 v_position;

uniform float brightness;

void main()
{	
	gl_FragColor = v_fragmentColor * brightness;
}