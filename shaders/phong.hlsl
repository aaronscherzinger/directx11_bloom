struct VertexPosNormalIn
{
    float4 position : POSITION;
    float4 normal : NORMAL;
};

struct VertexOut
{
    float4 position : SV_POSITION;
    // view space position and normal
    float4 viewSpacePosition : VIEWPOS;
    float3 viewSpaceNormal : VIEWNORMAL;
};

cbuffer Transformations : register(b0)
{
    float4x4 modelTransform;
    float4x4 viewTransform;
    float4x4 projTransform;
};

// vertex shader
VertexOut VSMain(VertexPosNormalIn v)
{
    VertexOut output;

    output.viewSpacePosition = mul(mul(v.position, modelTransform), viewTransform);
    output.position = mul(output.viewSpacePosition, projTransform);

    float3x3 normalMatrix = (float3x3) mul(modelTransform, viewTransform);
    output.viewSpaceNormal = mul(v.normal.xyz, normalMatrix);

    return output;
}

cbuffer LightSource : register(b0)
{
    float4 lightPositionViewSpace;
    // RGB color and the light power in the w-coordinate
    float4 lightColorAndPower;
};

cbuffer Material : register(b1)
{
    // rgb components contain ambient and diffuse colors
    float4 materialAmbientColor;
    float4 materialDiffuseColor;
    // rgb contains color, w-coordinate contains specular exponent
    float4 materialSpecularColorAndShininess;
};

// pixel shader
float4 PSMain(VertexOut p) : SV_TARGET
{
    // blinn-phong model
    float3 l = lightPositionViewSpace.xyz - p.viewSpacePosition.xyz;
    float d_sq = dot(l, l);
    l = normalize(l);

    float3 n = normalize(p.viewSpaceNormal);
    float3 v = normalize(-p.viewSpacePosition.xyz);
    float nDotL = dot(n, l);

    float lightPower = lightColorAndPower.w / d_sq;
    float lambertian = max(nDotL, 0.0);
    float3 diffuseColor = materialDiffuseColor.rgb * lightColorAndPower.rgb;

    float3 h = normalize(l + v);
    float specular = nDotL > 0.0 ? dot(h, n) : 0.0;
    // we need this check since pow(...) with specular == 0.0 will give us -INF :(
    if (specular > 0.0)
    {
        specular = pow(specular, materialSpecularColorAndShininess.w);
    }
    float3 specularColor = materialSpecularColorAndShininess.rgb * lightColorAndPower.rgb;

    float3 color = materialAmbientColor.rgb
                 + diffuseColor * lambertian * lightPower
                 + specularColor * specular * lightPower;

    return float4(color, 1.0);
}