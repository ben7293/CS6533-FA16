attribute vec4 position;
attribute vec2 texCoord;

attribute vec4 normal;
attribute vec4 binormal;
attribute vec4 tangent;

varying vec2 varyingTexCoord;
varying vec3 varyingPosition;
varying mat3 varyingTBNMatrix;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

void main() {
	varyingTexCoord = texCoord;
	//varyingNormal = normalize((normalMatrix * normal).xyz);
	vec4 p = modelViewMatrix * position;
	varyingPosition = p.xyz;
	varyingTBNMatrix = mat3(normalize((normalMatrix * tangent).xyz), normalize((normalMatrix * binormal).xyz), normalize((normalMatrix * normal).xyz));
	gl_Position = projectionMatrix * p;
}