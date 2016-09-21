#ifdef GL_ES
precision lowp float;
#endif
 
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

varying vec2 blurTexCoords[9];

void main()
{
	vec4 texColor = vec4(0.0);
	//using gaussian blur kernel 9
	//Horizontal Blur

	texColor += texture2D(CC_Texture0, blurTexCoords[0]) * 0.102059;
	texColor += texture2D(CC_Texture0, blurTexCoords[1]) * 0.115349;
	texColor += texture2D(CC_Texture0, blurTexCoords[2]) * 0.102059;
	texColor += texture2D(CC_Texture0, blurTexCoords[3]) * 0.115349;
	texColor += texture2D(CC_Texture0, blurTexCoords[4]) * 0.130371;
	texColor += texture2D(CC_Texture0, blurTexCoords[5]) * 0.115349;
	texColor += texture2D(CC_Texture0, blurTexCoords[6]) * 0.102059;
	texColor += texture2D(CC_Texture0, blurTexCoords[7]) * 0.115349;
	texColor += texture2D(CC_Texture0, blurTexCoords[8]) * 0.102059;

	gl_FragColor = texColor;
	//gl_FragColor = vec4(0.4);
}