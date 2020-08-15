#pragma once

class CTexture
{
public:
    CTexture();
    ~CTexture();
    static std::shared_ptr<CTexture> Create(ID3D12Device* device, int w, int h, int d, DXGI_FORMAT format);
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

struct ImageData
{
    int width;
    int height;
    int depth;
    std::vector<uint8_t> data;
};

std::shared_ptr<CTexture> LoadTextureFromFile(ID3D12Device* device, std::string path);
