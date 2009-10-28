#include "stdincludes.h"
#include "logging.h"
#include "error.h"
#include "timer.h"
#include "comptr.h"
#include "calluntil.h"
#include "window.h"
#include "vertexbuffer.h"
#include "renderdevice.h"
#include "displayenum.h"
#include "application.h"

using rebel::Application;

Application::Application(rebel::tstring const& appName)
    : _window(make_shared<Window>())
    , _appName(appName)
    , _deviceLost(false)
    , _managedResourcesOK(false)
    , _unmanagedResourcesOK(false)
    , _timer(make_shared<Timer>())
{
}

Application::~Application() {
}

int Application::run(rebel::tstring const& commandLine) {

    _d3d.attach(Direct3DCreate9(D3D_SDK_VERSION));
    if(!_d3d) {
        return EXIT_FAILURE;
    }

	_window->create(_appName, _T("LIBREBELAPPWINDOW"),
		WS_OVERLAPPEDWINDOW, WS_EX_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT, 400, 400);

	_window->connect_and_track_signal(
            WM_DESTROY, 
            shared_from_this(), 
            &Application::onDestroy, 
            99);

	_window->ShowWindow();
	_window->UpdateWindow();

    DisplayFormat format = {0};
    if(!getDisplayFormat(&format)) {
        startShutdown();
        return EXIT_FAILURE;
    }

    _displayFormat = format;

    D3DPRESENT_PARAMETERS pp = {0};
    pp.BackBufferWidth = format.mode.Width;
    pp.BackBufferHeight = format.mode.Height;
    pp.BackBufferFormat = format.backBufferFormat;
    pp.BackBufferCount = 0;
    pp.MultiSampleType = D3DMULTISAMPLE_NONE;
    pp.MultiSampleQuality = 0;
    pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pp.hDeviceWindow = _window->hWnd();
    pp.Windowed = !format.fullscreen;
    pp.EnableAutoDepthStencil = true;
    pp.AutoDepthStencilFormat = format.depthStencilFormat;
    pp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
    pp.FullScreen_RefreshRateInHz = format.fullscreen ? format.mode.RefreshRate: 0;
    pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    _presentParams = pp;

    IDirect3DDevice9Ptr dev;


    // Create the D3DDevice
    if(FAILED(_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, _window->hWnd(),
              D3DCREATE_HARDWARE_VERTEXPROCESSING|D3DCREATE_PUREDEVICE,
              &_presentParams, dev.addressOf())))
    {
        startShutdown();
        return EXIT_FAILURE;
    }

    RenderDevice::instance()->_device = dev;
    RenderDevice::instance()->_clearColor = D3DCOLOR_XRGB(0,0,0);
    RenderDevice::instance()->_clearDepth = true;
    RenderDevice::instance()->_clearStencil = formatStencilBits(format.depthStencilFormat) > 0;

    _deviceLost = false;

    if(!internalInitManaged()) {
        startShutdown();
        return EXIT_FAILURE;
    }

    if(!internalInitUnmanaged()) {
        startShutdown();
        return EXIT_FAILURE;
    }

	// Enter the message loop
	MSG msg = {0};
	while(msg.message!=WM_QUIT) {
		if(PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
            _timer->frameTick();
            update(_timer->getFrameTime());
            present();
        }
	}

	return EXIT_SUCCESS;
}

void Application::present() {
    HRESULT hr;

    if(_deviceLost) {
        hr = RenderDevice::instance()->raw()->TestCooperativeLevel();

        if(FAILED(hr)) {
            if(hr == D3DERR_DEVICELOST) {
                return;
            }

            if(hr == D3DERR_DEVICENOTRESET) {
                if(!resetDevice()) {
                    return;
                }
            }
        }

        _deviceLost = false;
    }

    RenderDevice::instance()->clear();

    if(RenderDevice::instance()->beginScene()) {
        render();
        RenderDevice::instance()->endScene();
    }

    hr = RenderDevice::instance()->raw()->Present(NULL, NULL, NULL, NULL);
    if(hr == D3DERR_DEVICELOST) {
        _deviceLost = true;
    }
}

bool Application::internalInitManaged() {
    _managedResourcesOK = true;

    if(!RenderDevice::instance()->_initManaged()) {
        internalDestroyManaged();
        return false;
    }

    return true;
}

bool Application::internalInitUnmanaged() {
    _unmanagedResourcesOK = true;

    if(!RenderDevice::instance()->_initUnmanaged()) {
        internalDestroyUnmanaged();
        internalDestroyManaged();
        return false;
    }

    return true;
}

void Application::internalDestroyManaged() {
    _managedResourcesOK = false;

    RenderDevice::instance()->_destroyManaged();
}

void Application::internalDestroyUnmanaged() {
    _unmanagedResourcesOK = false;

    RenderDevice::instance()->_destroyUnmanaged();
}

bool Application::resetDevice() {
    if(_unmanagedResourcesOK) {
        internalDestroyUnmanaged();
    }

    if(FAILED(RenderDevice::instance()->raw()->Reset(&_presentParams))) {
        return false;
    }

    return internalInitUnmanaged();
}

bool Application::onDestroy(LRESULT* pRV, rebel::Window&, UINT, WPARAM, LPARAM) {
    startShutdown();
    PostQuitMessage(0);
    *pRV = 0;
    return true;
}

void Application::startShutdown() {
    ULONG refCount = 0;

    internalDestroyUnmanaged();
    internalDestroyManaged();

    refCount = RenderDevice::instance()->_device.reset();
    LOG_INFO(tformat(_T("Device released with %1% remaining references\n")) % refCount);
    
    refCount = _d3d.reset();
    LOG_INFO(tformat(_T("D3D released with %1% remaining references\n")) % refCount);
}

