#version 120

uniform sampler2D texture;

// 불꽃 그라데이션 색상 정의 (0.0 ~ 1.0 RGB)
const vec3 COLOR_OUTER = vec3(0.85, 0.10, 0.00); // 1단계: 외곽 (진한 붉은주황)
const vec3 COLOR_MID   = vec3(1.00, 0.45, 0.00); // 2단계: 중간 (선명한 불꽃 주황)
const vec3 COLOR_CORE  = vec3(1.00, 0.95, 0.60); // 3단계: 중심 (빛나는 황백색)

void main()
{
    // 원본 스프라이트 픽셀 읽기
    vec4 texColor = texture2D(texture, gl_TexCoord[0].xy);

    // 흑백/밝기값 (R 채널 기준)
    float intensity = texColor.r;

    // 2단계 Lerp(mix)를 이용한 불꽃 그라데이션 계산
    vec3 finalRGB;
    if (intensity < 0.5)
    {
        // 밝기 0.0 ~ 0.5 구간: 진한 빨강 -> 주황 보간
        float t = intensity / 0.5;
        finalRGB = mix(COLOR_OUTER, COLOR_MID, t);
    }
    else
    {
        // 밝기 0.5 ~ 1.0 구간: 주황 -> 중심부 밝은 노랑 보간
        float t = (intensity - 0.5) / 0.5;
        finalRGB = mix(COLOR_MID, COLOR_CORE, t);
    }

    // 완전히 어두운 배경(검은색)은 투명 처리 / 원본 알파 유지
    float finalAlpha = texColor.a * step(0.05, intensity);

    gl_FragColor = vec4(finalRGB, finalAlpha) * gl_Color;
}