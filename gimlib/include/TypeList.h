#ifndef INCLUDED_GIMLIB_TYPELIST
#define INCLUDED_GIMLIB_TYPELIST

#include "TypeManip.h"
#include "TypeListMacros.h"

/////////////////////////////////////////////////////
namespace GimLib {
/////////////////////////////////////////////////////

//! 型リストの終端表現などに使用される型
class NullType {};

//! 空を表現する型
class EmptyType{};
//! EmptyTypeのオブジェクトの等値判定（常にtrue）
inline bool operator==(const EmptyType&, const EmptyType&) {
	return true;
}
//! EmptyTypeのオブジェクトの大小判定（常にfalse）
inline bool operator<(const EmptyType&, const EmptyType&) {
	return false;
}
//! EmptyTypeのオブジェクトの大小判定（常にfalse）
inline bool operator>(const EmptyType&, const EmptyType&) {
	return false;
}

//! 型リスト
template <typename T, typename U>
struct Typelist
{
	typedef T Head;
	typedef U Tail;
};

/*!
	\namespace DkLibrary::TL
	\brief
		型リストを操作するためのクラステンプレート群
*/
/////////////////////////////////////////////////////
namespace TL {
/////////////////////////////////////////////////////
	/*!
		\struct DkLibrary::TL::SizeNum
		\tparam TList 型リスト
		\brief
			型リストに含まれる型のサイズ合計を算出する。@n
			SizeSum<TList>::value で結果を取得できる。
	*/
	template <typename TList> struct SizeSum;
	template <> struct SizeSum<NullType> {
		static const size_t value = 0;
	};
	template <typename T, typename U> struct SizeSum<Typelist<T, U> > {
		static const size_t value = sizeof(T) + SizeSum<U>::value;
	};


	/*!
		\struct DkLibrary::TL::Length
		\tparam TList 型リスト
		\brief
			型リストの長さを算出する。@n
			Length<TList>::value で結果を取得できる。
	*/
	template <typename TList> struct Length;
	template <> struct Length<NullType> {
		static const int value = 0;
	};
	template <typename T, typename U> struct Length< Typelist<T, U> > {
		static const int value = 1 + Length<U>::value;
	};

	/*!
		\struct DkLibrary::TL::TypeAt
		\tparam TList 型リスト
		\tparam idx 位置を指定する整数
		\brief
			型リストから与えられたインデクスの位置にある型を取り出す。@n
			TypeAt<TList,idx>::value で結果を取得できる。
		\note
			リストの範囲外となるインデクスを指定した場合、コンパイルエラーとなる。
	*/
	template <typename TList, unsigned int idx> struct TypeAt;
	template <typename T, typename U>
	struct TypeAt< Typelist<T,U>, 0 > { typedef T Result; };
	template <typename T, typename U, unsigned idx>
	struct TypeAt< Typelist<T,U>, idx > {
		typedef typename TypeAt<U, idx - 1>::Result Result;
	};

	/*!
		\struct DkLibrary::TL::TypeAtNonStrict
		\tparam TList 型リスト
		\tparam idx 位置を指定する整数
		\tparam DefaultType	インデクスが範囲外の場合に返す型。デフォルトではNullType。
		\brief
			型リストから与えられたインデクスの位置にある型を取り出す。@n
			TypeAtNonStrict<TList,idx>::value で結果を取得できる。
		\note
			リストの範囲外となるインデクスを指定した場合、DefaultTypeを返す。
	*/
	template <typename TList, unsigned int idx, typename DefaultType = NullType>
	struct TypeAtNonStrict {
		typedef DefaultType Result;
	};
	template <typename T, typename U, typename DefaultType>
	struct TypeAtNonStrict<Typelist<T,U>,0,DefaultType> {
		typedef T Result;
	};
	template <typename T, typename U, unsigned int idx, typename DefaultType>
	struct TypeAtNonStrict<Typelist<T,U>,idx,DefaultType> {
		typedef typename
			TypeAtNonStrict<U,idx-1,DefaultType>::Result Result;
	};

