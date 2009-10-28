#include "stdincludes.h"
#include "logging.h"
#include "comptr.h"
#include "calluntil.h"
#include "renderdevice.h"

using rebel::RenderDevice;
using rebel::VertexBufferPtr;
using rebel::IndexBufferPtr;

rebel::RenderDevicePtr RenderDevice::instance() {
    static rebel::RenderDevicePtr   instance(new RenderDevice());
    return instance;
}

RenderDevice::RenderDevice()
    : _device()
{ }

rebel::IndexBufferPtr RenderDevice::createIndexBuffer(unsigned len, bool is32bit, bool writeOnly) {
    IDirect3DIndexBuffer9Ptr ibRaw;

    if(FAILED(_device->CreateIndexBuffer(len, writeOnly ? D3DUSAGE_WRITEONLY : 0, 
                    is32bit ? D3DFMT_INDEX32 : D3DFMT_INDEX16, 
                    D3DPOOL_DEFAULT, ibRaw.addressOf(), NULL)))
    {
        return IndexBufferPtr();
    }

    return IndexBufferPtr(new IndexBuffer(ibRaw));
}

rebel::VertexBufferPtr RenderDevice::createVertexBuffer(unsigned len, DWORD fvf, bool writeOnly) {
    IDirect3DVertexBuffer9Ptr vbRaw;

    if(FAILED(_device->CreateVertexBuffer(len, writeOnly ? D3DUSAGE_WRITEONLY : 0, fvf, 
                    D3DPOOL_DEFAULT, vbRaw.addressOf(), NULL)))
    {
        return VertexBufferPtr();
    }

    return VertexBufferPtr(new VertexBuffer(vbRaw));
}

void RenderDevice::setClearColor(D3DCOLOR clr) {
    _clearColor = clr;
}

void RenderDevice::clear() {
    DWORD clearFlags = D3DCLEAR_TARGET 
                     | (_clearDepth ? D3DCLEAR_ZBUFFER : 0) 
                     | (_clearStencil ? D3DCLEAR_STENCIL : 0) 
                     ;

    _device->Clear(0, NULL, clearFlags, _clearColor, 1.f, 0);
}

bool RenderDevice::beginScene() {
    return SUCCEEDED(_device->BeginScene());
}

void RenderDevice::endScene() {
    _device->EndScene();
}

