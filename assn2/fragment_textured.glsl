varying vec4 varyingNormal;

uniform vec3 uColor;

void main() {
	float diffuse = 0.3 + max(0.0, dot(varyingNormal, vec4(-0.5773, 0.5773, 0.5773, 0.0)));
	vec3 intensity = uColor * diffuse;
	gl_FragColor = vec4(intensity.xyz, 1.0);
}