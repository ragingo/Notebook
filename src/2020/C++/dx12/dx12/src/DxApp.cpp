﻿#include "pch.h"
#include "DxApp.h"
#include "../shaders/gen/SampleVS.h"
#include "../shaders/gen/SamplePS.h"
#include "Texture.h"

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

namespace
{
    ComPtr<IDXGIFactory7> CreateFactory()
    {
        HRESULT hr;
        ComPtr<IDXGIFactory7> factory;

        UINT flags = 0;
#ifdef _DEBUG
        {
            ComPtr<ID3D12Debug> debug;
            hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
            if (SUCCEEDED(hr)) {
                debug->EnableDebugLayer();
                flags |= DXGI_CREATE_FACTORY_DEBUG;
                debug->Release();
            }
        }
#endif
        hr = CreateDXGIFactory2(flags, IID_PPV_ARGS(&factory));
        if (FAILED(hr)) {
            return nullptr;
        }

        return factory;
    }

    ComPtr<IDXGIAdapter1> GetHardwareAdapter(IDXGIFactory7* factory)
    {
        ComPtr<IDXGIAdapter1> adapter;

        for (UINT i = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(i, &adapter); i++) {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
                continue;
            }

            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))) {
                break;
            }
        }

        return adapter;
    }

    ComPtr<ID3D12Device> CreateDevice(IDXGIAdapter1* adapter)
    {
        ComPtr<ID3D12Device> device;
        // https://stackoverflow.com/questions/54515510/d3d12createdevice-throws-com-error
        // まさにこれが発生する。無視。
        D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));

        return device;
    }

    void SetResourceBarrier(ID3D12GraphicsCommandList* commandList, ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
    {
        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource = resource;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        barrier.Transition.StateBefore = before;
        barrier.Transition.StateAfter = after;
        commandList->ResourceBarrier(1, &barrier);
    }

    bool IsRootSignatureVersionAvaiable(ID3D12Device* device, D3D_ROOT_SIGNATURE_VERSION version)
    {
        D3D12_FEATURE_DATA_ROOT_SIGNATURE data = {};
        data.HighestVersion = version;

        if (SUCCEEDED(device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &data, sizeof(data)))) {
            return true;
        }

        return false;
    }

    ComPtr<ID3D12RootSignature> CreateRootSignature(ID3D12Device* device)
    {
        D3D12_VERSIONED_ROOT_SIGNATURE_DESC sigDesc = {};

        if (IsRootSignatureVersionAvaiable(device, D3D_ROOT_SIGNATURE_VERSION_1_1)) {
            D3D12_DESCRIPTOR_RANGE1 range = {};
            range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            range.NumDescriptors = 1;
            range.BaseShaderRegister = 0;
            range.RegisterSpace = 0;
            range.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC;
            range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

            D3D12_ROOT_PARAMETER1 rootParam = {};
            rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
            rootParam.DescriptorTable.NumDescriptorRanges = 1;
            rootParam.DescriptorTable.pDescriptorRanges = &range;

            D3D12_STATIC_SAMPLER_DESC sampler = {};
            sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
            sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
            sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
            sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
            sampler.MipLODBias = 0;
            sampler.MaxAnisotropy = 0;
            sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
            sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
            sampler.MinLOD = 0.0f;
            sampler.MaxLOD = D3D12_FLOAT32_MAX;
            sampler.ShaderRegister = 0;
            sampler.RegisterSpace = 0;
            sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

            sigDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
            sigDesc.Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
            sigDesc.Desc_1_1.NumStaticSamplers = 1;
            sigDesc.Desc_1_1.pStaticSamplers = &sampler;
            sigDesc.Desc_1_1.NumParameters = 1;
            sigDesc.Desc_1_1.pParameters = &rootParam;
        }
        else {
            sigDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_0;
            sigDesc.Desc_1_0.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        }

        ComPtr<ID3DBlob> sig;
        ComPtr<ID3DBlob> error;
        D3D12SerializeVersionedRootSignature(&sigDesc, &sig, &error);

        ComPtr<ID3D12RootSignature> rootSignature;
        device->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&rootSignature));

        return rootSignature;
    }

    ComPtr<ID3D12PipelineState> CreatePipelineState(ID3D12Device* device, ID3D12RootSignature* rootSignature)
    {
        D3D12_INPUT_ELEMENT_DESC pos = {};
        pos.SemanticName = "POSITION";
        pos.SemanticIndex = 0;
        pos.Format = DXGI_FORMAT_R32G32B32_FLOAT;
        pos.InputSlot = 0;
        pos.AlignedByteOffset = 0;
        pos.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        pos.InstanceDataStepRate = 0;

        D3D12_INPUT_ELEMENT_DESC color = {};
        color.SemanticName = "COLOR";
        color.SemanticIndex = 0;
        color.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        color.InputSlot = 0;
        color.AlignedByteOffset = 12;
        color.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        color.InstanceDataStepRate = 0;

        D3D12_INPUT_ELEMENT_DESC texcoord = {};
        texcoord.SemanticName = "TEXCOORD";
        texcoord.SemanticIndex = 0;
        texcoord.Format = DXGI_FORMAT_R32G32_FLOAT;
        texcoord.InputSlot = 0;
        texcoord.AlignedByteOffset = 28;
        texcoord.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        texcoord.InstanceDataStepRate = 0;

        D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = { pos, color, texcoord };

        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
        psoDesc.pRootSignature = rootSignature;
        psoDesc.VS = D3D12_SHADER_BYTECODE{ g_SampleVS, sizeof(g_SampleVS) };
        psoDesc.PS = D3D12_SHADER_BYTECODE{ g_SamplePS, sizeof(g_SamplePS) };
        psoDesc.RasterizerState = {};
        psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
        psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
        psoDesc.RasterizerState.FrontCounterClockwise = FALSE;
        psoDesc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
        psoDesc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
        psoDesc.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        psoDesc.RasterizerState.DepthClipEnable = TRUE;
        psoDesc.RasterizerState.MultisampleEnable = FALSE;
        psoDesc.RasterizerState.AntialiasedLineEnable = FALSE;
        psoDesc.RasterizerState.ForcedSampleCount = 0;
        psoDesc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
        psoDesc.BlendState = {};
        psoDesc.BlendState.AlphaToCoverageEnable = FALSE;
        psoDesc.BlendState.IndependentBlendEnable = FALSE;
        {
            // clang-format off
            const D3D12_RENDER_TARGET_BLEND_DESC rtBlendDesc = {
                FALSE, FALSE,
                D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
                D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
                D3D12_LOGIC_OP_NOOP,
                D3D12_COLOR_WRITE_ENABLE_ALL,
            };
            for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
                psoDesc.BlendState.RenderTarget[i] = rtBlendDesc;
            }
        }
        psoDesc.DepthStencilState.DepthEnable = FALSE;
        psoDesc.DepthStencilState.StencilEnable = FALSE;
        psoDesc.SampleMask = UINT_MAX;
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.SampleDesc.Count = 1;

        ComPtr<ID3D12PipelineState> pipelineState;
        device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState));

        return pipelineState;
    }

    [[maybe_unused]] void UpdateTexture(
        ID3D12GraphicsCommandList* commandList,
        ID3D12Resource* destination,
        ID3D12Resource* intermediate,
        const D3D12_PLACED_SUBRESOURCE_FOOTPRINT& footprint,
        const D3D12_SUBRESOURCE_DATA& srcData,
        size_t dataSize
    )
    {
        auto intermediateDesc = intermediate->GetDesc();
        if (intermediateDesc.Dimension != D3D12_RESOURCE_DIMENSION_BUFFER) {
            return;
        }

        auto destinationDesc = destination->GetDesc();
        if (destinationDesc.Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE2D) {
            return;
        }

        uint8_t* data = nullptr;
        intermediate->Map(0, nullptr, reinterpret_cast<void**>(&data));
        memcpy(data, reinterpret_cast<const uint8_t*>(srcData.pData), dataSize);
        intermediate->Unmap(0, nullptr);

        D3D12_TEXTURE_COPY_LOCATION dst = {};
        dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
        dst.pResource = destination;
        dst.SubresourceIndex = 0;

        D3D12_TEXTURE_COPY_LOCATION src;
        src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
        src.pResource = intermediate;
        src.PlacedFootprint = footprint;

        commandList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
    }

    ComPtr<ID3D12Resource> CreateVertexBuffer(ID3D12Device* device, vector<Vertex> vertices)
    {
        const UINT vertexBufferSize = sizeof(Vertex) * vertices.size();

        D3D12_HEAP_PROPERTIES heapProps = {};
        heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
        heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        heapProps.CreationNodeMask = 1;
        heapProps.VisibleNodeMask = 1;

        D3D12_RESOURCE_DESC resDesc = {};
        resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        resDesc.Alignment = 0;
        resDesc.Width = vertexBufferSize;
        resDesc.Height = 1;
        resDesc.DepthOrArraySize = 1;
        resDesc.MipLevels = 1;
        resDesc.Format = DXGI_FORMAT_UNKNOWN;
        resDesc.SampleDesc.Count = 1;
        resDesc.SampleDesc.Quality = 0;
        resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

        ComPtr<ID3D12Resource> vertexBuffer;
        device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &resDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&vertexBuffer));

        UINT8* pVertexDataBegin;
        D3D12_RANGE readRange = {};
        vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
        memcpy(pVertexDataBegin, vertices.data(), vertexBufferSize);
        vertexBuffer->Unmap(0, nullptr);

        return vertexBuffer;
    }

    struct ImageData
    {
        int width;
        int height;
        int depth;
        vector<uint8_t> data;
    };

    ImageData LoadBitmapFromFile(string path)
    {
        ImageData img = {};
        fstream fs(path, ios_base::in | ios_base::binary);
        if(!fs){
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

    shared_ptr<Texture> LoadTextureFromFile(ID3D12Device* device, string path)
    {
        auto img = LoadBitmapFromFile(path);
        if (img.data.empty()) {
            return nullptr;
        }

        auto tex = Texture::Create(device, img.width, img.height, img.depth, DXGI_FORMAT_B8G8R8A8_UNORM);
        tex->Write(img.data);

        return tex;
    }

    // clang-format off
    vector<Vertex> CreateSquareVertices()
    {
        vector<Vertex> data;
        data.emplace_back(Vertex { { -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } });
        data.emplace_back(Vertex { {  1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } });
        data.emplace_back(Vertex { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } });

        data.emplace_back(Vertex { { -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } });
        data.emplace_back(Vertex { {  1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } });
        data.emplace_back(Vertex { {  1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } });
        return data;
    }
}

