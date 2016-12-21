attribute vec4 position;
attribute vec2 texCoord;
varying vec2 varyingTexCoord;

void main()
{
	gl_Position = position;
	varyingTexCoord = texCoord;
}