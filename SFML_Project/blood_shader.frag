// rewind.frag
uniform sampler2D currentTexture;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    
    vec4 texColor = texture2D(currentTexture, uv);

    float finalAlpha = texColor.r * texColor.a;

    gl_FragColor = vec4(0.85, 0.04, 0.04, finalAlpha);
}