#pragma once

class Texture
{
public:
    Texture();
    ~Texture();
    static std::shared_ptr<Texture> Create(ID3D12Device* device, int w, int h, int d, DXGI_FORMAT format);
    void Write(std::vector<uint8_t> data);

    ID3D12Resource* Get() const
    {
        return m_Texture.Get();
    }

private:
    uint32_t m_Depth = 24;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_Texture;
    D3D12_RESOURCE_DESC m_ResourceDesc = {};
    D3D12_HEAP_PROPERTIES m_HeapProps = {};
};
