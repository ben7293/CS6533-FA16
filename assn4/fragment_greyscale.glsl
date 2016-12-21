uniform sampler2D screenFramebuffer;
varying vec2 varyingTexCoord;

void main()
{
    vec4 texColor = texture2D(screenFramebuffer, varyingTexCoord);
    float brightness = (texColor.x + texColor.y + texColor.z) / 3.0;
    gl_FragColor = vec4(brightness, brightness, brightness, 1.0);
}