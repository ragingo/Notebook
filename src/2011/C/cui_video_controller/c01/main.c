
#undef  __cplusplus

#pragma warning(disable:4214)

#include <tchar.h>
#include <conio.h>
#pragma comment(lib, "strmiids.lib")
#include <dshow.h>

#define BUTTON_PLAY         '5'
#define BUTTON_VOLUME_UP    '8'
#define BUTTON_VOLUME_DOWN  '2'
#define BUTTON_QUIT         ' '

enum PlayState
{
	Playing,
	Paused
};

typedef struct tagMediaObject
{
	IGraphBuilder *pGraphBuilder;
	IMediaControl *pMediaControl;
	IVideoWindow  *pVideoWindow;
	IBasicAudio   *pBasicAudio;
	enum PlayState state;
} MediaObject;

void Initialize(MediaObject *obj)
{
	CoCreateInstance(&CLSID_FilterGraph, 0, CLSCTX_INPROC_SERVER, &IID_IFilterGraph, (void**)&obj->pGraphBuilder);
	obj->pGraphBuilder->lpVtbl->QueryInterface(obj->pGraphBuilder, &IID_IMediaControl, (void**)&obj->pMediaControl);
	obj->pGraphBuilder->lpVtbl->QueryInterface(obj->pGraphBuilder, &IID_IVideoWindow, (void**)&obj->pVideoWindow);
	obj->pGraphBuilder->lpVtbl->QueryInterface(obj->pGraphBuilder, &IID_IBasicAudio, (void**)&obj->pBasicAudio);
}

void Uninitialize(MediaObject *obj)
{
	obj->pBasicAudio->lpVtbl->Release(obj->pBasicAudio);
	obj->pVideoWindow->lpVtbl->Release(obj->pVideoWindow);
	obj->pMediaControl->lpVtbl->Release(obj->pMediaControl);
	obj->pGraphBuilder->lpVtbl->Release(obj->pGraphBuilder);
}

void RenderFile(MediaObject *obj, LPCTSTR filename)
{
	obj->pGraphBuilder->lpVtbl->RenderFile(obj->pGraphBuilder, filename, 0);
}

void Play(MediaObject *obj)
{
	obj->pMediaControl->lpVtbl->Run(obj->pMediaControl);
	obj->state = Playing;
}

void Pause(MediaObject *obj)
{
	obj->pMediaControl->lpVtbl->Pause(obj->pMediaControl);
	obj->state = Paused;
}

void SetVolume(MediaObject *obj, long value)
{
	long current = 0L;
	obj->pBasicAudio->lpVtbl->get_Volume(obj->pBasicAudio, &current);
	obj->pBasicAudio->lpVtbl->put_Volume(obj->pBasicAudio, current + value); 
}

int _tmain(int argc, TCHAR** argv)
{
	const TCHAR *filename = TEXT("C:\\Users\\Public\\Videos\\Sample Videos\\Wildlife.wmv");
	int key = 0;
	MediaObject obj;

	CoInitialize(NULL);
	Initialize(&obj);
	RenderFile(&obj, filename);
	Play(&obj);

	while((key=_getch()) != BUTTON_QUIT)
	{
		switch(key)
		{
		case BUTTON_PLAY:
			switch(obj.state)
			{
			case Playing:
				Pause(&obj);
				break;
			case Paused:
				Play(&obj);
				break;
			}
			break;

		case BUTTON_VOLUME_UP:
			SetVolume(&obj, 1000L);
			break;

		case BUTTON_VOLUME_DOWN:
			SetVolume(&obj, -1000L);
			break;
		}
	}

	Uninitialize(&obj);
	CoUninitialize();
}