void DxApp::Initialize(HWND hWnd, int width, int height)
{
    m_Hwnd = hWnd;
    m_Width = width;
    m_Height = height;

    m_ViewPort.TopLeftX = 0;
    m_ViewPort.TopLeftY = 0;
    m_ViewPort.Width = static_cast<float>(width);
    m_ViewPort.Height = static_cast<float>(height);
    m_ViewPort.MinDepth = D3D12_MIN_DEPTH;
    m_ViewPort.MaxDepth = D3D12_MAX_DEPTH;

    m_ScissorRect.left = 0;
    m_ScissorRect.top = 0;
    m_ScissorRect.right = width;
    m_ScissorRect.bottom = height;

    LoadPipeline();
    LoadAssets();
}

void DxApp::LoadPipeline()
{
    auto factory = CreateFactory();
    auto adapter = GetHardwareAdapter(factory.Get());
    m_Device = CreateDevice(adapter.Get());

    // command queue
    {
        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

        m_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CommandQueue));
    }

    // swap chain
    {
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.BufferCount = FRAME_COUNT;
        swapChainDesc.Width = m_Width;
        swapChainDesc.Height = m_Height;
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.SampleDesc.Count = 1;

        ComPtr<IDXGISwapChain1> swapChain;
        factory->CreateSwapChainForHwnd(m_CommandQueue.Get(), m_Hwnd, &swapChainDesc, nullptr, nullptr, &swapChain);
        factory->MakeWindowAssociation(m_Hwnd, DXGI_MWA_NO_ALT_ENTER);
        swapChain.As(&m_SwapChain);
    }

    // render target view
    {
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
        rtvHeapDesc.NumDescriptors = FRAME_COUNT;
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        m_Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_RtvHeap));

        UINT rtvDescSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        for (UINT i = 0; i < FRAME_COUNT; i++) {
            m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_RenderTargets[i]));
            m_RtvHandles[i] = m_RtvHeap->GetCPUDescriptorHandleForHeapStart();
            m_RtvHandles[i].ptr += static_cast<SIZE_T>(rtvDescSize) * i;
            m_Device->CreateRenderTargetView(m_RenderTargets[i].Get(), nullptr, m_RtvHandles[i]);
        }
    }

    // shader resource view
    {
        D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
        srvHeapDesc.NumDescriptors = 1;
        srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

        m_Device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_SrvHeap));
    }

    m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator));
}

