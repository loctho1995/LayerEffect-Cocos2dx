#ifdef GL_ES
precision lowp float;
#endif
 
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform float threshold;

void main()
{
	vec4 texColor = texture2D(CC_Texture0, v_texCoord);
		//luma conversion
	float brightness = (texColor.r * 0.2126) + (texColor.g * 0.7152) + (texColor.b * 0.0722);

	if(threshold == 0)
	{
		gl_FragColor = texColor * brightness;
	}
	else
	{
		if(brightness > threshold)
		{
			gl_FragColor = texColor;
		}
		else
		{
			gl_FragColor = vec4(0.0);
		}
	}
}