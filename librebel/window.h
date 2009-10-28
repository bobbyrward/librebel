#ifndef LIBREBEL_WINDOW_H_
#define LIBREBEL_WINDOW_H_


namespace rebel {

    class Window 
        : public boost::enable_shared_from_this<Window>
        , public boost::noncopyable
    {
    public:
        typedef boost::signals2::signal<
            bool(LRESULT*,Window&,UINT,WPARAM,LPARAM), 
            call_until<true> 
        > MessageSignal;

        typedef boost::shared_ptr<
            MessageSignal
        > MessageSignalPtr;

        typedef boost::unordered_map<
            UINT, 
            MessageSignalPtr
        > MessageMap;

    public: 
        Window();
        virtual ~Window();

        HWND hWnd() const {
            return _hWnd;
        }

        bool create(const tstring& title, const tstring& wndClass, 
			DWORD windowStyle=WS_OVERLAPPEDWINDOW, DWORD windowStyleEx=WS_EX_OVERLAPPEDWINDOW, 
			int x=CW_USEDEFAULT, int y=CW_USEDEFAULT, int width=CW_USEDEFAULT, int height=CW_USEDEFAULT);


        boost::signals2::connection connect_signal(UINT msg, MessageSignal::slot_type const& slot, int priority=0);

        template<typename T, typename TFunc>
        boost::signals2::connection connect_and_track_signal(
                UINT msg, boost::shared_ptr<T> ptr, TFunc func, int priority=0
                ) 
        {
            create_slot(msg);
            return _messages[msg]->connect(priority,
                MessageSignal::slot_type(func, ptr.get(), _1, _2, _3, _4, _5).track(ptr)
                );
        }

    private: 
        bool has_slot(UINT msg);
        void create_slot(UINT msg);

        LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        static LRESULT __stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    public: /* WRAPPED SDK METHODS */
        BOOL ShowWindow(int nCmdShow=SW_SHOWDEFAULT) {
            return ::ShowWindow(_hWnd, nCmdShow);
        }

        BOOL UpdateWindow() {
            return ::UpdateWindow(_hWnd);
        }

        HWND SetCapture() {
            return ::SetCapture(_hWnd);
        }

        BOOL ScreenToClient( LPPOINT ptCursor ) {
            return ::ScreenToClient(_hWnd, ptCursor);
        }

        BOOL DestroyWindow() {
            return ::DestroyWindow(_hWnd);
        }

        BOOL CloseWindow() {
            return ::CloseWindow(_hWnd);
        }


    private:
        MessageMap		_messages;
        HWND			_hWnd;
        tstring			_wndClass;
    };

    typedef boost::shared_ptr<Window> WindowPtr;
}


#endif // LIBREBEL_WINDOW_H_

