#ifndef INCLUDED_GIMLIB_REPOSITORY_ENTITY_MACRO
#define INCLUDED_GIMLIB_REPOSITORY_ENTITY_MACRO

#include <map>
#include "container_make.h"

/***********************************************************************/
/*!	@brief macro for generating entity class
************************************************************************
	@details
		For accessing to repository usuing IRepository, 
		you have to write class that express ENTITY.
		And IRepository should receive those classes as type parameter.
		These macro creates the entity class.
************************************************************************
	@par how to use
		pass to the macro (1)entity class name, (2)table name of repository,
		(3) 1st data type of field, (4) 1st field name 
		(5) 2nd data type of field, (6) 2nd field name...
	@code
		RPS_ENTITY_CLASS3(ProfileEntity, Profile, STRING, FirstName, STRING, LastName, INT, Age)
	@endcode
************************************************************************
	@par about entity class
		This macro create entity class for accessing the repository.
		This class has following interface.

		- constructor
			- set just data type without parameters
			- set field data with field data
		 	- set field data with values of field
		- static member
			- TABLE_NAME		function to get table name
			- FIELD_INFO_LIST	information list about fields
			- F_[FIELD_NAME]	field name
		- instance member
			- property			access field data as field data type
			- indxer			access field data by field name
	@code
		ProfileEntity entity;
		//get field data as typed data
		std::wstfing firstName entity.FirstName;
		//get field data
		RPS_FieldData firstNameData = entity[L"FirstName"];
		//update field data
		entity[L"FirstName"].StringData = L"Gim";
	@endcode
************************************************************************
	@author		gim_kondo
	@version	1.0
	@date		2013/6/24
************************************************************************/

