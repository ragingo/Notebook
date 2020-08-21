#pragma once

class CTexture;

class CGfx
{
public:
    bool Initialize(HWND hWnd, int width, int height);

    void Update();

    void Render();

    void Uninitialize();

    struct Vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT4 color;
        DirectX::XMFLOAT2 texcoord;
    };

    struct ConstantBuffer
    {
        DirectX::XMFLOAT4 offset;
        float padding[60];
    };

private:
    void LoadPipeline();

    void LoadAssets();

    void PopulateCommandList();

    void WaitForPreviousFrame();

    static_assert((sizeof(ConstantBuffer) % 256) == 0, "Constant Buffer size must be 256-byte aligned");

    static const int FRAME_COUNT = 2;
    bool m_Initialized = false;
    int m_FenceValue = 0;
    HANDLE m_FenceEvent = INVALID_HANDLE_VALUE;
    D3D12_CPU_DESCRIPTOR_HANDLE m_RtvHandles[FRAME_COUNT] = {};
    D3D12_VIEWPORT m_ViewPort = {};
    D3D12_RECT m_ScissorRect = {};
    Microsoft::WRL::ComPtr<ID3D12Device> m_Device;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CommandQueue;
    Microsoft::WRL::ComPtr<IDXGISwapChain4> m_SwapChain;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_RenderTargets[FRAME_COUNT] = {};
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_RtvHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_BasicHeap;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandList;
    Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_RootSignature;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_PipelineState;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_ConstantBuffer;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_IndexBuffer;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_VertexBuffer;
    D3D12_INDEX_BUFFER_VIEW m_IndexBufferView = {};
    D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView = {};
    Microsoft::WRL::ComPtr<ID3DBlob> m_VertexShader;
    Microsoft::WRL::ComPtr<ID3DBlob> m_PixelShader;
    ConstantBuffer m_ConstantBufferData = {};
    ConstantBuffer* m_pCbvDataBegin = nullptr;
    std::shared_ptr<CTexture> m_Texture;
};
