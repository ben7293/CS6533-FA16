uniform sampler2D screenFramebuffer;
varying vec2 varyingTexCoord;

void main(){
    vec4 color = texture2D(screenFramebuffer, varyingTexCoord);
    float intensity = (color.x + color.y + color.z) / 3.0;
    gl_FragColor = vec4(intensity, intensity, intensity, 1.0);
}