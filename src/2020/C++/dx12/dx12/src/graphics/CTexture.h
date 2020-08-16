#pragma once

class CTexture
{
public:
    CTexture();
    ~CTexture();
    static std::shared_ptr<CTexture> Create(ID3D12Device* device, int w, int h, DXGI_FORMAT format);
    void Write(ID3D12GraphicsCommandList* commandList, std::vector<uint8_t> data);
    void Write(ID3D12GraphicsCommandList* commandList, const DirectX::Image* img);

    ID3D12Resource* Get() const
    {
        return m_Texture.Get();
    }

private:
    Microsoft::WRL::ComPtr<ID3D12Device> m_Device;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_Texture;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_UploadTexture;
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

std::shared_ptr<CTexture> LoadTextureFromFile(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, std::string path);
std::shared_ptr<CTexture> LoadTextureFromFile2(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, std::wstring path);
