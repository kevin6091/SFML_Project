// rewind.frag
uniform sampler2D currentTexture; // 현재 화면
uniform float time;               // 찌그러짐을 움직이게 할 시간 값
uniform float accTime1;
uniform float accTime2;

void main()
{
   // 현재 픽셀의 좌표 (0.0 ~ 1.0)
    vec2 uv = gl_TexCoord[0].xy;

    // 디스토션
    float wave = cos(uv.y * 50.0 + time * 15.0) * 0.01;
    uv.x += wave * accTime1;
    uv.y += wave * accTime2;

    // 색상 분리
    float shift = 0.008;
    float r = texture2D(currentTexture, uv + vec2(shift, 0.0)).r;
    float g = texture2D(currentTexture, uv).g;
    float b = texture2D(currentTexture, uv - vec2(shift, 0.0)).b;

    // 필름 그레인
    float noise = (fract(sin(dot(uv + vec2(time), vec2(12.9898, 78.233))) * 43758.5453) - 0.5) * 0.3;

    // 색상 합성 및 시안
    vec3 color = vec3(r, g, b) + noise;
    color *= vec3(0.6, 0.9, 1.0); 

    gl_FragColor = vec4(color, 1.0);
}