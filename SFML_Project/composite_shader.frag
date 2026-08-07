// composite.frag
uniform sampler2D textureBG;
uniform sampler2D textureActor;
uniform sampler2D texturePlayer;
uniform sampler2D textureEffect;
uniform float time;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    uv.y = 1.0 - uv.y;

    // 1. 각 도화지(타겟)에서 현재 픽셀의 색상을 뽑아옵니다.
    vec4 bg = texture2D(textureBG, uv);
    vec4 actor = texture2D(textureActor, uv);
    vec4 player = texture2D(texturePlayer, uv);
    vec4 effect = texture2D(textureEffect, uv);

    // ----------------------------------------------------
    // [여기에 원하는 특수 효과 수학 공식을 넣습니다!]
    // 예시: 이펙트 레이어의 색상만큼 배경을 꿀렁이게 만들기 (아지랑이)
    // vec2 distortedUV = uv + (effect.rg * 0.05 * sin(time * 10.0));
    // bg = texture2D(textureBG, distortedUV);
    // ----------------------------------------------------

    // 2. 기본 알파 블렌딩 (수동으로 덮어씌우기)
    // 배경 위에 액터를 얹음
    vec4 finalColor = mix(bg, actor, actor.a); 
    finalColor = mix(finalColor, player, player.a);
    
    // 그 위에 이펙트를 얹음 (보통 이펙트는 더 밝게 빛나는 Additive 블렌딩을 하기도 함)
    finalColor = mix(finalColor, effect, effect.a); 

    gl_FragColor = finalColor;
}