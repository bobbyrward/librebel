#ifndef LIBREBEL_VERTEXBUFFER_H_
#define LIBREBEL_VERTEXBUFFER_H_

namespace rebel {

    class RenderDevice;

    class VertexBuffer 
        : public boost::noncopyable
    {
    private:
        friend class RenderDevice;
        VertexBuffer(IDirect3DVertexBuffer9Ptr vb);

    public:
        template<typename Vertex>
        Vertex* lock(unsigned vertexOffset, unsigned vertexCount, DWORD flags) {
            Vertex *locked;
            if(FAILED(_vb->Lock(vertexOffset * sizeof(Vertex), vertexCount * sizeof(Vertex),
                            reinterpret_cast<void**>(&locked), flags))) 
            {
                return NULL;
            }

            return locked;
        }

        void unlock() {
            _vb->Unlock();
        }

        IDirect3DVertexBuffer9Ptr raw() const { return _vb; }

    protected:
        IDirect3DVertexBuffer9Ptr   _vb;
    };

    typedef boost::shared_ptr<VertexBuffer> VertexBufferPtr;
}

#endif // LIBREBEL_VERTEXBUFFER_H_

