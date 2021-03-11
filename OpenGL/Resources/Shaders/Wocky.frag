#version 460

out vec4 out_Color;

in vec2 s_TexCoord;
in vec3 s_Normal;
in vec3 s_FragPos;

uniform vec3 u_CamPos;

const float c_PI = 3.14159265359;

vec4 hash4( vec2 p ) { return fract(sin(vec4( 1.0+dot(p,vec2(37.0,17.0)), 2.0+dot(p,vec2(11.0,47.0)), 3.0+dot(p,vec2(41.0,29.0)), 4.0+dot(p,vec2(23.0,31.0))))*103.0); }

vec3 error(int code) {
    vec3 color = vec3(0.0, 0.0, 0.0);
    switch(code) {
    case 0:
        //black
        color = vec3(0.1, 0.1, 0.1);
        break;
    case 1:
        //gray
        color = vec3(0.5, 0.5, 0.5);
        break;
    case 2:
        //red
        color = vec3(1.0, 0.0, 0.0);
        break;
    case 3:
        //orange
        color = vec3(1.0, 0.5, 0.0);
        break;
    case 4:
        //yellow
        color = vec3(1.0, 1.0, 0.0);
        break;
    case 5:
        //green
        color = vec3(0.0, 1.0, 0.0);
        break;
    case 6:
        //cyan
        color = vec3(0.0, 1.0, 1.0);
        break;
    case 7:
        //blue
        color = vec3(0.0, 0.0, 1.0);
        break;
    case 8:
        //purple
        color = vec3(0.5, 0.0, 1.0);
        break;
    case 9:
        //pink
        color = vec3(1.0, 0.0, 1.0);
        break;
    case 10:
        //white
        color = vec3(1.0, 1.0, 1.0);
        break;
    default:
        break;
    }
    return color;
}

vec4 textureNoTile(sampler2D samp, in vec2 uv)
{
    return texture(samp, uv);
    //ivec2 iuv = ivec2( floor( uv ) );
    // vec2 fuv = fract( uv );
    //
    //// generate per-tile transform
    //vec4 ofa = hash4( iuv + ivec2(0,0) );
    //vec4 ofb = hash4( iuv + ivec2(1,0) );
    //vec4 ofc = hash4( iuv + ivec2(0,1) );
    //vec4 ofd = hash4( iuv + ivec2(1,1) );
    //
    //vec2 ddx = dFdx( uv );
    //vec2 ddy = dFdy( uv );
    //
    //// transform per-tile uvs
    //ofa.zw = sign( ofa.zw-0.5 );
    //ofb.zw = sign( ofb.zw-0.5 );
    //ofc.zw = sign( ofc.zw-0.5 );
    //ofd.zw = sign( ofd.zw-0.5 );
    //
    //// uv's, and derivatives (for correct mipmapping)
    //vec2 uva = uv*ofa.zw + ofa.xy, ddxa = ddx*ofa.zw, ddya = ddy*ofa.zw;
    //vec2 uvb = uv*ofb.zw + ofb.xy, ddxb = ddx*ofb.zw, ddyb = ddy*ofb.zw;
    //vec2 uvc = uv*ofc.zw + ofc.xy, ddxc = ddx*ofc.zw, ddyc = ddy*ofc.zw;
    //vec2 uvd = uv*ofd.zw + ofd.xy, ddxd = ddx*ofd.zw, ddyd = ddy*ofd.zw;
    //    
    //// fetch and blend
    //vec2 b = smoothstep( 0.25,0.75, fuv );
    //
    //return mix( mix( textureGrad( samp, uva, ddxa, ddya ), 
    //                 textureGrad( samp, uvb, ddxb, ddyb ), b.x ), 
    //            mix( textureGrad( samp, uvc, ddxc, ddyc ),
    //                 textureGrad( samp, uvd, ddxd, ddyd ), b.x), b.y );
}

void main()
{
    vec3 concentricColor = error(int(round(distance(u_CamPos, s_FragPos))) % 11);
    
    vec3 concentricColorWorld = error(int(round(distance(vec3(0.0, 0.0, 0.0), s_FragPos))) % 11);

    vec3 normalizedColor = normalize(abs(s_FragPos - u_CamPos));

    vec2 moddedColorTemp = mod((s_FragPos - u_CamPos), 1.0).xz;
    //vec3 moddedColor = vec3(moddedColorTemp.x, 0.0, moddedColorTemp.y);
    vec3 moddedColor = vec3(moddedColorTemp.x, mix(moddedColorTemp.x, moddedColorTemp.y, 0.5), moddedColorTemp.y);

    vec2 moddedWorldColorTemp = mod((s_FragPos), 1.0).xz;
    //vec3 moddedWorldColor = vec3(moddedWorldColorTemp.x, 0.0, moddedWorldColorTemp.y);
    vec3 moddedWorldColor = vec3(moddedWorldColorTemp.x, mix(moddedWorldColorTemp.x, moddedWorldColorTemp.y, 0.5), moddedWorldColorTemp.y);
    //out_Color = vec4(concentricColor, 1.0);
    //out_Color = vec4(normalizedColor, 1.0);
    //out_Color = vec4(moddedColor, 1.0);
    //out_Color = vec4(moddedWorldColor, 1.0);
    //out_Color = vec4(concentricColor * 0.33333 + normalizedColor * 0.33333 + moddedWorldColor * 0.33333, 1.0);
    //out_Color = vec4(concentricColor * 0.05 + normalizedColor * 0.50 + moddedColor * 0.225 + moddedWorldColor * 0.225, 1.0);
    out_Color = vec4(concentricColor * 0.05 + concentricColorWorld * 0.05 + normalizedColor * 0.50 + moddedColor * 0.2 + moddedWorldColor * 0.2, 1.0);
    //out_Color = round(sin(s_FragPos, ));
}