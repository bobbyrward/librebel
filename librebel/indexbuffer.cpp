#include "stdincludes.h"
#include "logging.h"
#include "comptr.h"
#include "indexbuffer.h"

using rebel::IndexBuffer; 

IndexBuffer::IndexBuffer(rebel::IDirect3DIndexBuffer9Ptr ib)
    : _ib(ib)
{}


