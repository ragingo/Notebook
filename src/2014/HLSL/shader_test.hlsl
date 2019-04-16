// memo: http://www.cc.kyoto-su.ac.jp/~kano/pdf/course/chapter10.pdf

sampler2D img : register(S0);

const int TEXTURE_WIDTH  = 1024;
const int TEXTURE_HEIGHT = 768;
static float TexWidthInv  = (1.0 / float(TEXTURE_WIDTH));
static float TexHeightInv = (1.0 / float(TEXTURE_HEIGHT));
static float2 CoodOffsets[9] = {
    float2(-1.0 * TexWidthInv, -1.0 * TexHeightInv),
    float2( 0.0 * TexWidthInv, -1.0 * TexHeightInv),
    float2( 1.0 * TexWidthInv, -1.0 * TexHeightInv),
    float2(-1.0 * TexWidthInv,  0.0 * TexHeightInv),
    float2( 0.0 * TexWidthInv,  0.0 * TexHeightInv),
    float2( 1.0 * TexWidthInv,  0.0 * TexHeightInv),
    float2(-1.0 * TexWidthInv,  1.0 * TexHeightInv),
    float2( 0.0 * TexWidthInv,  1.0 * TexHeightInv),
    float2( 1.0 * TexWidthInv,  1.0 * TexHeightInv)
};
// static float2 CoodOffsets[9] = {
//     float2(-1.0 * TexWidthInv, -1.0 * TexHeightInv),
//     float2(-1.0 * TexWidthInv,  0.0 * TexHeightInv),
//     float2(-1.0 * TexWidthInv,  1.0 * TexHeightInv),
//     float2( 0.0 * TexWidthInv, -1.0 * TexHeightInv),
//     float2( 0.0 * TexWidthInv,  0.0 * TexHeightInv),
//     float2( 0.0 * TexWidthInv,  1.0 * TexHeightInv),
//     float2( 1.0 * TexWidthInv, -1.0 * TexHeightInv),
//     float2( 1.0 * TexWidthInv,  0.0 * TexHeightInv),
//     float2( 1.0 * TexWidthInv,  1.0 * TexHeightInv)
// };

struct PS_INPUT
{
    float2 uv : TEXCOORD0;
};

struct EffectInfo
{
    float4 pixels[9] : COLOR;
};

float4 get_texture(float2 uv, int offset)
{
    return tex2D(img, uv + CoodOffsets[offset]);
}

// グレースケール
float4 apply_effect_grayscale(float4 color)
{
    return dot(color.rgb, float3(0.298912, 0.586611, 0.114478));
}

// 二値化
float4 apply_effect_binarization(float4 color, float threshold)
{
    if (color.r > threshold ||
        color.g > threshold ||
        color.b > threshold) {
        color = float4(0.0, 0.0, 0.0, 1.0);
    }
    else {
        color = float4(1.0, 1.0, 1.0, 1.0);
    }
    return color;
}

// 二次微分 laplacian 4方向
float apply_effect_laplacian4(EffectInfo info)
{
    float4 pix[9] = info.pixels;
    float d = 
        0.0 * pix[0] +  1.0 * pix[1] +  0.0 * pix[2] + 
        1.0 * pix[3] + -4.0 * pix[4] +  1.0 * pix[5] + 
        0.0 * pix[6] +  1.0 * pix[7] +  0.0 * pix[8];

    float x = abs(d);
    return x;
}

// 二次微分 laplacian 8方向
float apply_effect_laplacian8(EffectInfo info)
{
    float4 pix[9] = info.pixels;
    float d = 
         1.0 * pix[0].rgb +  1.0 * pix[1].rgb +  1.0 * pix[2].rgb + 
         1.0 * pix[3].rgb + -8.0 * pix[4].rgb +  1.0 * pix[5].rgb + 
         1.0 * pix[6].rgb +  1.0 * pix[7].rgb +  1.0 * pix[8].rgb;

    float x = abs(d);
    // float x = max(d, 0.0);

    if (x < 0.4) {
        x = 0.0;
    }
    else {
        x = 1.0;
    }
    return x;
}

