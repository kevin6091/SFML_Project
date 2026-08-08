// composite.frag
uniform sampler2D textureBG;
uniform sampler2D textureActor;
uniform sampler2D texturePlayer;
uniform sampler2D textureEffect;
uniform float accSlow;
uniform bool isSlow;


void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    uv.y = 1.0 - uv.y;

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

    if(isSlow)
    {
        vec4 black = vec4(0.0, 0.0, 0.0, 1.0);
        bg = mix(bg, black, accSlow);
        actor = mix(actor, black, accSlow);
    }

    vec4 finalColor = mix(bg, actor, actor.a); 
    finalColor = mix(finalColor, player, player.a);
    finalColor = mix(finalColor, effect, effect.a); 

    gl_FragColor = finalColor;
}