#define RPS_INT_CTYPE()			int
#define RPS_DOUBLE_CTYPE()		double
#define RPS_STRING_CTYPE()		std::wstring
#define RPS_INT_ARG_CTYPE()		int
#define RPS_DOUBLE_ARG_CTYPE()	double
#define RPS_STRING_ARG_CTYPE()	const std::wstring&
#define RPS_INT_FIELD_NAME()	IntData
#define RPS_DOUBLE_FIELD_NAME()	DoubleData
#define RPS_STRING_FIELD_NAME()	StringData
#define RPS_TABLE_NAME_DEFINE(name)	static const std::wstring TABLE_NAME() { static const std::wstring o = L#name; return o; }
#define RPS_ENTITY_FIELD_DEFINE(type, name)															\
	__declspec(property(get=Get##name)) RPS_##type##_CTYPE() name;									\
	RPS_##type##_CTYPE() Get##name() const {return zzFields.at(L#name).RPS_##type##_FIELD_NAME(); }	\
	static const std::wstring& F_##name() { static const std::wstring o = L#name; return o; }


//------------------------------------------------------
// create 3 fields entity
//------------------------------------------------------
#define RPS_ENTITY_CLASS3(class_name, table_name, type1, name1, type2, name2, type3, name3)	\
class class_name {						\
public:									\
	RPS_TABLE_NAME_DEFINE(table_name)	\
	static const std::vector<RPS_FieldInfo> FIELD_INFO_LIST(){	\
		const static std::vector<RPS_FieldInfo> o				\
			= (GimLib::make_vector<RPS_FieldInfo>()				\
				,RPS_FieldInfo(RPS_VT_##type1, L#name1)			\
				,RPS_FieldInfo(RPS_VT_##type2, L#name2)			\
				,RPS_FieldInfo(RPS_VT_##type3, L#name3));		\
		return o;												\
	}															\
	class_name() {											\
		zzFields[L#name1] = RPS_FieldData(RPS_VT_##type1);	\
		zzFields[L#name2] = RPS_FieldData(RPS_VT_##type2);	\
		zzFields[L#name3] = RPS_FieldData(RPS_VT_##type3);	\
	}														\
	class_name(		RPS_FieldData name1##_, RPS_FieldData name2##_		\
				,	RPS_FieldData name3##_							) {	\
		zzFields[L#name1] = name1##_;									\
		zzFields[L#name2] = name2##_;									\
		zzFields[L#name3] = name3##_;									\
	}																	\
	class_name(	RPS_##type1##_ARG_CTYPE() name1##_, RPS_##type2##_ARG_CTYPE() name2##_,		\
				RPS_##type3##_ARG_CTYPE() name3##_										) {	\
		zzFields[L#name1] = RPS_FieldData(name1##_);	\
		zzFields[L#name2] = RPS_FieldData(name2##_);	\
		zzFields[L#name3] = RPS_FieldData(name3##_);	\
	}													\
	RPS_ENTITY_FIELD_DEFINE(type1,name1)	\
	RPS_ENTITY_FIELD_DEFINE(type2,name2)	\
	RPS_ENTITY_FIELD_DEFINE(type3,name3)	\
	const RPS_FieldData& operator[](const std::wstring& key) const {	\
		if (key == L#name1) { return zzFields.at(L#name1); }	\
		if (key == L#name2) { return zzFields.at(L#name2); }	\
		if (key == L#name3) { return zzFields.at(L#name3); }	\
		return RPS_EMPTY_FIELD;									\
	}															\
	RPS_FieldData& operator[](const std::wstring& key) {	\
		if (key == L#name1) { return zzFields[L#name1]; }	\
		if (key == L#name2) { return zzFields[L#name2]; }	\
		if (key == L#name3) { return zzFields[L#name3]; }	\
		throw 0;											\
	}														\
private:											\
	std::map<std::wstring, RPS_FieldData> zzFields;	\
};

//------------------------------------------------------
// create 4 fields entity
//------------------------------------------------------
#define RPS_ENTITY_CLASS4(class_name, table_name	, type1, name1, type2, name2	\
													, type3, name3, type4, name4)	\
class class_name {												\
public:															\
	RPS_TABLE_NAME_DEFINE(table_name)							\
	static const std::vector<RPS_FieldInfo> FIELD_INFO_LIST(){	\
		const static std::vector<RPS_FieldInfo> o				\
			= (GimLib::make_vector<RPS_FieldInfo>()				\
				,RPS_FieldInfo(RPS_VT_##type1, L#name1)			\
				,RPS_FieldInfo(RPS_VT_##type2, L#name2)			\
				,RPS_FieldInfo(RPS_VT_##type3, L#name3)			\
				,RPS_FieldInfo(RPS_VT_##type4, L#name4));		\
		return o;												\
	}															\
	class_name() {											\
		zzFields[L#name1] = RPS_FieldData(RPS_VT_##type1);	\
		zzFields[L#name2] = RPS_FieldData(RPS_VT_##type2);	\
		zzFields[L#name3] = RPS_FieldData(RPS_VT_##type3);	\
		zzFields[L#name4] = RPS_FieldData(RPS_VT_##type4);	\
	}														\
	class_name(		RPS_FieldData name1##_, RPS_FieldData name2##_		\
				,	RPS_FieldData name3##_, RPS_FieldData name4##_ ) {	\
		zzFields[L#name1] = name1##_;									\
		zzFields[L#name2] = name2##_;									\
		zzFields[L#name3] = name3##_;									\
		zzFields[L#name4] = name4##_;									\
	}																	\
	class_name(	RPS_##type1##_ARG_CTYPE() name1##_, RPS_##type2##_ARG_CTYPE() name2##_,		\
				RPS_##type3##_ARG_CTYPE() name3##_, RPS_##type4##_ARG_CTYPE() name4##_ ) {	\
		zzFields[L#name1] = RPS_FieldData(name1##_);	\
		zzFields[L#name2] = RPS_FieldData(name2##_);	\
		zzFields[L#name3] = RPS_FieldData(name3##_);	\
		zzFields[L#name4] = RPS_FieldData(name4##_);	\
	}													\
	RPS_ENTITY_FIELD_DEFINE(type1,name1)	\
	RPS_ENTITY_FIELD_DEFINE(type2,name2)	\
	RPS_ENTITY_FIELD_DEFINE(type3,name3)	\
	RPS_ENTITY_FIELD_DEFINE(type4,name4)	\
	const RPS_FieldData& operator[](const std::wstring& key) const {	\
		if (key == L#name1) { return zzFields.at(L#name1); }	\
		if (key == L#name2) { return zzFields.at(L#name2); }	\
		if (key == L#name3) { return zzFields.at(L#name3); }	\
		if (key == L#name4) { return zzFields.at(L#name4); }	\
		return RPS_EMPTY_FIELD;									\
	}															\
	RPS_FieldData& operator[](const std::wstring& key) {	\
		if (key == L#name1) { return zzFields[L#name1]; }	\
		if (key == L#name2) { return zzFields[L#name2]; }	\
		if (key == L#name3) { return zzFields[L#name3]; }	\
		if (key == L#name4) { return zzFields[L#name4]; }	\
		throw 0;											\
	}														\
private:											\
	std::map<std::wstring, RPS_FieldData> zzFields;	\
};

//------------------------------------------------------
// create 5 fields entity
//------------------------------------------------------
#define RPS_ENTITY_CLASS5(class_name, table_name,	type1, name1, type2, name2,					\
													type3, name3, type4, name4, type5, name5)	\
class class_name {												\
public:															\
	RPS_TABLE_NAME_DEFINE(table_name)							\
	static const std::vector<RPS_FieldInfo> FIELD_INFO_LIST(){	\
		const static std::vector<RPS_FieldInfo> o				\
			= (GimLib::make_vector<RPS_FieldInfo>()				\
				,RPS_FieldInfo(RPS_VT_##type1, L#name1)			\
				,RPS_FieldInfo(RPS_VT_##type2, L#name2)			\
				,RPS_FieldInfo(RPS_VT_##type3, L#name3)			\
				,RPS_FieldInfo(RPS_VT_##type4, L#name4)			\
				,RPS_FieldInfo(RPS_VT_##type5, L#name5));		\
		return o;												\
	}															\
	class_name() {											\
		zzFields[L#name1] = RPS_FieldData(RPS_VT_##type1);	\
		zzFields[L#name2] = RPS_FieldData(RPS_VT_##type2);	\
		zzFields[L#name3] = RPS_FieldData(RPS_VT_##type3);	\
		zzFields[L#name4] = RPS_FieldData(RPS_VT_##type4);	\
		zzFields[L#name5] = RPS_FieldData(RPS_VT_##type5);	\
	}														\
	class_name(		RPS_FieldData name1##_, RPS_FieldData name2##_, RPS_FieldData name3##_		\
				,	RPS_FieldData name4##_, RPS_FieldData name5##_) {							\
		zzFields[L#name1] = name1##_;															\
		zzFields[L#name2] = name2##_;															\
		zzFields[L#name3] = name3##_;															\
		zzFields[L#name4] = name4##_;															\
		zzFields[L#name5] = name5##_;															\
	}																							\
	class_name(	RPS_##type1##_ARG_CTYPE() name1##_, RPS_##type2##_ARG_CTYPE() name2##_, RPS_##type3##_ARG_CTYPE() name3##_,	\
				RPS_##type4##_ARG_CTYPE() name4##_, RPS_##type5##_ARG_CTYPE() name5##_ ) {									\
		zzFields[L#name1] = RPS_FieldData(name1##_);	\
		zzFields[L#name2] = RPS_FieldData(name2##_);	\
		zzFields[L#name3] = RPS_FieldData(name3##_);	\
		zzFields[L#name4] = RPS_FieldData(name4##_);	\
		zzFields[L#name5] = RPS_FieldData(name5##_);	\
	}													\
	RPS_ENTITY_FIELD_DEFINE(type1,name1)	\
	RPS_ENTITY_FIELD_DEFINE(type2,name2)	\
	RPS_ENTITY_FIELD_DEFINE(type3,name3)	\
	RPS_ENTITY_FIELD_DEFINE(type4,name4)	\
	RPS_ENTITY_FIELD_DEFINE(type5,name5)	\
	const RPS_FieldData& operator[](const std::wstring& key) const {	\
		if (key == L#name1) { return zzFields.at(L#name1); }	\
		if (key == L#name2) { return zzFields.at(L#name2); }	\
		if (key == L#name3) { return zzFields.at(L#name3); }	\
		if (key == L#name4) { return zzFields.at(L#name4); }	\
		if (key == L#name5) { return zzFields.at(L#name5); }	\
		return RPS_EMPTY_FIELD;									\
	}															\
	RPS_FieldData& operator[](const std::wstring& key) {	\
		if (key == L#name1) { return zzFields[L#name1]; }	\
		if (key == L#name2) { return zzFields[L#name2]; }	\
		if (key == L#name3) { return zzFields[L#name3]; }	\
		if (key == L#name4) { return zzFields[L#name4]; }	\
		if (key == L#name5) { return zzFields[L#name5]; }	\
		throw 0;											\
	}														\
private:											\
	std::map<std::wstring, RPS_FieldData> zzFields;	\
};

//------------------------------------------------------
// create 6 fields entity
//------------------------------------------------------
#define RPS_ENTITY_CLASS6(class_name, table_name,	type1, name1, type2, name2,	type3, name3,	\
													type4, name4, type5, name5, type6, name6)	\
class class_name {												\
public:															\
	RPS_TABLE_NAME_DEFINE(table_name)							\
	static const std::vector<RPS_FieldInfo> FIELD_INFO_LIST(){	\
		const static std::vector<RPS_FieldInfo> o				\
			= (GimLib::make_vector<RPS_FieldInfo>()				\
				,RPS_FieldInfo(RPS_VT_##type1, L#name1)			\
				,RPS_FieldInfo(RPS_VT_##type2, L#name2)			\
				,RPS_FieldInfo(RPS_VT_##type3, L#name3)			\
				,RPS_FieldInfo(RPS_VT_##type4, L#name4)			\
				,RPS_FieldInfo(RPS_VT_##type5, L#name5)			\
				,RPS_FieldInfo(RPS_VT_##type6, L#name6));		\
		return o;												\
	}															\
	class_name() {											\
		zzFields[L#name1] = RPS_FieldData(RPS_VT_##type1);	\
		zzFields[L#name2] = RPS_FieldData(RPS_VT_##type2);	\
		zzFields[L#name3] = RPS_FieldData(RPS_VT_##type3);	\
		zzFields[L#name4] = RPS_FieldData(RPS_VT_##type4);	\
		zzFields[L#name5] = RPS_FieldData(RPS_VT_##type5);	\
		zzFields[L#name6] = RPS_FieldData(RPS_VT_##type6);	\
	}														\
	class_name(		RPS_FieldData name1##_, RPS_FieldData name2##_, RPS_FieldData name3##_		\
				,	RPS_FieldData name4##_, RPS_FieldData name5##_, RPS_FieldData name6##_) {	\
		zzFields[L#name1] = name1##_;															\
		zzFields[L#name2] = name2##_;															\
		zzFields[L#name3] = name3##_;															\
		zzFields[L#name4] = name4##_;															\
		zzFields[L#name5] = name5##_;															\
		zzFields[L#name6] = name6##_;															\
	}																							\
	class_name(	RPS_##type1##_ARG_CTYPE() name1##_, RPS_##type2##_ARG_CTYPE() name2##_, RPS_##type3##_ARG_CTYPE() name3##_ ,	\
				RPS_##type4##_ARG_CTYPE() name4##_, RPS_##type5##_ARG_CTYPE() name5##_, RPS_##type6##_ARG_CTYPE() name6##_ ) {	\
		zzFields[L#name1] = RPS_FieldData(name1##_);	\
		zzFields[L#name2] = RPS_FieldData(name2##_);	\
		zzFields[L#name3] = RPS_FieldData(name3##_);	\
		zzFields[L#name4] = RPS_FieldData(name4##_);	\
		zzFields[L#name5] = RPS_FieldData(name5##_);	\
		zzFields[L#name6] = RPS_FieldData(name6##_);	\
	}													\
	RPS_ENTITY_FIELD_DEFINE(type1,name1)	\
	RPS_ENTITY_FIELD_DEFINE(type2,name2)	\
	RPS_ENTITY_FIELD_DEFINE(type3,name3)	\
	RPS_ENTITY_FIELD_DEFINE(type4,name4)	\
	RPS_ENTITY_FIELD_DEFINE(type5,name5)	\
	RPS_ENTITY_FIELD_DEFINE(type6,name6)	\
	const RPS_FieldData& operator[](const std::wstring& key) const {	\
		if (key == L#name1) { return zzFields.at(L#name1); }	\
		if (key == L#name2) { return zzFields.at(L#name2); }	\
		if (key == L#name3) { return zzFields.at(L#name3); }	\
		if (key == L#name4) { return zzFields.at(L#name4); }	\
		if (key == L#name5) { return zzFields.at(L#name5); }	\
		if (key == L#name6) { return zzFields.at(L#name6); }	\
		return RPS_EMPTY_FIELD;									\
	}															\
	RPS_FieldData& operator[](const std::wstring& key) {	\
		if (key == L#name1) { return zzFields[L#name1]; }	\
		if (key == L#name2) { return zzFields[L#name2]; }	\
		if (key == L#name3) { return zzFields[L#name3]; }	\
		if (key == L#name4) { return zzFields[L#name4]; }	\
		if (key == L#name5) { return zzFields[L#name5]; }	\
		if (key == L#name6) { return zzFields[L#name6]; }	\
		throw 0;											\
	}														\
private:										\
	std::map<std::wstring, RPS_FieldData> zzFields;	\
};

//------------------------------------------------------
// create 7 fields entity
//------------------------------------------------------
#define RPS_ENTITY_CLASS7(class_name, table_name,	type1, name1, type2, name2,	type3, name3,	\
													type4, name4, type5, name5, type6, name6,	\
													type7, name7 )								\
class class_name {												\
public:															\
	RPS_TABLE_NAME_DEFINE(table_name)							\
	static const std::vector<RPS_FieldInfo> FIELD_INFO_LIST(){	\
		const static std::vector<RPS_FieldInfo> o				\
			= (GimLib::make_vector<RPS_FieldInfo>()				\
				,RPS_FieldInfo(RPS_VT_##type1, L#name1)			\
				,RPS_FieldInfo(RPS_VT_##type2, L#name2)			\
				,RPS_FieldInfo(RPS_VT_##type3, L#name3)			\
				,RPS_FieldInfo(RPS_VT_##type4, L#name4)			\
				,RPS_FieldInfo(RPS_VT_##type5, L#name5)			\
				,RPS_FieldInfo(RPS_VT_##type6, L#name6)			\
				,RPS_FieldInfo(RPS_VT_##type7, L#name7));		\
		return o;												\
	}															\
	class_name() {											\
		zzFields[L#name1] = RPS_FieldData(RPS_VT_##type1);	\
		zzFields[L#name2] = RPS_FieldData(RPS_VT_##type2);	\
		zzFields[L#name3] = RPS_FieldData(RPS_VT_##type3);	\
		zzFields[L#name4] = RPS_FieldData(RPS_VT_##type4);	\
		zzFields[L#name5] = RPS_FieldData(RPS_VT_##type5);	\
		zzFields[L#name6] = RPS_FieldData(RPS_VT_##type6);	\
		zzFields[L#name7] = RPS_FieldData(RPS_VT_##type7);	\
	}														\
	class_name(		RPS_FieldData name1##_, RPS_FieldData name2##_, RPS_FieldData name3##_		\
				,	RPS_FieldData name4##_, RPS_FieldData name5##_, RPS_FieldData name6##_		\
				,	RPS_FieldData name7##_													) {	\
		zzFields[L#name1] = name1##_;															\
		zzFields[L#name2] = name2##_;															\
		zzFields[L#name3] = name3##_;															\
		zzFields[L#name4] = name4##_;															\
		zzFields[L#name5] = name5##_;															\
		zzFields[L#name6] = name6##_;															\
		zzFields[L#name7] = name7##_;															\
	}																							\
	class_name(	RPS_##type1##_ARG_CTYPE() name1##_, RPS_##type2##_ARG_CTYPE() name2##_, RPS_##type3##_ARG_CTYPE() name3##_ ,	\
				RPS_##type4##_ARG_CTYPE() name4##_, RPS_##type5##_ARG_CTYPE() name5##_, RPS_##type6##_ARG_CTYPE() name6##_ ,	\
				RPS_##type7##_ARG_CTYPE() name7##_ ) {	\
		zzFields[L#name1] = RPS_FieldData(name1##_);	\
		zzFields[L#name2] = RPS_FieldData(name2##_);	\
		zzFields[L#name3] = RPS_FieldData(name3##_);	\
		zzFields[L#name4] = RPS_FieldData(name4##_);	\
		zzFields[L#name5] = RPS_FieldData(name5##_);	\
		zzFields[L#name6] = RPS_FieldData(name6##_);	\
		zzFields[L#name7] = RPS_FieldData(name7##_);	\
	}													\
	RPS_ENTITY_FIELD_DEFINE(type1,name1)	\
	RPS_ENTITY_FIELD_DEFINE(type2,name2)	\
	RPS_ENTITY_FIELD_DEFINE(type3,name3)	\
	RPS_ENTITY_FIELD_DEFINE(type4,name4)	\
	RPS_ENTITY_FIELD_DEFINE(type5,name5)	\
	RPS_ENTITY_FIELD_DEFINE(type6,name6)	\
	RPS_ENTITY_FIELD_DEFINE(type7,name7)	\
	const RPS_FieldData& operator[](const std::wstring& key) const {	\
		if (key == L#name1) { return zzFields.at(L#name1); }	\
		if (key == L#name2) { return zzFields.at(L#name2); }	\
		if (key == L#name3) { return zzFields.at(L#name3); }	\
		if (key == L#name4) { return zzFields.at(L#name4); }	\
		if (key == L#name5) { return zzFields.at(L#name5); }	\
		if (key == L#name6) { return zzFields.at(L#name6); }	\
		if (key == L#name7) { return zzFields.at(L#name7); }	\
		return RPS_EMPTY_FIELD;									\
	}															\
	RPS_FieldData& operator[](const std::wstring& key) {	\
		if (key == L#name1) { return zzFields[L#name1]; }	\
		if (key == L#name2) { return zzFields[L#name2]; }	\
		if (key == L#name3) { return zzFields[L#name3]; }	\
		if (key == L#name4) { return zzFields[L#name4]; }	\
		if (key == L#name5) { return zzFields[L#name5]; }	\
		if (key == L#name6) { return zzFields[L#name6]; }	\
		if (key == L#name7) { return zzFields[L#name7]; }	\
		throw 0;											\
	}														\
private:										\
	std::map<std::wstring, RPS_FieldData> zzFields;	\
};

//------------------------------------------------------
// create 8 fields entity
//------------------------------------------------------
#define RPS_ENTITY_CLASS8(class_name, table_name,	type1, name1, type2, name2,	type3, name3,	\
													type4, name4, type5, name5, type6, name6,	\
													type7, name7, type8, name8 )				\
class class_name {												\
public:															\
	RPS_TABLE_NAME_DEFINE(table_name)							\
	static const std::vector<RPS_FieldInfo> FIELD_INFO_LIST(){	\
		const static std::vector<RPS_FieldInfo> o				\
			= (GimLib::make_vector<RPS_FieldInfo>()				\
				,RPS_FieldInfo(RPS_VT_##type1, L#name1)			\
				,RPS_FieldInfo(RPS_VT_##type2, L#name2)			\
				,RPS_FieldInfo(RPS_VT_##type3, L#name3)			\
				,RPS_FieldInfo(RPS_VT_##type4, L#name4)			\
				,RPS_FieldInfo(RPS_VT_##type5, L#name5)			\
				,RPS_FieldInfo(RPS_VT_##type6, L#name6)			\
				,RPS_FieldInfo(RPS_VT_##type7, L#name7)			\
				,RPS_FieldInfo(RPS_VT_##type8, L#name8));		\
		return o;												\
	}															\
	class_name() {											\
		zzFields[L#name1] = RPS_FieldData(RPS_VT_##type1);	\
		zzFields[L#name2] = RPS_FieldData(RPS_VT_##type2);	\
		zzFields[L#name3] = RPS_FieldData(RPS_VT_##type3);	\
		zzFields[L#name4] = RPS_FieldData(RPS_VT_##type4);	\
		zzFields[L#name5] = RPS_FieldData(RPS_VT_##type5);	\
		zzFields[L#name6] = RPS_FieldData(RPS_VT_##type6);	\
		zzFields[L#name7] = RPS_FieldData(RPS_VT_##type7);	\
		zzFields[L#name8] = RPS_FieldData(RPS_VT_##type8);	\
	}														\
	class_name(		RPS_FieldData name1##_, RPS_FieldData name2##_, RPS_FieldData name3##_		\
				,	RPS_FieldData name4##_, RPS_FieldData name5##_, RPS_FieldData name6##_		\
				,	RPS_FieldData name7##_, RPS_FieldData name8##_							) {	\
		zzFields[L#name1] = name1##_;															\
		zzFields[L#name2] = name2##_;															\
		zzFields[L#name3] = name3##_;															\
		zzFields[L#name4] = name4##_;															\
		zzFields[L#name5] = name5##_;															\
		zzFields[L#name6] = name6##_;															\
		zzFields[L#name7] = name7##_;															\
		zzFields[L#name8] = name8##_;															\
	}																							\
	class_name(	RPS_##type1##_ARG_CTYPE() name1##_, RPS_##type2##_ARG_CTYPE() name2##_, RPS_##type3##_ARG_CTYPE() name3##_ ,	\
				RPS_##type4##_ARG_CTYPE() name4##_, RPS_##type5##_ARG_CTYPE() name5##_, RPS_##type6##_ARG_CTYPE() name6##_ ,	\
				RPS_##type7##_ARG_CTYPE() name7##_, RPS_##type8##_ARG_CTYPE() name8##_										) {	\
		zzFields[L#name1] = RPS_FieldData(name1##_);	\
		zzFields[L#name2] = RPS_FieldData(name2##_);	\
		zzFields[L#name3] = RPS_FieldData(name3##_);	\
		zzFields[L#name4] = RPS_FieldData(name4##_);	\
		zzFields[L#name5] = RPS_FieldData(name5##_);	\
		zzFields[L#name6] = RPS_FieldData(name6##_);	\
		zzFields[L#name7] = RPS_FieldData(name7##_);	\
		zzFields[L#name8] = RPS_FieldData(name8##_);	\
	}													\
	RPS_ENTITY_FIELD_DEFINE(type1,name1)	\
	RPS_ENTITY_FIELD_DEFINE(type2,name2)	\
	RPS_ENTITY_FIELD_DEFINE(type3,name3)	\
	RPS_ENTITY_FIELD_DEFINE(type4,name4)	\
	RPS_ENTITY_FIELD_DEFINE(type5,name5)	\
	RPS_ENTITY_FIELD_DEFINE(type6,name6)	\
	RPS_ENTITY_FIELD_DEFINE(type7,name7)	\
	RPS_ENTITY_FIELD_DEFINE(type8,name8)	\
	const RPS_FieldData& operator[](const std::wstring& key) const {	\
		if (key == L#name1) { return zzFields.at(L#name1); }	\
		if (key == L#name2) { return zzFields.at(L#name2); }	\
		if (key == L#name3) { return zzFields.at(L#name3); }	\
		if (key == L#name4) { return zzFields.at(L#name4); }	\
		if (key == L#name5) { return zzFields.at(L#name5); }	\
		if (key == L#name6) { return zzFields.at(L#name6); }	\
		if (key == L#name7) { return zzFields.at(L#name7); }	\
		if (key == L#name8) { return zzFields.at(L#name8); }	\
		return RPS_EMPTY_FIELD;									\
	}															\
	RPS_FieldData& operator[](const std::wstring& key) {	\
		if (key == L#name1) { return zzFields[L#name1]; }	\
		if (key == L#name2) { return zzFields[L#name2]; }	\
		if (key == L#name3) { return zzFields[L#name3]; }	\
		if (key == L#name4) { return zzFields[L#name4]; }	\
		if (key == L#name5) { return zzFields[L#name5]; }	\
		if (key == L#name6) { return zzFields[L#name6]; }	\
		if (key == L#name7) { return zzFields[L#name7]; }	\
		if (key == L#name8) { return zzFields[L#name8]; }	\
		throw 0;											\
	}														\
private:										\
	std::map<std::wstring, RPS_FieldData> zzFields;	\
};

#endif