	/*!
		\struct DkLibrary::TL::IndexOf
		\tparam TList	型リスト
		\tparam T		検索対象の型
		\brief
			型リストから指定の型を検索し、その位置を示すインデクスを取得する。@n
			IndexOf<TList, T>::value で結果を取得できる。
		\note
			見つからなかった場合は-1を返す。
	*/
	template <typename TList, typename T> struct IndexOf;
	template <typename T> struct IndexOf<NullType, T> {
		static const int value = -1;
	};
	template <typename T, typename TAIL>
	struct IndexOf<Typelist<T, TAIL>, T> {
		static const int value = 0;
	};
	template <typename HEAD, typename TAIL, typename T>
	struct IndexOf<Typelist<HEAD, TAIL>, T> {
	private:
		static const int temp = IndexOf<TAIL, T>::value;
	public:
		static const int value = ( temp == -1 ? -1 : 1 + temp );
	};

	/*!
		\struct DkLibrary::TL::Append
		\tparam TList	型リスト
		\tparam T		追加する型（型リストでも良い）
		\brief
			型リストに要素（型or型リスト）を追加する。@n
			Append<TList, T>::Result でTの追加された型リストを取得できる。
	*/
	//型リストに型Tを追加する
	template <class TList, typename T> struct Append;
	template <> struct Append<NullType, NullType> {
		typedef NullType Result;
	};
	template <typename T> struct Append<NullType, T> {
		typedef Typelist<T, NullType> Result;
	};
	template <typename Head, typename Tail>
	struct Append<NullType, Typelist<Head,Tail> > {
		typedef Typelist<Head,Tail> Result;
	};
	template <typename Head, typename Tail, typename T>
	struct Append<Typelist<Head,Tail>, T > {
		typedef Typelist<Head, typename Append<Tail, T>::Result> Result;
	};

	/*!
		\struct DkLibrary::TL::Erase
		\tparam TList	型リスト
		\tparam T		削除する型
		\brief
			型リストTListから最初に見つかったTを削除する。@n
			Erase<TList, T>::Result で削除後の型リストを取得できる。
		\note
			Tが無かった場合、元の型リストを返す。
	*/
	template <class TList, typename T> struct Erase;
	template <typename T> struct Erase<NullType, T> {
		typedef NullType Result;
	};
	template <typename Tail, typename T>
	struct Erase<Typelist<T, Tail>, T> {
		typedef Tail Result;
	};
	template <typename Head, typename Tail, typename T>
	struct Erase<Typelist<Head,Tail>,T> {
		typedef Typelist<Head, typename Erase<Tail, T>::Result> Result;
	};

	/*!
		\struct DkLibrary::TL::EraseAll
		\tparam TList	型リスト
		\tparam T		削除する型
		\brief
			型リストTListから全てのTを削除する。@n
			EraseAll<TList, T>::Result で結果を取得できる。
		\note
			Tが無かった場合、元の型リストを返す。
	*/
	template <class TList, typename T> struct EraseAll;
	template <typename T> struct EraseAll<NullType, T> {
		typedef NullType Result;
	};
	template <typename Tail, typename T>
	struct EraseAll<Typelist<T,Tail>,T> {
		typedef typename EraseAll<Tail,T>::Result Result;
	};
	template <typename Head, typename Tail, typename T>
	struct EraseAll<Typelist<Head,Tail>,T> {
		typedef Typelist<	Head,
							typename EraseAll<Tail,T>::Result> Result;
	};

	/*!
		\struct DkLibrary::TL::NoDuplicates
		\tparam TList	型リスト
		\brief
			型リストTListから全ての重複を取り除く。@n
			NoDuplicates<TList>::Result で結果を取得できる。
	*/
	template <class TList> struct NoDuplicates;
	template <> struct NoDuplicates<NullType> {
		typedef NullType Result;
	};
	template <typename Head, typename Tail>
	struct NoDuplicates<Typelist<Head,Tail> > {
	private:
		typedef typename NoDuplicates<Tail>::Result L1;
		//※L1は重複のないリストなので、EraseAllではなく、Eraseで良い
		typedef typename Erase<L1, Head>::Result L2;
	public:
		typedef Typelist<Head,L2> Result;
	};

