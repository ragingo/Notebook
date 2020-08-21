#pragma once

class CSprite
{
public:
    CSprite(ID3D12Device* device);
    ~CSprite();

    void Create();

    void Draw(ID3D12GraphicsCommandList* commandList);

private:
    struct Vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT4 color;
        DirectX::XMFLOAT2 texcoord;
    };

    ID3D12Device* m_Device = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_VertexBuffer;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_IndexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView = {};
    D3D12_INDEX_BUFFER_VIEW m_IndexBufferView = {};
};