void DxApp::LoadAssets()
{
    m_RootSignature = CreateRootSignature(m_Device.Get());
    m_PipelineState = CreatePipelineState(m_Device.Get(), m_RootSignature.Get());

    // command list
    m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator.Get(), m_PipelineState.Get(), IID_PPV_ARGS(&m_CommandList));

    {
        auto vertices = CreateSquareVertices();
        m_VertexBuffer = CreateVertexBuffer(m_Device.Get(), vertices);

        m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
        m_VertexBufferView.StrideInBytes = sizeof(Vertex);
        m_VertexBufferView.SizeInBytes = sizeof(Vertex) * vertices.size();
    }

    // texture
    {
        m_Texture = LoadTextureFromFile(m_Device.Get(), "./assets/cat_256x256_32bit.bmp");

        SetResourceBarrier(m_CommandList.Get(), m_Texture->Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Format = m_Texture->Get()->GetDesc().Format;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;
        m_Device->CreateShaderResourceView(m_Texture->Get(), &srvDesc, m_SrvHeap->GetCPUDescriptorHandleForHeapStart());
    }

    m_CommandList->Close();

    ID3D12CommandList* ppCommandLists[] = { m_CommandList.Get() };
    m_CommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // fence
    m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence));
    m_FenceValue = 1;
    m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

    WaitForPreviousFrame();
}

