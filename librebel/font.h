#ifndef LIBREBEL_FONT_H_
#define LIBREBEL_FONT_H_

namespace rebel {

    class Font {
    public:
        Font(tstring const& face, float size, bool bold=false, bool italic=false);
        virtual ~Font();
    
    private:
        ID3DXFontPtr    _font;
        float           _size;
        tstring         _face;
        bool            _bold;
        bool            _italic;
    };

}

#endif // LIBREBEL_FONT_H_

