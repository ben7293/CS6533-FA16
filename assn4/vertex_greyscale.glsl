attribute vec4 position;
attribute vec2 texCoordVar;
varying vec2 varyingTexCoord;

void main(){
	gl_Position = position;
	varyingTexCoord = texCoordVar;
}