#ifndef LIBREBEL_RENDERDEVICE_H_
#define LIBREBEL_RENDERDEVICE_H_

#include <librebel/vertexbuffer.h>
#include <librebel/indexbuffer.h>

namespace rebel {

    class Application;

    class RenderDevice
        : public boost::noncopyable   
        , public boost::enable_shared_from_this<RenderDevice>
    {
    private:
        friend class Application;
        RenderDevice();

    public:
        static boost::shared_ptr<RenderDevice> instance();

    public:
        typedef boost::signals2::signal< bool(), call_until<false> > InitSignal;
        typedef boost::signals2::signal< void() > DestroySignal;

    public:
        InitSignal&     initManagedSignal()         { return _initManaged; }
        InitSignal&     initUnmanagedSignal()       { return _initUnmanaged; }

        //NOTE: There is no guarantee that either of the init signals have fired previously to the destroy calls
        DestroySignal&  destroyManagedSignal()      { return _destroyManaged; }
        DestroySignal&  destroyUnmanagedSignal()    { return _destroyUnmanaged; }


        template<typename ClassType, typename FuncType>
        boost::signals2::connection hookInitManaged(boost::shared_ptr<ClassType> ptr, FuncType func) {
            return _initManaged.connect(InitSignal::slot_type(func, ptr.get()).track(ptr));
        }
                
        /*
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
        */

    public:
        VertexBufferPtr createVertexBuffer(unsigned len, DWORD fvf, bool writeOnly);
        IndexBufferPtr createIndexBuffer(unsigned len, bool is32bit, bool writeOnly);

        IDirect3DDevice9Ptr raw() const { return _device; }

        void setClearColor(D3DCOLOR clr);

    public:
        void clear();
        bool beginScene();
        void endScene();

    private:
        IDirect3DDevice9Ptr     _device;
        InitSignal              _initManaged;
        InitSignal              _initUnmanaged;
        DestroySignal           _destroyManaged;
        DestroySignal           _destroyUnmanaged;
        D3DCOLOR                _clearColor;
        bool                    _clearDepth;
        bool                    _clearStencil;
    };


    typedef boost::shared_ptr<RenderDevice> RenderDevicePtr;

    template< typename FuncType
            , typename ClassType
            >
    boost::signals2::connection hookRenderDeviceInitManaged(
            FuncType func,
            boost::shared_ptr<ClassType> ptr
    ) {
        return RenderDevice::instance()->initManagedSignal().connect(
                RenderDevice::InitSignal::slot_type(func, ptr.get()).track(ptr)
            );
    }
                
    template< typename FuncType
            , typename ClassType
            >
    boost::signals2::connection hookRenderDeviceInitManaged(
            FuncType func,
            ClassType *ptr
    ) {
        return RenderDevice::instance()->initManagedSignal().connect(
                RenderDevice::InitSignal::slot_type(func, ptr)
            );
    }
                
    template< typename FuncType
            , typename ClassType
            >
    boost::signals2::connection hookRenderDeviceInitUnmanaged(
            FuncType func,
            boost::shared_ptr<ClassType> ptr
    ) {
        return RenderDevice::instance()->initUnmanagedSignal().connect(
                RenderDevice::InitSignal::slot_type(func, ptr.get()).track(ptr)
            );
    }
                
    template< typename FuncType
            , typename ClassType
            >
    boost::signals2::connection hookRenderDeviceInitUnmanaged(
            FuncType func,
            ClassType *ptr
    ) {
        return RenderDevice::instance()->initUnmanagedSignal().connect(
                RenderDevice::InitSignal::slot_type(func, ptr)
            );
    }

}

#endif // LIBREBEL_RENDERDEVICE_H_

