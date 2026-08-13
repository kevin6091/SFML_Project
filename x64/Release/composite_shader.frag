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

    if(isSlow)
    {
        vec4 black = vec4(0.0, 0.0, 0.0, 1.0);
        bg = mix(bg, black, accSlow);
        actor = mix(actor, black, accSlow);
    }

    vec4 finalColor = bg;

    // 1. Actor 합성 (배경을 Actor의 불투명도만큼 가리고, 그 위에 Actor 색상을 얹음)
    finalColor.rgb = finalColor.rgb * (1.0 - actor.a) + actor.rgb;
    
    // 2. Player 합성
    finalColor.rgb = finalColor.rgb * (1.0 - player.a) + player.rgb;
    
    // 3. Effect 합성 (이펙트가 알파 블렌딩일 경우 완벽하게 조화됩니다)
    finalColor.rgb = finalColor.rgb * (1.0 - effect.a) + effect.rgb;

    // 최종 출력 (화면에 그릴 때는 알파값이 1.0(불투명)이어야 합니다)
    gl_FragColor = vec4(finalColor.rgb, 1.0);
}