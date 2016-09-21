#ifdef GL_ES
precision lowp float;
#endif
 
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform float contrast;

void main()
{
	vec4 texColor1 = texture2D(CC_Texture0, v_texCoord);
	vec4 texColor2 = texture2D(CC_Texture1, v_texCoord);

	gl_FragColor = (texColor1 + texColor2 * contrast);
}