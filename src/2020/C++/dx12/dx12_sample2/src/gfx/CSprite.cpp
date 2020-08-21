#include "CSprite.h"

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

CSprite::CSprite(ID3D12Device* device)
    : m_Device(device)
{
}

CSprite::~CSprite()
{
}

void CSprite::Create()
{
    assert(m_Device);

    // vertex buffer
    {
        vector<Vertex> vertices;
        // clang-format off
        {
            vertices.emplace_back(Vertex{ { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } });
            vertices.emplace_back(Vertex{ { -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } });
            vertices.emplace_back(Vertex{ {  1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } });
            vertices.emplace_back(Vertex{ {  1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } });
        }

        auto bufferSize = sizeof(Vertex) * vertices.size();
        auto props = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        auto desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

        m_Device->CreateCommittedResource(
            &props,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&m_VertexBuffer));

        void* dataBegin = nullptr;
        m_VertexBuffer->Map(0, nullptr, &dataBegin);
        memcpy(dataBegin, vertices.data(), bufferSize);
        m_VertexBuffer->Unmap(0, nullptr);

        m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
        m_VertexBufferView.StrideInBytes = sizeof(Vertex);
        m_VertexBufferView.SizeInBytes = bufferSize;
    }

    // index buffer
    {
        vector<uint16_t> indices = { 0, 1, 2, 2, 1, 3 };

        const size_t bufferSize = sizeof(uint16_t) * indices.size();
        auto props = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        auto desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

        m_Device->CreateCommittedResource(
            &props,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&m_IndexBuffer));

        void* dataBegin = nullptr;
        m_IndexBuffer->Map(0, nullptr, &dataBegin);
        memcpy(dataBegin, indices.data(), bufferSize);
        m_IndexBuffer->Unmap(0, nullptr);

        m_IndexBufferView.BufferLocation = m_IndexBuffer->GetGPUVirtualAddress();
        m_IndexBufferView.Format = DXGI_FORMAT_R16_UINT;
        m_IndexBufferView.SizeInBytes = sizeof(uint16_t) * indices.size();
    }
}

void CSprite::Draw(ID3D12GraphicsCommandList* commandList)
{
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    {
        D3D12_VERTEX_BUFFER_VIEW views[] = { m_VertexBufferView };
        commandList->IASetVertexBuffers(0, _countof(views), views);
    }

    commandList->IASetIndexBuffer(&m_IndexBufferView);
    commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}
