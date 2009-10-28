#ifndef LIBREBEL_DISPLAYENUM_H_
#define LIBREBEL_DISPLAYENUM_H_

namespace rebel {
	struct DisplayFormat {
		UINT			adapterOrdinal;
        bool			fullscreen;
		D3DFORMAT		displayFormat;
		D3DFORMAT		backBufferFormat;
		D3DFORMAT		depthStencilFormat;
		D3DDISPLAYMODE	mode;
	};

	bool findAdapterMode(IDirect3D9Ptr d3d,
		DisplayFormat *pFormat, bool fullscreen, 
		unsigned minFullscreenWidth, unsigned minFullscreenHeight,
		unsigned minColorChannelBits, unsigned minAlphaBits,
		unsigned minDepthBits, unsigned minStencilBits);

    unsigned formatColorBits(D3DFORMAT fmt);
    unsigned formatAlphaBits(D3DFORMAT fmt);
    unsigned formatDepthBits(D3DFORMAT fmt);
    unsigned formatStencilBits(D3DFORMAT fmt);
}

#endif // LIBREBEL_DISPLAYENUM_H_

