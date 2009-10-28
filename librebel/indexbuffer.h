#ifndef LIBREBEL_INDEXBUFFER_H_
#define LIBREBEL_INDEXBUFFER_H_

namespace rebel {

    class RenderDevice;

    class IndexBuffer 
        : public boost::noncopyable
    {
    private:
        friend class RenderDevice;
        IndexBuffer(IDirect3DIndexBuffer9Ptr ib);

    public:
        template<typename IndexType>
        IndexType* lock(unsigned offset, unsigned count, DWORD flags) {
            IndexType *indices;

            if(FAILED(_ib->Lock(offset * sizeof(IndexType), count * sizeof(IndexType),
                            reinterpret_cast<void**>(&locked), flags)))
            {
                return NULL;
            }

            return indices;
        }

        void unlock() {
            _ib->Unlock();
        }

        IDirect3DIndexBuffer9Ptr raw() const { return _ib; }

    protected:
        IDirect3DIndexBuffer9Ptr   _ib;
    };

    typedef boost::shared_ptr<IndexBuffer> IndexBufferPtr;
}

#endif // LIBREBEL_INDEXBUFFER_H_


