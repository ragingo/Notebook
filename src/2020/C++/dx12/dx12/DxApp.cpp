#include "pch.h"
#include "DxApp.h"

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
}

void DxApp::Initialize(HWND hWnd)
{
    auto factory = std::move(CreateFactory());
    auto adapter = std::move(GetHardwareAdapter(factory.Get()));
    auto device = std::move(CreateDevice(adapter.Get()));

    // command queue
    {
        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

        device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CommandQueue));
    }

    // swap chain
    {
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.BufferCount = FRAME_COUNT;
        swapChainDesc.Width= 500;
        swapChainDesc.Height = 500;
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.SampleDesc.Count = 1;

        ComPtr<IDXGISwapChain1> swapChain;
        factory->CreateSwapChainForHwnd(m_CommandQueue.Get(), hWnd, &swapChainDesc, nullptr, nullptr, &swapChain);
        factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);
        swapChain.As(&m_SwapChain);
    }

    // render target view
    {
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
        rtvHeapDesc.NumDescriptors = FRAME_COUNT;
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_RtvHeap));

        UINT rtvDescSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        for (UINT i = 0; i < FRAME_COUNT; i++) {
            m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_RenderTargets[i]));
            m_RtvHandles[i] = m_RtvHeap->GetCPUDescriptorHandleForHeapStart();
            m_RtvHandles[i].ptr += rtvDescSize * i;
            device->CreateRenderTargetView(m_RenderTargets[i].Get(), nullptr, m_RtvHandles[i]);
        }
    }

    device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator));

    // command list
    device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_CommandList));

    m_CommandList->Close();

    // fence
    device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence));
    m_FenceValue = 1;
    m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void DxApp::Render()
{
    m_CommandAllocator->Reset();
    m_CommandList->Reset(m_CommandAllocator.Get(), nullptr);

    UINT frameIndex = m_SwapChain->GetCurrentBackBufferIndex();

    SetResourceBarrier(m_CommandList.Get(), m_RenderTargets[frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

    {
        const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
        m_CommandList->ClearRenderTargetView(m_RtvHandles[frameIndex], clearColor, 0, nullptr);
    }

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
