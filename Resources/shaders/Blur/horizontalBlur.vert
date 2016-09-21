attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;
 
#ifdef GL_ES
varying lowp vec4 v_fragmentColor;
varying mediump vec2 v_texCoord;
#else
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
#endif

uniform vec2 textureSize;

varying vec2 blurTexCoords[11];

void main()
{
	gl_Position = CC_PMatrix * a_position;	
	v_fragmentColor = a_color;
	v_texCoord = a_texCoord;

	for(int i = -5; i<= 5; i++)
	{
		blurTexCoords[i + 5] = v_texCoord + vec2(i / textureSize.x, 0.0);
	}
}