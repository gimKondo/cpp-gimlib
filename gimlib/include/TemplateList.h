#ifndef INCLUDED_GIMLIB_TEMPLATELIST
#define INCLUDED_GIMLIB_TEMPLATELIST

#include "TypeList.h"

/////////////////////////////////////////////////////
namespace GimLib {
/////////////////////////////////////////////////////

//! テンプレートクラス リスト
template <typename TPARAM, template <class> class T, typename U>
struct TemplateList
{
	typedef T<TPARAM> Head;
	typedef U Tail;
};

/////////////////////////////////////////////////////
} // end namespace GimLib
/////////////////////////////////////////////////////

#define TEMPLATELIST_1(TP, T1) GimLib::TemplateList<TP, T1, GimLib::NullType>
#define TEMPLATELIST_2(TP, T1, T2) GimLib::TemplateList<TP, T1, TEMPLATELIST_1(TP, T2) >
#define TEMPLATELIST_3(TP, T1, T2, T3) GimLib::TemplateList<TP, T1, TEMPLATELIST_2(TP, T2, T3) >
#define TEMPLATELIST_4(TP, T1, T2, T3, T4) GimLib::TemplateList<TP, T1, TEMPLATELIST_3(TP, T2, T3, T4) >
#define TEMPLATELIST_5(TP, T1, T2, T3, T4, T5) GimLib::TemplateList<TP, T1, TEMPLATELIST_4(TP, T2, T3, T4, T5) >

#endif
