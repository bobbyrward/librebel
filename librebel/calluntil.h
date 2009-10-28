#ifndef LIBREBEL_CALLUNTIL_H_
#define LIBREBEL_CALLUNTIL_H_

namespace rebel {
    template<bool UntilWhat=true>
    struct call_until {
        typedef bool result_type;

        template<typename InputIterator>
        bool operator()(InputIterator first, InputIterator last) {
            if(first != last) {
                for(bool rv = *first; first != last; rv=*++first) {
                    if(rv == UntilWhat) return UntilWhat;
                }
            }
            return !UntilWhat;
        }
    };
}

#endif // LIBREBEL_CALLUNTIL_H_

