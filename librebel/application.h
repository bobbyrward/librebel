#ifndef LIBREBEL_APPLICATION_H_
#define LIBREBEL_APPLICATION_H_

namespace rebel {

    class Application 
        : public boost::enable_shared_from_this<Application>
        , public boost::noncopyable
    {
    public:
        typedef boost::signals2::signal< bool(), call_until<false> > InitSignal;
        typedef boost::signals2::signal< void() > DestroySignal;

    public:
        Application(tstring const& appName);
        virtual ~Application();

        int run(tstring const& commandLine);

    public:
        WindowPtr       window() const              { return _window; }
        tstring const & appName() const             { return _appName; }
        IDirect3D9Ptr   d3d() const                 { return _d3d; }
        TimerPtr        timer() const               { return _timer; }

    protected:
        virtual bool initialize() = 0;
        virtual void shutdown() = 0;
        virtual void update(float elapsedTime) = 0;
        virtual bool getDisplayFormat(DisplayFormat *format) = 0;
        virtual void render() = 0;

    private:
        bool onDestroy(LRESULT*, Window&, UINT, WPARAM, LPARAM);
        void startShutdown();
        void present();
        bool resetDevice();

        bool internalInitManaged();
        bool internalInitUnmanaged();
        void internalDestroyManaged();
        void internalDestroyUnmanaged();

    protected:
        WindowPtr               _window;
        tstring                 _appName;
        IDirect3D9Ptr           _d3d;
        D3DPRESENT_PARAMETERS   _presentParams;
        DisplayFormat           _displayFormat;
        bool                    _deviceLost;
        bool                    _managedResourcesOK;
        bool                    _unmanagedResourcesOK;
        TimerPtr                _timer;
    };

    typedef boost::shared_ptr<Application> ApplicationPtr;

}

#endif // LIBREBEL_APPLICATION_H_

