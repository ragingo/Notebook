#include "CDxApp.h"
#include "CTexture.h"
#include "../../shaders/gen/SampleVS.h"
#include "../../shaders/gen/SamplePS.h"

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

    ComPtr<ID3D12CommandQueue> CreateCommandQueue(ID3D12Device* device)
    {
        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

        ComPtr<ID3D12CommandQueue> commandQueue;
        device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue));

        return commandQueue;
    }

    ComPtr<IDXGISwapChain4> CreateSwapChain(IDXGIFactory7* factory, ID3D12CommandQueue* commandQueue, int frameCount, int width, int height, HWND hWnd)
    {
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.BufferCount = frameCount;
        swapChainDesc.Width = width;
        swapChainDesc.Height = height;
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.SampleDesc.Count = 1;

        ComPtr<IDXGISwapChain1> swapChain1;
        factory->CreateSwapChainForHwnd(commandQueue, hWnd, &swapChainDesc, nullptr, nullptr, &swapChain1);
        factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);

        ComPtr<IDXGISwapChain4> swapChain4;
        swapChain1.As(&swapChain4);

        return swapChain4;
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
        ComPtr<ID3DBlob> sig;
        D3D12_VERSIONED_ROOT_SIGNATURE_DESC sigDesc = {};

        if (IsRootSignatureVersionAvaiable(device, D3D_ROOT_SIGNATURE_VERSION_1_1)) {
            CD3DX12_DESCRIPTOR_RANGE1 range1;
            range1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);

            CD3DX12_ROOT_PARAMETER1 rootParam1;
            rootParam1.InitAsDescriptorTable(1, &range1, D3D12_SHADER_VISIBILITY_PIXEL);

            CD3DX12_DESCRIPTOR_RANGE1 range2;
            range2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);

            CD3DX12_ROOT_PARAMETER1 rootParam2;
            rootParam2.InitAsDescriptorTable(1, &range2, D3D12_SHADER_VISIBILITY_VERTEX);

            D3D12_ROOT_PARAMETER1 rootParams[] = { rootParam1, rootParam2 };

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

            D3D12_ROOT_SIGNATURE_FLAGS flags =
                D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
                D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
                D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
                D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

            sigDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
            sigDesc.Desc_1_1.Flags = flags;
            sigDesc.Desc_1_1.NumStaticSamplers = 1;
            sigDesc.Desc_1_1.pStaticSamplers = &sampler;
            sigDesc.Desc_1_1.NumParameters = _countof(rootParams);
            sigDesc.Desc_1_1.pParameters = &rootParams[0];

            ComPtr<ID3DBlob> error;
            D3D12SerializeVersionedRootSignature(&sigDesc, &sig, &error);
        }
        else {
            sigDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_0;
            sigDesc.Desc_1_0.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

            ComPtr<ID3DBlob> error;
            D3D12SerializeVersionedRootSignature(&sigDesc, &sig, &error);
        }

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
        psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
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

    ComPtr<ID3D12Resource> CreateVertexBuffer(ID3D12Device* device, vector<Vertex> vertices)
    {
        const size_t bufferSize = sizeof(Vertex) * vertices.size();
        auto props = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        auto desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

        ComPtr<ID3D12Resource> vertexBuffer;
        device->CreateCommittedResource(
            &props,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&vertexBuffer));

        Vertex* dataBegin = nullptr;
        vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&dataBegin));
        memcpy(dataBegin, vertices.data(), bufferSize);
        vertexBuffer->Unmap(0, nullptr);

        return vertexBuffer;
    }

    ComPtr<ID3D12Resource> CreateConstantBuffer(ID3D12Device* device)
    {
        const auto bufferSize = sizeof(ConstantBuffer);
        auto props = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        auto desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

        ComPtr<ID3D12Resource> constantBuffer;
        device->CreateCommittedResource(
            &props,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&constantBuffer));

        return constantBuffer;
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

void CDxApp::Initialize(HWND hWnd, int width, int height)
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

    m_ScissorRect = { 0, 0, width, height };

    LoadPipeline();
    LoadAssets();

    m_Initialized = true;
}

void CDxApp::LoadPipeline()
{
    auto factory = CreateFactory();
    auto adapter = GetHardwareAdapter(factory.Get());
    m_Device = CreateDevice(adapter.Get());
    m_CommandQueue = CreateCommandQueue(m_Device.Get());
    m_SwapChain = CreateSwapChain(factory.Get(), m_CommandQueue.Get(), FRAME_COUNT, m_Width, m_Height, m_Hwnd);

    // render target view
    {
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
        rtvHeapDesc.NumDescriptors = FRAME_COUNT;
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        m_Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_RtvHeap));

        UINT rtvDescSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
        rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

        for (UINT i = 0; i < FRAME_COUNT; i++) {
            m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_RenderTargets[i]));
            m_RtvHandles[i] = m_RtvHeap->GetCPUDescriptorHandleForHeapStart();
            m_RtvHandles[i].ptr += static_cast<SIZE_T>(rtvDescSize) * i;
            m_Device->CreateRenderTargetView(m_RenderTargets[i].Get(), &rtvDesc, m_RtvHandles[i]);
        }
    }

    // descriptor heap
    {
        D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
        descriptorHeapDesc.NumDescriptors = 2;
        descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

        m_Device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&m_BasicHeap));
    }

    m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator));
}

