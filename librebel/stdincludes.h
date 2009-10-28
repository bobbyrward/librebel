#ifndef LIBREBEL_STDINCLUDES_H_
#define LIBREBEL_STDINCLUDES_H_

#include <windows.h>
#include <tchar.h>

#include <cassert>
#include <cmath>

#include <boost/signals2.hpp>

#include <boost/unordered/unordered_map.hpp>

#include <boost/format.hpp>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>

#include <string>
#include <vector>
#include <list>
#include <algorithm>


namespace rebel {
    using boost::make_shared;

#ifdef UNICODE
    typedef std::wstring	tstring;
    typedef boost::wformat  tformat;
    #define WIDEN2(x) L ## x
    #define WIDEN(x) WIDEN2(x)
    #define __TFILE__ WIDEN(__FILE__)
#else
    typedef std::string		tstring;
    typedef boost::format   tformat;
    #define __TFILE__ __FILE__
#endif
}


#endif // LIBREBEL_STDINCLUDES_H_

