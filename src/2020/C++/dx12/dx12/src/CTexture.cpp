#include "pch.h"
#include "CTexture.h"

using namespace std;

CTexture::CTexture()
{
}

CTexture::~CTexture()
{
    if (m_Texture) {
        m_Texture.Reset();
    }
    m_HeapProps = {};
    m_ResourceDesc = {};
}

shared_ptr<CTexture> CTexture::Create(ID3D12Device* device, int w, int h, int d, DXGI_FORMAT format)
{
    assert(device);

    D3D12_HEAP_PROPERTIES heapProps = {};
    heapProps.Type = D3D12_HEAP_TYPE_CUSTOM;
    heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
    heapProps.CreationNodeMask = 1;
    heapProps.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC resDesc = {};
    resDesc.MipLevels = 1;
    resDesc.Format = format;
    resDesc.Width = w;
    resDesc.Height = h;
    resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    resDesc.DepthOrArraySize = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.SampleDesc.Quality = 0;
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

    auto instance = make_shared<CTexture>();

    instance->m_Depth = d / 8;
    swap(instance->m_HeapProps, heapProps);
    swap(instance->m_ResourceDesc, resDesc);

    HRESULT hr = device->CreateCommittedResource(
        &instance->m_HeapProps,
        D3D12_HEAP_FLAG_NONE,
        &instance->m_ResourceDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&instance->m_Texture));

    if (FAILED(hr)) {
        if (instance->m_Texture) {
            instance->m_Texture->Release();
        }
    }

    return instance;
}

void CTexture::Write(vector<uint8_t> data)
{
    assert(m_Texture);

    uint32_t w = static_cast<uint32_t>(m_ResourceDesc.Width);
    uint32_t h = m_ResourceDesc.Height;
    uint32_t d = m_Depth;

    D3D12_BOX box = { 0, 0, 0, w, h, 1 };
    m_Texture->WriteToSubresource(0, &box, data.data(), w * d, w * h * d);
}

namespace
{
    ImageData LoadBitmapFromFile(string path)
    {
        ImageData img = {};
        fstream fs(path, ios_base::in | ios_base::binary);
        if (!fs) {
            return {};
        }

        vector<uint8_t> x;
        x.resize(4);
        fs.read(reinterpret_cast<char*>(x.data()), x.size());
        if (!(x[0] == (int)'B' && x[1] == (int)'M')) {
            return {};
        }

        fs.seekg(0, ios_base::beg);

        BITMAPFILEHEADER fileHeader = {};
        fs.read(reinterpret_cast<char*>(&fileHeader), sizeof(BITMAPFILEHEADER));

        BITMAPINFOHEADER infoHeader = {};
        fs.read(reinterpret_cast<char*>(&infoHeader), sizeof(BITMAPINFOHEADER));

        img.width = infoHeader.biWidth;
        img.height = infoHeader.biHeight;
        img.depth = infoHeader.biBitCount;
        int size = img.width * img.height * (img.depth / 8);

        img.data.resize(size);
        fs.seekg(fileHeader.bfOffBits, ios_base::beg);
        fs.read(reinterpret_cast<char*>(img.data.data()), img.data.size());
        fs.close();

        return img;
    }
}

shared_ptr<CTexture> LoadTextureFromFile(ID3D12Device* device, string path)
{
    auto img = LoadBitmapFromFile(path);
    if (img.data.empty()) {
        return nullptr;
    }

    auto tex = CTexture::Create(device, img.width, img.height, img.depth, DXGI_FORMAT_B8G8R8A8_UNORM);
    tex->Write(img.data);

    return tex;
}
