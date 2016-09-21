#ifdef GL_ES
precision lowp float;
#endif
 
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
varying vec2 v_position;

uniform vec2 lightPosition;
uniform float radius;
uniform vec4 lightColor;
uniform float brightness;

void main()
{
	vec2 d = lightPosition - v_position;
	float distance = length(d);

	vec4 c = vec4(1.0, 1.0, 1.0, 1.0);

	if(distance <= radius)
	{
		c = c * (1.0f - distance / radius) * brightness;
	}
	else
	{
		 c = vec4(0.0, 0.0, 0.0, 0.0);
	}	

	gl_FragColor = v_fragmentColor;
}