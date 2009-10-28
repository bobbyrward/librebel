#ifndef LIBREBEL_COMPTR_H_
#define LIBREBEL_COMPTR_H_

namespace rebel {

    template< typename IInterface >
    class comptr {
    public:
        comptr();
        comptr(comptr<IInterface> const& rhs);
        explicit comptr(IInterface* pi);
        virtual ~comptr();

        comptr& operator = (comptr<IInterface> const& rhs);

        IInterface* operator -> () const;
        operator bool() const;

        IInterface* get() const;
        IInterface** addressOf();
        ULONG reset();

        void attach(IInterface* pi);
        IInterface* detach();

    private:
        IInterface *_pi;
    };


#if(DIRECT3D_VERSION >= 0x0900)
    typedef comptr<IDirect3D9>                  IDirect3D9Ptr;
    typedef comptr<IDirect3DBaseTexture9>       IDirect3DBaseTexture9Ptr;
    typedef comptr<IDirect3DCubeTexture9>       IDirect3DCubeTexture9Ptr;
    typedef comptr<IDirect3DDevice9>            IDirect3DDevice9Ptr;
    typedef comptr<IDirect3DIndexBuffer9>       IDirect3DIndexBuffer9Ptr;
    typedef comptr<IDirect3DPixelShader9>       IDirect3DPixelShader9Ptr;
    typedef comptr<IDirect3DQuery9>             IDirect3DQuery9Ptr;
    typedef comptr<IDirect3DResource9>          IDirect3DResource9Ptr;
    typedef comptr<IDirect3DStateBlock9>        IDirect3DStateBlock9Ptr;
    typedef comptr<IDirect3DSurface9>           IDirect3DSurface9Ptr;
    typedef comptr<IDirect3DSwapChain9>         IDirect3DSwapChain9Ptr;
    typedef comptr<IDirect3DTexture9>           IDirect3DTexture9Ptr;
    typedef comptr<IDirect3DVertexBuffer9>      IDirect3DVertexBuffer9Ptr;
    typedef comptr<IDirect3DVertexDeclaration9> IDirect3DVertexDeclaration9Ptr;
    typedef comptr<IDirect3DVertexShader9>      IDirect3DVertexShader9Ptr;
    typedef comptr<IDirect3DVolume9>            IDirect3DVolume9Ptr;
    typedef comptr<IDirect3DVolumeTexture9>     IDirect3DVolumeTexture9Ptr;

#endif

#if(D3DX_VERSION >= 0x0900)
    typedef comptr<ID3DXBuffer>                 ID3DXBufferPtr;
    typedef comptr<ID3DXFont>                   ID3DXFontPtr;
    typedef comptr<ID3DXSprite>                 ID3DXSpritePtr;
    typedef comptr<ID3DXRenderToSurface>        ID3DXRenderToSurfacePtr;
    typedef comptr<ID3DXRenderToEnvMap>         ID3DXRenderToEnvMapPtr;
    typedef comptr<ID3DXLine>                   ID3DXLinePtr;
#endif


    template<typename IInterface>
    comptr<IInterface>::comptr()
        : _pi(NULL)
    { }

    template<typename IInterface>
    comptr<IInterface>::comptr(comptr<IInterface> const& rhs) 
        : _pi(NULL)
    {
        *this = rhs;
    }

    template<typename IInterface>
    comptr<IInterface>::comptr(IInterface* pi) 
        : _pi(pi)
    { }

    template<typename IInterface>
    comptr<IInterface>::~comptr() {
        reset();
    }

    template<typename IInterface>
    comptr<IInterface>& comptr<IInterface>::operator = (comptr<IInterface> const& rhs) {
        attach(rhs.get());
        if(_pi) {
            _pi->AddRef();
        }
        return *this;
    }

    template<typename IInterface>
    comptr<IInterface>::operator bool() const {
        return _pi != NULL;
    }

    template<typename IInterface>
    IInterface* comptr<IInterface>::operator -> () const {
        return _pi;
    }

    template<typename IInterface>
    IInterface* comptr<IInterface>::get() const {
        return _pi;
    }

    template<typename IInterface>
    IInterface** comptr<IInterface>::addressOf() {
        assert(_pi == NULL);
        return &_pi;
    }

    template<typename IInterface>
    ULONG comptr<IInterface>::reset() {
        ULONG refCount = 0;

        if(_pi) {
            refCount = _pi->Release();
            _pi = NULL;
        }

        return refCount;
    }

    template<typename IInterface>
    void comptr<IInterface>::attach(IInterface* pi) {
        reset();
        _pi = pi;
    }

    template<typename IInterface>
    IInterface* comptr<IInterface>::detach() {
        IInterface *pi = _pi;
        _pi = NULL;
        return pi;
    }

}

#endif // LIBREBEL_COMPTR_H_

