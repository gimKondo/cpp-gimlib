#ifndef INCLUDED_GIMLIB_TYPECAST
#define INCLUDED_GIMLIB_TYPECAST

namespace GimLib {

//! const��t������
template<typename T>
const T& add_const(const T& x) { return x; }


} // end namespace GimLib

#endif