	/*!
		\struct DkLibrary::TL::Replace
		\tparam TList	型リスト
		\tparam T		置換される型
		\tparam U		置換する型
		\brief
			型リストTListから最初に見つかったTをUに置換する。@n
			Replace<TList, T, U>::Result で結果を取得できる。
	*/
	template <typename TList, typename T, typename U> struct Replace;
	template <typename T, typename U> struct Replace<NullType, T, U> {
		typedef NullType Result;
	};
	template <typename Tail, typename T, typename U>
	struct Replace<Typelist<T,Tail>,T,U> {
		typedef Typelist<U,Tail> Result;
	};
	template <typename Head, typename Tail, typename T, typename U>
	struct Replace<Typelist<Head,Tail>,T,U> {
		typedef Typelist<	Head,
							typename Replace<Tail,T,U>::Result> Result;
	};

	/*!
		\struct DkLibrary::TL::ReplaceAll
		\tparam TList	型リスト
		\tparam T		置換される型
		\tparam U		置換する型
		\brief
			型リストTListの全てのTをUに置換する。@n
			ReplaceAll<TList, T, U> で結果を取得できる。
	*/
	template <typename TList, typename T, typename U> struct ReplaceAll;
	template <typename T, typename U> struct ReplaceAll<NullType,T,U> {
		typedef NullType Result;
	};
	template <typename Tail, typename T, typename U>
	struct ReplaceAll<Typelist<T,Tail>,T,U> {
		typedef Typelist<	U, 
							typename ReplaceAll<Tail,T,U>::Result> Result;
	};
	template <typename Head, typename Tail, typename T, typename U>
	struct ReplaceAll<Typelist<Head,Tail>,T,U> {
		typedef Typelist<	Head,
							typename ReplaceAll<Tail,T,U>::Result> Result;
	};

	/*!
		\struct DkLibrary::TL::Reverse
		\tparam TList	型リスト
		\brief
			型リストTListの並び順を反転する。@n
			Reverse<TList>::Result で結果を取得できる。
	*/
	template <typename TList> struct Reverse;
	template <> struct Reverse<NullType> {
		typedef NullType Result;
	};
	template <typename Head, typename Tail>
	struct Reverse<Typelist<Head,Tail> > {
		typedef typename Append<
			typename Reverse<Tail>::Result,Head>::Result Result;
	};

	/*!
		\struct DkLibrary::TL::MostDerived
		\tparam TList	型リスト
		\tparam T		派生クラスを探したい基本型
		\brief
			型リストTListからTの最も派生された型を取得する。@n
			MostDerived<TList, T>::Result で結果を取得できる。
		\note
			派生クラスが見つからなかった場合はTを返す。
	*/
	template <typename TList, typename T> struct MostDerived;
	template <typename T> struct MostDerived<NullType,T> {
		typedef T Result;
	};
	template <typename Head, typename Tail, typename T>
	struct MostDerived<Typelist<Head,Tail>,T> {
	private:
		typedef typename MostDerived<Tail,T>::Result Candidate;
	public:
		typedef typename Select<
			SuperSubclass<Candidate,Head>::value,
				Head,Candidate>::Result Result;
	};

	/*!
		\struct DkLibrary::TL::DerivedToFront
		\tparam TList	型リスト
		\brief
			型リストTListを深く派生された型が前に来るように並び替える。@n
			DerivedToFront<TList>::Result で結果を取得できる。
	*/
	template <typename TList> struct DerivedToFront;
	template <> struct DerivedToFront<NullType> {
		typedef NullType Result;
	};
	template <typename Head, typename Tail>
	struct DerivedToFront<Typelist<Head,Tail> > {
	private:
		typedef typename MostDerived<Tail,Head>::Result TheMostDerived;
		typedef typename Replace<Tail,TheMostDerived,Head>::Result Temp;
		typedef typename DerivedToFront<Temp>::Result L;
	public:
		typedef Typelist<TheMostDerived,L> Result;
	};

/////////////////////////////////////////////////////
} // end namespace TL
} // end namespace GimLib
/////////////////////////////////////////////////////

#endif