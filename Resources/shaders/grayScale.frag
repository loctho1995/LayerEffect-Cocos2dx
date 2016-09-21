#ifdef GL_ES
precision lowp float;
#endif
 
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

void main()
{
	vec4 texColor = texture2D(CC_Texture0, v_texCoord);

	float luma = texColor.r * 0.2126f + texColor.g * 0.7152f + texColor.b * 0.0722f;

	gl_FragColor = vec4(luma, luma, luma, texColor.a);
}