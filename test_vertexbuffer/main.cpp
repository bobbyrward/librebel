#include <librebel/rebel.h>

class TestApp 
    : public rebel::Application
{
public:
    TestApp() 
        : rebel::Application(_T("TestVertexBuffer"))
        , _statsString()
        , _lastStatsUpdate(0)
    {
    }

    virtual ~TestApp() {
    }

public:
    virtual bool getDisplayFormat(rebel::DisplayFormat *format) { 
        return rebel::findAdapterMode(d3d(), format, false, 0, 0, 5, 0, 15, 0);
    }

    bool initUnmanagedResources() {
		return true;
    }

    void destroyUnmanagedResources() {
    }

    virtual void render() { 
    }

    virtual bool initialize() { 
        rebel::hookRenderDeviceInitUnmanaged(&TestApp::initUnmanagedResources, this);
        return true;
    }

    virtual void shutdown() { 
    }

    virtual void update(float elapsedTime) { 
        static rebel::tformat  statsFormatter(_T("FPS: %0.2f"));

        if(_timer->getTimeSince(_lastStatsUpdate) > 0.25f) {
            _statsString = boost::str(rebel::tformat(statsFormatter) % _timer->getFramesPerSecond());
            _lastStatsUpdate = _timer->getRawTime();
        }
    }

private:
    rebel::tstring  _statsString;
    long long       _lastStatsUpdate;
};


INT WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR cmdLine, INT) {
    rebel::Logging::addOutputHandler(
            boost::make_shared<rebel::LogOutputHandler_Debugger>()
            );

    rebel::Logging::setMinLevel(rebel::LogLevel_Debug);

    return boost::make_shared<TestApp>()->run(cmdLine);
}
