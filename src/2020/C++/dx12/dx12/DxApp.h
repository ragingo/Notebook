#pragma once

class DxApp
{
public:
    void Initialize(HWND hWnd);
    void Render();
    void WaitForPreviousFrame();

private:
    static const int FRAME_COUNT = 2;
    int m_FenceValue = 0;
    HANDLE m_FenceEvent = INVALID_HANDLE_VALUE;
    D3D12_CPU_DESCRIPTOR_HANDLE m_RtvHandles[FRAME_COUNT];
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CommandQueue;
    Microsoft::WRL::ComPtr<IDXGISwapChain3> m_SwapChain;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_RenderTargets[FRAME_COUNT];
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_RtvHeap;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandList;
    Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence;
};
