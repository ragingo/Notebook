#pragma once

template <typename T>
class AsyncCallback :
	public IMFAsyncCallback
{
private:
	typedef HRESULT (T::*AsyncCallbackDelegate)(IMFAsyncResult *pAysncResult);
	T *m_Parent;
	AsyncCallbackDelegate m_Delegate;
public:

	AsyncCallback(T *parent, AsyncCallbackDelegate delegate) :
		m_Parent(parent),
		m_Delegate(delegate)
	{
	}

	STDMETHODIMP_(ULONG) AddRef()
	{
		return m_Parent->AddRef();
	}

	STDMETHODIMP_(ULONG) Release()
	{
		return m_Parent->Release();
	}

	STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject)
	{
		if (!ppvObject)
		{
			return E_POINTER;
		}
		if (riid == __uuidof(IUnknown))
		{
			*ppvObject = static_cast<IUnknown*>(this);
		}
		else if (riid == __uuidof(IMFAsyncCallback))
		{
			*ppvObject = static_cast<IMFAsyncCallback*>(this);
		}
		else
		{
			*ppvObject = NULL;
			return E_NOINTERFACE;
		}
		AddRef();
		return S_OK;
	}

	STDMETHODIMP GetParameters(DWORD*, DWORD*)
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP Invoke(IMFAsyncResult *pAsyncResult)
	{
		return (m_Parent->*m_Delegate)(pAsyncResult);
	}
};