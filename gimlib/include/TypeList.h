#ifndef INCLUDED_GIMLIB_TYPELIST
#define INCLUDED_GIMLIB_TYPELIST

#include "TypeManip.h"
#include "TypeListMacros.h"

/////////////////////////////////////////////////////
namespace GimLib {
/////////////////////////////////////////////////////

//! �^���X�g�̏I�[�\���ȂǂɎg�p�����^
class NullType {};

//! ���\������^
class EmptyType{};
//! EmptyType�̃I�u�W�F�N�g�̓��l����i���true�j
inline bool operator==(const EmptyType&, const EmptyType&) {
	return true;
}
//! EmptyType�̃I�u�W�F�N�g�̑召����i���false�j
inline bool operator<(const EmptyType&, const EmptyType&) {
	return false;
}
//! EmptyType�̃I�u�W�F�N�g�̑召����i���false�j
inline bool operator>(const EmptyType&, const EmptyType&) {
	return false;
}

//! �^���X�g
template <typename T, typename U>
struct Typelist
{
	typedef T Head;
	typedef U Tail;
};

/*!
	\namespace DkLibrary::TL
	\brief
		�^���X�g�𑀍삷�邽�߂̃N���X�e���v���[�g�Q
*/
/////////////////////////////////////////////////////
namespace TL {
/////////////////////////////////////////////////////
	/*!
		\struct DkLibrary::TL::SizeNum
		\tparam TList �^���X�g
		\brief
			�^���X�g�Ɋ܂܂��^�̃T�C�Y���v���Z�o����B@n
			SizeSum<TList>::value �Ō��ʂ��擾�ł���B
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
		\tparam TList �^���X�g
		\brief
			�^���X�g�̒������Z�o����B@n
			Length<TList>::value �Ō��ʂ��擾�ł���B
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
		\tparam TList �^���X�g
		\tparam idx �ʒu���w�肷�鐮��
		\brief
			�^���X�g����^����ꂽ�C���f�N�X�̈ʒu�ɂ���^�����o���B@n
			TypeAt<TList,idx>::value �Ō��ʂ��擾�ł���B
		\note
			���X�g�͈̔͊O�ƂȂ�C���f�N�X���w�肵���ꍇ�A�R���p�C���G���[�ƂȂ�B
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
		\tparam TList �^���X�g
		\tparam idx �ʒu���w�肷�鐮��
		\tparam DefaultType	�C���f�N�X���͈͊O�̏ꍇ�ɕԂ��^�B�f�t�H���g�ł�NullType�B
		\brief
			�^���X�g����^����ꂽ�C���f�N�X�̈ʒu�ɂ���^�����o���B@n
			TypeAtNonStrict<TList,idx>::value �Ō��ʂ��擾�ł���B
		\note
			���X�g�͈̔͊O�ƂȂ�C���f�N�X���w�肵���ꍇ�ADefaultType��Ԃ��B
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
		\tparam TList	�^���X�g
		\tparam T		�����Ώۂ̌^
		\brief
			�^���X�g����w��̌^���������A���̈ʒu�������C���f�N�X���擾����B@n
			IndexOf<TList, T>::value �Ō��ʂ��擾�ł���B
		\note
			������Ȃ������ꍇ��-1��Ԃ��B
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
		\tparam TList	�^���X�g
		\tparam T		�ǉ�����^�i�^���X�g�ł��ǂ��j
		\brief
			�^���X�g�ɗv�f�i�^or�^���X�g�j��ǉ�����B@n
			Append<TList, T>::Result ��T�̒ǉ����ꂽ�^���X�g���擾�ł���B
	*/
	//�^���X�g�Ɍ^T��ǉ�����
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
		\tparam TList	�^���X�g
		\tparam T		�폜����^
		\brief
			�^���X�gTList����ŏ��Ɍ�������T���폜����B@n
			Erase<TList, T>::Result �ō폜��̌^���X�g���擾�ł���B
		\note
			T�����������ꍇ�A���̌^���X�g��Ԃ��B
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
		\tparam TList	�^���X�g
		\tparam T		�폜����^
		\brief
			�^���X�gTList����S�Ă�T���폜����B@n
			EraseAll<TList, T>::Result �Ō��ʂ��擾�ł���B
		\note
			T�����������ꍇ�A���̌^���X�g��Ԃ��B
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
		\tparam TList	�^���X�g
		\brief
			�^���X�gTList����S�Ă̏d������菜���B@n
			NoDuplicates<TList>::Result �Ō��ʂ��擾�ł���B
	*/
	template <class TList> struct NoDuplicates;
	template <> struct NoDuplicates<NullType> {
		typedef NullType Result;
	};
	template <typename Head, typename Tail>
	struct NoDuplicates<Typelist<Head,Tail> > {
	private:
		typedef typename NoDuplicates<Tail>::Result L1;
		//��L1�͏d���̂Ȃ����X�g�Ȃ̂ŁAEraseAll�ł͂Ȃ��AErase�ŗǂ�
		typedef typename Erase<L1, Head>::Result L2;
	public:
		typedef Typelist<Head,L2> Result;
	};

	/*!
		\struct DkLibrary::TL::Replace
		\tparam TList	�^���X�g
		\tparam T		�u�������^
		\tparam U		�u������^
		\brief
			�^���X�gTList����ŏ��Ɍ�������T��U�ɒu������B@n
			Replace<TList, T, U>::Result �Ō��ʂ��擾�ł���B
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
		\tparam TList	�^���X�g
		\tparam T		�u�������^
		\tparam U		�u������^
		\brief
			�^���X�gTList�̑S�Ă�T��U�ɒu������B@n
			ReplaceAll<TList, T, U> �Ō��ʂ��擾�ł���B
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
		\tparam TList	�^���X�g
		\brief
			�^���X�gTList�̕��я��𔽓]����B@n
			Reverse<TList>::Result �Ō��ʂ��擾�ł���B
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
		\tparam TList	�^���X�g
		\tparam T		�h���N���X��T��������{�^
		\brief
			�^���X�gTList����T�̍ł��h�����ꂽ�^���擾����B@n
			MostDerived<TList, T>::Result �Ō��ʂ��擾�ł���B
		\note
			�h���N���X��������Ȃ������ꍇ��T��Ԃ��B
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
		\tparam TList	�^���X�g
		\brief
			�^���X�gTList��[���h�����ꂽ�^���O�ɗ���悤�ɕ��ёւ���B@n
			DerivedToFront<TList>::Result �Ō��ʂ��擾�ł���B
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