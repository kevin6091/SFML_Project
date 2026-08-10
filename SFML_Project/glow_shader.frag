uniform sampler2D textureGlow;      // 빛나는 오브젝트들만 모인 텍스처
uniform sampler2D textureComposite; // 배경+캐릭터가 이미 합성된 원본 텍스처
uniform vec2 resolution;            // 렌더 타겟의 해상도
uniform float radius;               // 빛 번짐 강도 (예: 4.0)

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
    
    // [주의 및 선택 사항]
    // 현재 C++ 코드에서 'glowStates.blendMode = BlendAdd;' 를 사용하셨습니다.
    // 
    // 상황 A: 만약 renderTarget_Final이 까맣게 비워져 있는 상태라면
    // 아래처럼 baseColor와 glowColor를 셰이더 안에서 더해서 출력하는 것이 맞습니다.
    vec3 finalColor = baseColor.rgb + glowColor;
    gl_FragColor = vec4(finalColor, baseColor.a);
    
    /*
    상황 B: 만약 앞선 코드에서 renderTarget_Final에 이미 Composite 화면을 그려두셨다면,
    여기서 baseColor를 또 출력하면 C++의 BlendAdd 때문에 화면이 2배로 밝아져(하얗게 타버림) 버립니다.
    그럴 때는 위 두 줄을 지우고, 순수하게 글로우만 반환하도록 아래처럼 사용하셔야 합니다.
    
    gl_FragColor = vec4(glowColor, sum.a);
    */
}