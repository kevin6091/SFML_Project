uniform sampler2D textureGlow;      // 빛나는 오브젝트들만 모인 텍스처
uniform sampler2D textureComposite; // 배경+캐릭터가 이미 합성된 원본 텍스처
uniform vec2 resolution;            // 렌더 타겟의 해상도
uniform float radius;               // 빛 번짐 강도 (예: 4.0)
uniform bool isReverse;

void main()
{
    // 현재 픽셀의 UV 좌표 획득
    vec2 uv = gl_TexCoord[0].xy;
    vec2 uv2 = uv;
    // ★ SFML RenderTexture 고질병인 Y축 뒤집힘 보정
    uv2.y = 1.0 - uv2.y;

    // ----------------------------------------------------
    // 1. textureGlow 가우시안 블러(빛 번짐) 처리
    // ----------------------------------------------------
    vec4 sum = vec4(0.0);
    vec2 offset = radius / resolution; // 픽셀당 이동할 UV 거리

    // 1차원 정규분포 가중치 배열 (5x5 블러용)
    float weights[5];
    weights[0] = 0.06136;
    weights[1] = 0.24477;
    weights[2] = 0.38774;
    weights[3] = 0.24477;
    weights[4] = 0.06136;

    // 2중 루프로 25개 주변 픽셀을 샘플링하여 부드럽게 섞기
    for (int x = -2; x <= 2; ++x)
    {
        for (int y = -2; y <= 2; ++y)
        {
            float weight = weights[x + 2] * weights[y + 2];
            vec2 sampleUV = uv2 + vec2(float(x), float(y)) * offset;
            
            sum += texture2D(textureGlow, sampleUV) * weight;
        }
    }

    // 블러 처리된 글로우 색상 (원한다면 1.5배 등 밝기를 증폭시켜도 됨)
    vec3 glowColor = sum.rgb * 1.5;


    // ----------------------------------------------------
    // 2. textureComposite(원본 화면)과 합성하기
    // ----------------------------------------------------
    vec4 baseColor = texture2D(textureComposite, uv);


    // ----------------------------------------------------
    // 3. 최종 출력
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