// nicovita sobel
float apply_effect_nicovita_sobel(EffectInfo info)
{
    float4 pix[9] = info.pixels;
    float dx =
        -1.5 * pix[0] + 1.5 * pix[2] -
         2.0 * pix[3] + 2.0 * pix[5] -
         1.5 * pix[6] + 1.5 * pix[8];

    float dy =
        -1.5 * pix[0] + 1.5 * pix[6] -
         2.0 * pix[1] + 2.0 * pix[7] -
         1.5 * pix[2] + 1.5 * pix[8];

    float x = (abs(dx) + abs(dy)) * 0.5;
    if (x > 0.9) {
        x = 1.0;
    }
    return x;
}

// 一次微分 Roberts
float apply_effect_roberts(EffectInfo info)
{
    float4 pix[9] = info.pixels;
    float dx =
         0.0 * pix[0] +  0.0 * pix[1] +  0.0 * pix[2] +
         0.0 * pix[3] +  1.0 * pix[4] +  0.0 * pix[5] +
         0.0 * pix[6] +  0.0 * pix[7] + -1.0 * pix[8];

    float dy =
         0.0 * pix[0] +  0.0 * pix[1] +  0.0 * pix[2] +
         0.0 * pix[3] +  0.0 * pix[4] +  1.0 * pix[5] +
         0.0 * pix[6] + -1.0 * pix[7] +  0.0 * pix[8];

    float x = sqrt(pow(dx,2.0) + pow(dy,2.0)) * 0.5;
    return x;
}

// Prewitt
float apply_effect_prewitt(EffectInfo info)
{
    float4 pix[9] = info.pixels;
    float4 m[8];
    float n = 0.0;
    m[0] =  pix[0] + pix[1] + pix[2] + pix[3] - 2 * pix[4] + pix[5] - pix[6] - pix[7] - pix[8];
    m[1] =  pix[0] + pix[1] + pix[2] + pix[3] - 2 * pix[4] - pix[5] + pix[6] - pix[7] - pix[8];
    m[2] =  pix[0] + pix[1] - pix[2] + pix[3] - 2 * pix[4] - pix[5] + pix[6] + pix[7] - pix[8];
    m[3] =  pix[0] - pix[1] - pix[2] + pix[3] - 2 * pix[4] - pix[5] + pix[6] + pix[7] + pix[8];
    m[4] = -pix[0] - pix[1] - pix[2] + pix[3] - 2 * pix[4] + pix[5] + pix[6] + pix[7] + pix[8];
    m[5] = -pix[0] - pix[1] + pix[2] - pix[3] - 2 * pix[4] + pix[5] + pix[6] + pix[7] + pix[8];
    m[6] = -pix[0] + pix[1] + pix[2] - pix[3] - 2 * pix[4] + pix[5] - pix[6] + pix[7] + pix[8];
    m[7] =  pix[0] + pix[1] + pix[2] - pix[3] - 2 * pix[4] + pix[5] - pix[6] - pix[7] + pix[8];
    for(int i=0;i<8; i++) {
        n = max(n, m[i]);
    }
    float x = n * 0.8;
    return x;
}

float4 main(PS_INPUT input) : SV_Target
{
    float4 pix[9];

    for (int i=0; i<9; i++) {
        pix[i] = get_texture(input.uv, i);
        pix[i] = apply_effect_grayscale(pix[i]);
        // pix[i] = apply_effect_binarization(pix[i], 0.1);
    }

    float x = 0.0;
    EffectInfo effectInfo = (EffectInfo)0;
    effectInfo.pixels = pix;

    // pattern 1
    // x = apply_effect_laplacian4(effectInfo);

    // pattern 2
    x = apply_effect_laplacian8(effectInfo);

    // pattern 3
    // x = apply_effect_nicovita_sobel(effectInfo); // 全然うまく行かない

    // pattern 4
    // x = apply_effect_roberts(effectInfo);

    // pattern 5
    // x = apply_effect_prewitt(effectInfo);

    // グレースケール実験用
    // x = apply_effect_grayscale(tex2D(img, input.uv));
    // 二値化実験用
    // x = apply_effect_binarization(tex2D(img, input.uv), 0.5);

    float4 color = float4(x, x, x, 1.0);

    return color;
}


// technique main
// {
//     pass p0
//     {
//         PixelShader = compile ps_3_0 main();
//     }
// }
