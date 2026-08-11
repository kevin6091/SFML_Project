uniform sampler2D currentTexture;
uniform bool isRed;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    
    vec4 texColor = texture2D(currentTexture, uv);

    if(isRed)
        texColor = texColor * vec4(0.85, 0.02, 0.02, 1);

    gl_FragColor = texColor;
}