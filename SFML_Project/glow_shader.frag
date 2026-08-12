// glow_shader.frag
uniform sampler2D textureGlow;      // 빛나는 오브젝트들만 모인 텍스처
uniform sampler2D textureComposite; // 배경+캐릭터가 이미 합성된 원본 텍스처
uniform vec2 resolution;            // 렌더 타겟의 해상도
uniform float radius;               // 빛 번짐 강도 (기본 4.0 ~ 8.0 추천)
uniform bool isReverse;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec2 uv2 = uv;
    uv2.y = 1.0 - uv2.y;   

    vec4 sum = vec4(0.0);
    
    // 1픽셀의 정확한 크기
    vec2 pixelSize = 1.0 / resolution; 

    float weights[5];
    weights[0] = 0.06136;
    weights[1] = 0.24477;
    weights[2] = 0.38774;
    weights[3] = 0.24477;
    weights[4] = 0.06136;

    for (int x = -2; x <= 2; ++x)
    {
        for (int y = -2; y <= 2; ++y)
        {
            float weight = weights[x + 2] * weights[y + 2];
            
            // ★ [수정 2] 픽셀 사이즈에 radius를 곱해서 샘플링 범위를 넓게 찢어줍니다.
            vec2 sampleUV = uv2 + vec2(float(x), float(y)) * pixelSize * radius;
            
            sum += texture2D(textureGlow, sampleUV) * weight;
        }
    }

    // ★ [수정 3] 빛 증폭량 상향! 
    // 빛이 너무 약하면 블러가 아니라 그냥 칙칙하게 보입니다. 2.5 ~ 3.0배로 뻥튀기해 줍니다.
    vec3 glowColor = sum.rgb * 2.5;

    // ----------------------------------------------------
    // 2. textureComposite(원본 화면)과 합성하기
    // ----------------------------------------------------
    vec4 baseColor = texture2D(textureComposite, uv);

    // ----------------------------------------------------
    // 3. 최종 출력 (가산 혼합 - Additive)
    // ----------------------------------------------------
    vec3 finalColor = baseColor.rgb + glowColor;
    
    if(isReverse)
    {
        gl_FragColor = vec4(0.8 - finalColor.r, 0.8 - finalColor.g, 0.8 - finalColor.b, baseColor.a);
    }
    else
    {
        gl_FragColor = vec4(finalColor, baseColor.a);
    }
}