#include "stdincludes.h"
#include "error.h"

rebel::tstring rebel::dxErrorString(HRESULT hr) {
    return DXGetErrorString9(hr);
}

rebel::tstring rebel::dxErrorDescription(HRESULT hr) {
    return DXGetErrorDescription9(hr);
}

