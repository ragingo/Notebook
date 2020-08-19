#include "CTexture.h"

using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;

namespace
{
    ComPtr<ID3D12Resource> CreateUploadHeap(ID3D12Device* device, UINT64 bufferSize)
    {
        auto props = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        auto desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

        ComPtr<ID3D12Resource> textureUploadHeap;
        device->CreateCommittedResource(
            &props,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&textureUploadHeap));

        textureUploadHeap->SetName(L"TextureUploadHeap");

        return textureUploadHeap;
    }
}

CTexture::CTexture()
{
}

CTexture::~CTexture()
{
    if (m_Texture) {
        m_Texture.Reset();
    }
    m_ResourceDesc = {};
}

shared_ptr<CTexture> CTexture::Create(ID3D12Device* device, int w, int h, DXGI_FORMAT format)
{
    assert(device);

    auto props = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    auto desc = CD3DX12_RESOURCE_DESC::Tex2D(format, w, h);

    ComPtr<ID3D12Resource> tex;
    device->CreateCommittedResource(
        &props,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&tex));

    auto instance = make_shared<CTexture>();
    instance->m_Device = device;
    instance->m_Texture.Swap(tex);
    swap(instance->m_ResourceDesc, desc);

    return instance;
}

void CTexture::Write(ID3D12GraphicsCommandList* commandList, vector<uint8_t> data)
{
    assert(m_Texture);

    D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint = {};
    UINT64 totalSize = 0;
    m_Device->GetCopyableFootprints(&m_ResourceDesc, 0, 1, 0, &footprint, nullptr, nullptr, &totalSize);

    uint32_t size = footprint.Footprint.RowPitch * footprint.Footprint.Height;

    uint8_t* ptr = nullptr;
    m_UploadTexture = CreateUploadHeap(m_Device.Get(), size);
    m_UploadTexture->Map(0, nullptr, reinterpret_cast<void**>(&ptr));
    memcpy(ptr, data.data(), size);
    m_UploadTexture->Unmap(0, nullptr);

    D3D12_TEXTURE_COPY_LOCATION dst = {};
    dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    dst.pResource = m_Texture.Get();
    dst.SubresourceIndex = 0;

    D3D12_TEXTURE_COPY_LOCATION src = {};
    src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
    src.pResource = m_UploadTexture.Get();
    src.PlacedFootprint = footprint;

    commandList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
}

void CTexture::Write(ID3D12GraphicsCommandList* commandList, const Image* img)
{
    vector<uint8_t> data;
    data.resize(img->rowPitch * img->height);
    memcpy(data.data(), img->pixels, data.size());
    Write(commandList, data);
}

shared_ptr<CTexture> LoadTextureFromFile(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, wstring path)
{
    TexMetadata meta = {};
    ScratchImage scratchImg = {};
    LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &meta, scratchImg);

    auto tex = CTexture::Create(device, meta.width, meta.height, meta.format);
    tex->Write(commandList, scratchImg.GetImage(0, 0, 0));

    return tex;
}
