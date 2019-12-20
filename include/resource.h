#pragma once

#include <d3d11.h>

struct ShaderProgram
{
    // binary blobs for vertex and pixel shader
    ID3D10Blob* vsBlob;
    ID3D10Blob* psBlob;

    // vertex and pixel shader
    ID3D11VertexShader* vShader;
    ID3D11PixelShader* pShader;
};

struct ComputeShader
{
    // binary blob
    ID3D10Blob* csBlob;

    // compute shader
    ID3D11ComputeShader* cShader;
};

// render target consisting of texture, view for use as render target, as well as SRV and UAV
struct RenderTarget
{
    ID3D11Texture2D* renderTargetTexture;
    ID3D11RenderTargetView* renderTargetView;
    ID3D11ShaderResourceView* shaderResourceView;
    ID3D11UnorderedAccessView* unorderedAccessView;
};

struct DepthStencilTarget
{
    ID3D11Texture2D* dsTexture;
    ID3D11DepthStencilView* dsView;
};

struct Mesh
{
    ID3D11Buffer* vertexBuffer;
    ID3D11InputLayout* vertexLayout;

    UINT vertexCount;
    UINT stride;
    UINT offset;
    D3D_PRIMITIVE_TOPOLOGY topology;
};

struct Transformations
{
    DirectX::XMMATRIX model;
    DirectX::XMMATRIX view;
    DirectX::XMMATRIX proj;
};

struct LightSource
{
    // light position in view space
    DirectX::XMFLOAT4 lightPosition;
    // RGB color and the light power in the w-coordinate
    DirectX::XMFLOAT4 lightColorAndPower;
};

struct Material
{
    // rgb components contain ambient and diffuse colors
    DirectX::XMFLOAT4 ambient;
    DirectX::XMFLOAT4 diffuse;
    // rgb contains color, w-coordinate contains specular exponent
    DirectX::XMFLOAT4 specularAndShininess;
};

struct ThresholdParams
{
    alignas(16) float threshold;
};

struct CompositeParams
{
    alignas(16) float coefficient;
};

// (GAUSSIAN_RADIUS + 1) must be multiple of 4 because of the way we set up the shader
#define GAUSSIAN_RADIUS 7

struct BlurParams
{
    alignas(16) float coefficients[GAUSSIAN_RADIUS + 1];
    int radius;     // must be <= MAX_GAUSSIAN_RADIUS
    int direction;  // 0 = horizontal, 1 = vertical
};