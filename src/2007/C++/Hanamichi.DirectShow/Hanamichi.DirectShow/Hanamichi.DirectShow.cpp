// Hanamichi.DirectShow.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//char *str = _com_util::ConvertBSTRToString(L"123ABC\n\n");
	//printf_s("%s", str);

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	CoInitialize(NULL);
	Hanamichi::DirectShow::DeviceControl2 *mc = new Hanamichi::DirectShow::DeviceControl2();
	//Hanamichi::DirectShow::DeviceControl *mc = new Hanamichi::DirectShow::DeviceControl(NULL);
	//Hanamichi::DirectShow::MediaFileControl *mc = new Hanamichi::DirectShow::MediaFileControl(NULL);
	//Hanamichi::DirectShow::DvdControl *mc = new Hanamichi::DirectShow::DvdControl(NULL, "E:\\", 10);
// �����p����

	mc->OpenDevice();
	mc->ShowVideoWindow(NULL, true);
	mc->BeginCapture();
	//mc->Initialize();
	//mc->OnFileOpen("C:\\Users\\Gateway_WinVista\\Desktop\\mongol_4.avi", lstrlenA("C:\\Users\\Gateway_WinVista\\Desktop\\mongol_4.avi"));
	//mc->ShowVideoWindow(NULL);
	//mc->StartCapture();
	//mc->Play();
	getchar();
	//mc->Stop();

	delete mc;
	//CoUninitialize();

/*
IGraphBuilder *pGraphBuilder;
 ICaptureGraphBuilder2 *pCaptureGraphBuilder2;
 IMediaControl *pMediaControl;

 // �⑫���j�p�P��̓��{���
 //   Enumerate : �񋓁A�ꗗ
 //   Moniker   : ������
 ICreateDevEnum *pCreateDevEnum = NULL;
 IEnumMoniker *pEnumMoniker = NULL;
 IMoniker *pMoniker = NULL;

 ULONG nFetched = 0;
 IBaseFilter *pDeviceFilter;

 // COM��������
 CoInitialize(NULL);

 // FilterGraph�𐶐�
 CoCreateInstance(CLSID_FilterGraph,
	NULL,
	CLSCTX_INPROC,
	IID_IGraphBuilder,
	(LPVOID *)&pGraphBuilder);

 // CaptureGraphBuilder2�Ƃ����L���v�`���pGraphBuilder�𐶐�����
 CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, 
   IID_ICaptureGraphBuilder2, 
   (LPVOID *)&pCaptureGraphBuilder2);

 // FilterGraph���Z�b�g����
 pCaptureGraphBuilder2->SetFiltergraph(pGraphBuilder);

 // MediaControl�C���^�[�t�F�[�X�擾
 pGraphBuilder->QueryInterface(IID_IMediaControl,
	(LPVOID *)&pMediaControl);

 // �f�o�C�X��񋓂��邽�߂�CreateDevEnum�𐶐�
 CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, 
   IID_ICreateDevEnum, (PVOID *)&pCreateDevEnum);
    
 // VideoInputDevice��񋓂��邽�߂�EnumMoniker�𐶐� 
 pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,
   &pEnumMoniker, 0);
 if (pEnumMoniker == NULL) {
   // �ڑ����ꂽ�f�����̓f�o�C�X����������ꍇ�ɂ͂���if���ɓ���܂�
   printf("no device\n");
   return 0;
 }

 // EnumMoniker��Reset����
 // Reset����ƁA�擪���琔���Ȃ����܂�
 pEnumMoniker->Reset();

 // �ŏ���Moniker���擾
 pEnumMoniker->Next(1, &pMoniker, &nFetched);

 // Monkier��Filter��Bind����
 pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&pDeviceFilter );

 // FilterGraph�Ƀf�o�C�X�t�B���^��ǉ�����
 pGraphBuilder->AddFilter(pDeviceFilter, L"Device Filter");

 // ���̎��_��Monker�n�AEnumerate�n�͗p�ς�
 pMoniker->Release();
 pEnumMoniker->Release();
 pCreateDevEnum->Release();

 // Graph�𐶐�����
 pCaptureGraphBuilder2->RenderStream(&PIN_CATEGORY_PREVIEW,
   NULL, pDeviceFilter, NULL, NULL);

 // �Đ��J�n
 pMediaControl->Run();

 // �Đ����Ƀv���O�������I����Ă��܂�Ȃ��悤��
 MessageBoxA(NULL,
	"Block Execution",
	"Block",
	MB_OK);

 // ���������
 pMediaControl->Release();
 pCaptureGraphBuilder2->Release();
 pGraphBuilder->Release();*/

 // COM�I��
 CoUninitialize();
	return 0;
}

