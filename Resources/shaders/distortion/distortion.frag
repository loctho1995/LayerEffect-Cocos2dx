#ifdef GL_ES
precision lowp float;
#endif
 
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform float weightX;
uniform float weightY;


void main()
{
	//dudv map texture
	vec4 texColor2 = texture2D(CC_Texture0, v_texCoord);

	vec2 distortion1 = (texColor2.rg * 2.0 - 1.0);
	distortion1 = vec2(distortion1.x * weightX, distortion1.y * weightY);

	vec2 newCoord = v_texCoord + distortion1;

	gl_FragColor = texture2D(CC_Texture0, newCoord);
}