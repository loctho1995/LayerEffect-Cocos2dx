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
	vec4 texColor = texture2D(CC_Texture0, v_texCoord);
	vec3 normalVector = texColor.xyz;

	vec2 d = lightPosition - v_position;
	vec3 direction = vec3(d.x, d.y, 60.0f);
	float distance = length(direction);

	normalVector = normalize(normalVector);
	direction =  normalize(direction);
	
	float diffuse = max(dot(normalVector, direction), brightness);

	vec4 c = lightColor;
	
	if(distance <= radius)
	{
		float coff = (1.0 - pow(distance / radius, 2)) * brightness;

		c = (c * coff + texColor);

		c.a = texColor.a;
	}
	else
	{
		 c = texColor;
	}	

	gl_FragColor = c;
}