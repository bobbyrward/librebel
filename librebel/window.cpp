#include "stdincludes.h"
#include "calluntil.h"
#include "window.h"


using rebel::Window;


Window::Window() 
    : _hWnd(NULL)
    , _wndClass()
{
}

Window::~Window() {
    if(!_wndClass.empty()) {
        UnregisterClass(
            _wndClass.c_str(), 
            GetModuleHandle(NULL)
            );
    }
}

boost::signals2::connection Window::connect_signal(
        UINT msg, MessageSignal::slot_type const& slot, int priority
        )
{
    create_slot(msg);
    return _messages[msg]->connect(priority, slot);
}

bool Window::has_slot(UINT msg) {
    MessageMap::const_iterator ci = _messages.find(msg);
    return ci == _messages.end() ? false : !(*ci).second->empty();
}

void Window::create_slot(UINT msg) {
    if(!has_slot(msg)) {
        _messages[msg] = boost::make_shared<MessageSignal>();
    }
}

bool Window::create(const rebel::tstring& title, const rebel::tstring& wndClass, DWORD windowStyle,
        DWORD windowStyleEx, int x, int y, int width, int height)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, 
        &Window::WndProc, 0L, 0L,
        GetModuleHandle(NULL), NULL, LoadCursor( NULL, IDC_ARROW ),
        (HBRUSH)GetStockObject(WHITE_BRUSH), NULL, wndClass.c_str(), 
        NULL };

    _wndClass = wndClass;

    RegisterClassEx(&wc);

    _hWnd = CreateWindowEx(windowStyleEx, _wndClass.c_str(), 
        title.c_str(), windowStyle, x, y, width, height, NULL,
        NULL, wc.hInstance, this);

    return true;
}

LRESULT Window::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if(has_slot(msg)) {
        LRESULT rv = 0;
        bool handled = (*_messages[msg])(&rv, *this, msg, wParam, lParam);
        if(handled) return rv;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT __stdcall Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE:
            LPCREATESTRUCT lpCS;
            lpCS = reinterpret_cast<LPCREATESTRUCT>(LongToPtr(lParam));
            SetWindowLongPtr(hWnd, GWL_USERDATA, PtrToLong(lpCS->lpCreateParams));

        default:
            Window *p = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWL_USERDATA));
            if(p) {
                return p->MsgProc(hWnd, msg, wParam, lParam);
            } else {
                return DefWindowProc(hWnd, msg, wParam, lParam);
            }
    };
}
