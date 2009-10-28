#include <librebel/rebel.h>

bool destroy_window(LRESULT* pRV, rebel::Window& window, UINT msg, WPARAM wParam, LPARAM lParam) {
	PostQuitMessage(0);
	*pRV = 0;
	return true;
}

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT) {
	rebel::WindowPtr wnd = boost::make_shared<rebel::Window>();

	wnd->create(_T("Window Title"), _T("TESTWndCLass"),
		WS_OVERLAPPEDWINDOW, WS_EX_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT, 400, 400);

	wnd->connect_signal(WM_DESTROY, destroy_window, 99);
	wnd->ShowWindow();
	wnd->UpdateWindow();

	// Enter the message loop
	MSG msg = {0};
	while(msg.message!=WM_QUIT) {
		if(PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}