void DxApp::Render()
{
    PopulateCommandList();

    ID3D12CommandList* ppCommandLists[] = { m_CommandList.Get() };
    m_CommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    m_SwapChain->Present(1, 0);

    WaitForPreviousFrame();
}

void DxApp::PopulateCommandList()
{
    m_CommandAllocator->Reset();
    m_CommandList->Reset(m_CommandAllocator.Get(), m_PipelineState.Get());
    m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());

    ID3D12DescriptorHeap* heaps[] = { m_SrvHeap.Get() };
    m_CommandList->SetDescriptorHeaps(_countof(heaps), heaps);
    m_CommandList->SetGraphicsRootDescriptorTable(0, m_SrvHeap->GetGPUDescriptorHandleForHeapStart());

    UINT frameIndex = m_SwapChain->GetCurrentBackBufferIndex();

    SetResourceBarrier(m_CommandList.Get(), m_RenderTargets[frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

    {
        const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
        m_CommandList->ClearRenderTargetView(m_RtvHandles[frameIndex], clearColor, 0, nullptr);
    }

    m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_CommandList->IASetVertexBuffers(0, 1, &m_VertexBufferView);
    m_CommandList->RSSetViewports(1, &m_ViewPort);
    m_CommandList->RSSetScissorRects(1, &m_ScissorRect);
    m_CommandList->OMSetRenderTargets(1, &m_RtvHandles[frameIndex], FALSE, nullptr);
    m_CommandList->DrawInstanced(6, 1, 0, 0);

    SetResourceBarrier(m_CommandList.Get(), m_RenderTargets[frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

    m_CommandList->Close();
}

void DxApp::WaitForPreviousFrame()
{
    const UINT64 fence = m_FenceValue;
    m_CommandQueue->Signal(m_Fence.Get(), fence);
    m_FenceValue++;

    if (m_Fence->GetCompletedValue() < fence) {
        m_Fence->SetEventOnCompletion(fence, m_FenceEvent);
        WaitForSingleObject(m_FenceEvent, INFINITE);
    }
}
