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

varying vec2 blurTexCoords[9];

void main()
{
	gl_Position = CC_PMatrix * a_position;	
	v_fragmentColor = a_color;
	v_texCoord = a_texCoord;

	int k = 0;

	for(int i = -1; i <= 1; i++)
	{
		for(int j = -1; j <= 1; j++)
		{
			blurTexCoords[k] = v_texCoord + vec2(i / 250.0, j / 250.0);
			k++;
		}
	}
}