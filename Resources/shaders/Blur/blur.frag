#ifdef GL_ES
precision lowp float;
#endif
 
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

varying vec2 blurTexCoords[11];

void main()
{

	vec4 texColor = vec4(0.0);
	//using gaussian blur kernel 11
	//Horizontal Blur

	texColor += texture2D(CC_Texture0, blurTexCoords[0]) * 0.0093;
	texColor += texture2D(CC_Texture0, blurTexCoords[1]) * 0.028002;
	texColor += texture2D(CC_Texture0, blurTexCoords[2]) * 0.065984;
	texColor += texture2D(CC_Texture0, blurTexCoords[3]) * 0.121703;
	texColor += texture2D(CC_Texture0, blurTexCoords[4]) * 0.175713;
	texColor += texture2D(CC_Texture0, blurTexCoords[5]) * 0.198596;
	texColor += texture2D(CC_Texture0, blurTexCoords[6]) * 0.175713;
	texColor += texture2D(CC_Texture0, blurTexCoords[7]) * 0.121703;
	texColor += texture2D(CC_Texture0, blurTexCoords[8]) * 0.065984;
	texColor += texture2D(CC_Texture0, blurTexCoords[9]) * 0.028002;
	texColor += texture2D(CC_Texture0, blurTexCoords[10]) * 0.0093;

	gl_FragColor = texColor;
}