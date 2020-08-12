#include "pch.h"
#include "DxApp.h"
#include "../shaders/gen/SampleVS.h"
#include "../shaders/gen/SamplePS.h"

using namespace DirectX;
using namespace Microsoft::WRL;

namespace
{
    ComPtr<IDXGIFactory4> CreateFactory()
    {
        HRESULT hr;
        ComPtr<IDXGIFactory4> factory;

        UINT flags = 0;
#ifdef _DEBUG
        {
            ComPtr<ID3D12Debug> debug;
            hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
            if (SUCCEEDED(hr)) {
                debug->EnableDebugLayer();
                flags |= DXGI_CREATE_FACTORY_DEBUG;
            }
        }
#endif
        hr = CreateDXGIFactory2(flags, IID_PPV_ARGS(&factory));
        if (FAILED(hr)) {
            return nullptr;
        }

        return factory;
    }

    ComPtr<IDXGIAdapter1> GetHardwareAdapter(IDXGIFactory2* factory)
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
        D3D12_RESOURCE_BARRIER descBarrier = {};
        descBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        descBarrier.Transition.pResource = resource;
        descBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        descBarrier.Transition.StateBefore = before;
        descBarrier.Transition.StateAfter = after;
        commandList->ResourceBarrier(1, &descBarrier);
    }

    ComPtr<ID3D12RootSignature> CreateRootSignature(ID3D12Device* device)
    {
        D3D12_ROOT_SIGNATURE_DESC sigDesc = {};
        sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

        ComPtr<ID3D12RootSignature> rootSignature;
        ComPtr<ID3DBlob> sig;
        ComPtr<ID3DBlob> error;
        D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &sig, &error);
        device->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&rootSignature));

        return rootSignature;
    }

    ComPtr<ID3D12PipelineState> CreatePipelineState(ID3D12Device* device, ID3D12RootSignature* rootSignature)
    {
        // Define the vertex input layout.
        D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };

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

    m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator));
}

void DxApp::LoadAssets()
{
    m_RootSignature = CreateRootSignature(m_Device.Get());
    m_PipelineState = CreatePipelineState(m_Device.Get(), m_RootSignature.Get());

    // command list
    m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_CommandList));

    m_CommandList->Close();

    // vertex buffer
    {
        float aspectRatio = 9.0 / 16.0;

        Vertex triangleVertices[] =
        {
            { { 0.0f, 0.25f * aspectRatio, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { { 0.25f, -0.25f * aspectRatio, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { { -0.25f, -0.25f * aspectRatio, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
        };

        const UINT vertexBufferSize = sizeof(triangleVertices);

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

        m_Device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &resDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&m_VertexBuffer)
        );

        UINT8* pVertexDataBegin;
        D3D12_RANGE readRange = {};
        m_VertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
        memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
        m_VertexBuffer->Unmap(0, nullptr);

        m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
        m_VertexBufferView.StrideInBytes = sizeof(Vertex);
        m_VertexBufferView.SizeInBytes = vertexBufferSize;
    }

    // fence
    m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence));
    m_FenceValue = 1;
    m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void DxApp::Render()
{
    m_CommandAllocator->Reset();
    m_CommandList->Reset(m_CommandAllocator.Get(), m_PipelineState.Get());
    m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
    m_CommandList->RSSetViewports(1, &m_ViewPort);
    m_CommandList->RSSetScissorRects(1, &m_ScissorRect);

    UINT frameIndex = m_SwapChain->GetCurrentBackBufferIndex();

    SetResourceBarrier(m_CommandList.Get(), m_RenderTargets[frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

    m_CommandList->OMSetRenderTargets(1, &m_RtvHandles[frameIndex], FALSE, nullptr);

    {
        const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
        m_CommandList->ClearRenderTargetView(m_RtvHandles[frameIndex], clearColor, 0, nullptr);
    }

    m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_CommandList->IASetVertexBuffers(0, 1, &m_VertexBufferView);
    m_CommandList->DrawInstanced(3, 1, 0, 0);

    SetResourceBarrier(m_CommandList.Get(), m_RenderTargets[frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

    m_CommandList->Close();

    ID3D12CommandList* ppCommandLists[] = { m_CommandList.Get() };
    m_CommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    m_SwapChain->Present(1, 0);

    WaitForPreviousFrame();
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
