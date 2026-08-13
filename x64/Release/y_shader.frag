uniform sampler2D currentTexture;
uniform float alpha;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec4 texColor = texture2D(currentTexture, uv);

    texColor.a = texColor.a * alpha;
    gl_FragColor = texColor;
}