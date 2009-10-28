#include "stdincludes.h"
#include "logging.h"
#include "comptr.h"
#include "vertexbuffer.h"

using rebel::VertexBuffer; 

VertexBuffer::VertexBuffer(rebel::IDirect3DVertexBuffer9Ptr vb)
    : _vb(vb)
{}

