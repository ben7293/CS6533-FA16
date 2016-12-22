precision mediump float;
uniform sampler2D screenFramebuffer;
varying vec2 varyingTexCoord;

void main(void)
{
	const float blurSize = 0.004;
	vec4 sum = vec4(0.0);
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 16.0*blurSize, varyingTexCoord.y)) * 0.000004;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 15.0*blurSize, varyingTexCoord.y)) * 0.000012;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 14.0*blurSize, varyingTexCoord.y)) * 0.00004;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 13.0*blurSize, varyingTexCoord.y)) * 0.00012;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 12.0*blurSize, varyingTexCoord.y)) * 0.000331;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 11.0*blurSize, varyingTexCoord.y)) * 0.000841;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 10.0*blurSize, varyingTexCoord.y)) * 0.001971;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 9.0*blurSize, varyingTexCoord.y)) * 0.004258;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 8.0*blurSize, varyingTexCoord.y)) * 0.008483;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 7.0*blurSize, varyingTexCoord.y)) * 0.015583;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 6.0*blurSize, varyingTexCoord.y)) * 0.026396;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 5.0*blurSize, varyingTexCoord.y)) * 0.04123;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 4.0*blurSize, varyingTexCoord.y)) * 0.059384;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 3.0*blurSize, varyingTexCoord.y)) * 0.07887;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 2.0*blurSize, varyingTexCoord.y)) * 0.096593;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x - 1.0*blurSize, varyingTexCoord.y)) * 0.109084;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x, varyingTexCoord.y)) * 0.113597;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 16.0*blurSize, varyingTexCoord.y)) * 0.000004;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 15.0*blurSize, varyingTexCoord.y)) * 0.000012;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 14.0*blurSize, varyingTexCoord.y)) * 0.00004;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 13.0*blurSize, varyingTexCoord.y)) * 0.00012;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 12.0*blurSize, varyingTexCoord.y)) * 0.000331;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 11.0*blurSize, varyingTexCoord.y)) * 0.000841;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 10.0*blurSize, varyingTexCoord.y)) * 0.001971;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 9.0*blurSize, varyingTexCoord.y)) * 0.004258;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 8.0*blurSize, varyingTexCoord.y)) * 0.008483;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 7.0*blurSize, varyingTexCoord.y)) * 0.015583;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 6.0*blurSize, varyingTexCoord.y)) * 0.026396;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 5.0*blurSize, varyingTexCoord.y)) * 0.04123;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 4.0*blurSize, varyingTexCoord.y)) * 0.059384;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 3.0*blurSize, varyingTexCoord.y)) * 0.07887;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 2.0*blurSize, varyingTexCoord.y)) * 0.096593;
	sum += texture2D(screenFramebuffer, vec2(varyingTexCoord.x + 1.0*blurSize, varyingTexCoord.y)) * 0.109084;
	gl_FragColor = sum;
}
