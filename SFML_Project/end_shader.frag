uniform sampler2D textureFinal;
uniform float time;         
uniform float shatterStart; 

float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec4 finalColor = vec4(0.0, 0.0, 0.0, 1.0); 
    bool found = false;

    if (shatterStart <= 0.0)
    {
        finalColor = texture2D(textureFinal, uv);
        found = true;
    }
    else
    {
        float cols = 10.0;
        float rows = 10.0;

        for (int x = 0; x < 10; x++)
        {
            for (int y = 0; y < 10; y++)
            {
                vec2 cellId = vec2(float(x), float(y));
                
                float r1 = random(cellId);
                float r2 = random(cellId + vec2(11.1, 22.2));
                float r3 = random(cellId + vec2(33.3, 44.4));

                vec2 cellCenter = (cellId + 0.5) / vec2(cols, rows);

                vec2 vel = vec2((r1 - 0.5) * 2.0, (r2 - 0.5) * 1.5 - 0.5);

                vec2 shift = vel * shatterStart + vec2(0.0, 4.0 * shatterStart * shatterStart);

                float angle = (r3 - 0.5) * 20.0 * shatterStart;
                float c = cos(angle);
                float s = sin(angle);
                mat2 rot = mat2(c, -s, s, c);

                vec2 orig_uv = uv - shift;
                orig_uv = rot * (orig_uv - cellCenter) + cellCenter; // 중심축 기준 회전 복구

                vec2 distortion = vec2(sin(orig_uv.y * 40.0), cos(orig_uv.x * 40.0)) * 0.015;
                vec2 origCellId = floor((orig_uv + distortion) * vec2(cols, rows));

                if (origCellId == cellId && 
                    orig_uv.x >= 0.0 && orig_uv.x <= 1.0 && 
                    orig_uv.y >= 0.0 && orig_uv.y <= 1.0)
                {
                    finalColor = texture2D(textureFinal, orig_uv);
                    found = true;
                    break;
                }
            }
            if (found) break; // 이미 그릴 픽셀을 찾았다면 외부 루프도 탈출
        }
    }

    float flickerSpeed = 15.0;
    float randomVal = random(vec2(floor(time * flickerSpeed), 0.0));
    
    bool doInvert = (randomVal > 0.7) && (shatterStart <= 0.0);

    if (found && doInvert)
    {
        finalColor.rgb = vec3(0.8) - finalColor.rgb;
    }

    gl_FragColor = finalColor;
}