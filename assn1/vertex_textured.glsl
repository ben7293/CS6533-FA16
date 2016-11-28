attribute vec4 position;
//attribute vec4 color;
attribute vec2 texCoord;

varying vec2 varyingTexCoord;
//varying vec4 varyingColor;

uniform vec2 modelPosition;

void main() {
	//varyingColor = color;
	varyingTexCoord = texCoord;
	//gl_Position = position;
	gl_Position = vec4(modelPosition.x, modelPosition.y, 0.0, 0.0) + position;
}