void CDxApp::LoadAssets()
{
    m_RootSignature = CreateRootSignature(m_Device.Get());
    m_PipelineState = CreatePipelineState(m_Device.Get(), m_RootSignature.Get());

    // command list
    m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator.Get(), m_PipelineState.Get(), IID_PPV_ARGS(&m_CommandList));

    // vertex buffer
    {
        auto vertices = CreateSquareVertices();
        m_VertexBuffer = CreateVertexBuffer(m_Device.Get(), vertices);

        m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
        m_VertexBufferView.StrideInBytes = sizeof(Vertex);
        m_VertexBufferView.SizeInBytes = static_cast<UINT>(sizeof(Vertex) * vertices.size());
    }

    // texture
    {
        m_Texture = LoadTextureFromFile2(m_Device.Get(), m_CommandList.Get(), L"./assets/cat_256x256_32bit.bmp");
        //m_Texture = LoadTextureFromFile2(m_Device.Get(), m_CommandList.Get(), L"./assets/cat_256x256_lz77.png");

        auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_Texture->Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
        m_CommandList->ResourceBarrier(1, &barrier);

        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Format = m_Texture->Get()->GetDesc().Format;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;
        m_Device->CreateShaderResourceView(m_Texture->Get(), &srvDesc, m_BasicHeap->GetCPUDescriptorHandleForHeapStart());
    }

    // constant buffer
    {
        m_ConstantBuffer = CreateConstantBuffer(m_Device.Get());

        auto handle = m_BasicHeap->GetCPUDescriptorHandleForHeapStart();
        handle.ptr += m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
        cbvDesc.BufferLocation = m_ConstantBuffer->GetGPUVirtualAddress();
        cbvDesc.SizeInBytes = sizeof(m_ConstantBufferData);
        m_Device->CreateConstantBufferView(&cbvDesc, handle);

        m_ConstantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_pCbvDataBegin));
        *m_pCbvDataBegin = m_ConstantBufferData;
    }

    m_CommandList->Close();

    ID3D12CommandList* commandLists[] = { m_CommandList.Get() };
    m_CommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

    // fence
    m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence));
    m_FenceValue = 1;
    m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

    WaitForPreviousFrame();
}

void CDxApp::Render()
{
    if (!m_Initialized) {
        return;
    }

    const float translationSpeed = 0.005f;
    const float offsetBounds = 1.25f;

    m_ConstantBufferData.offset.x += translationSpeed;
    if (m_ConstantBufferData.offset.x > offsetBounds)
    {
        m_ConstantBufferData.offset.x = -offsetBounds;
    }
    *m_pCbvDataBegin = m_ConstantBufferData;

    PopulateCommandList();

    ID3D12CommandList* commandLists[] = { m_CommandList.Get() };
    m_CommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

    m_SwapChain->Present(1, 0);

    WaitForPreviousFrame();
}

void CDxApp::PopulateCommandList()
{
    m_CommandAllocator->Reset();
    m_CommandList->Reset(m_CommandAllocator.Get(), m_PipelineState.Get());
    m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());

    ID3D12DescriptorHeap* heaps[] = { m_BasicHeap.Get() };
    m_CommandList->SetDescriptorHeaps(_countof(heaps), heaps);

    auto handle = m_BasicHeap->GetGPUDescriptorHandleForHeapStart();
    m_CommandList->SetGraphicsRootDescriptorTable(0, handle);
    handle.ptr += m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    m_CommandList->SetGraphicsRootDescriptorTable(1, handle);

    UINT frameIndex = m_SwapChain->GetCurrentBackBufferIndex();

    {
        auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_RenderTargets[frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
        m_CommandList->ResourceBarrier(1, &barrier);
    }

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

    {
        auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_RenderTargets[frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
        m_CommandList->ResourceBarrier(1, &barrier);
    }

    m_CommandList->Close();
}

void CDxApp::WaitForPreviousFrame()
{
    const UINT64 fence = m_FenceValue;
    m_CommandQueue->Signal(m_Fence.Get(), fence);
    m_FenceValue++;

    if (m_Fence->GetCompletedValue() < fence) {
        m_Fence->SetEventOnCompletion(fence, m_FenceEvent);
        WaitForSingleObject(m_FenceEvent, INFINITE);
    }
}

void CDxApp::Destroy()
{
    if (!m_Initialized) {
        return;
    }

    WaitForPreviousFrame();

    CloseHandle(m_FenceEvent);
}
