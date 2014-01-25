#pragma once

#include <memory>
#include "include/Repository.h"

#define INITGUID		// ADOを定義するための定数(GUID)の初期化
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" rename_namespace("ADOCG") rename("EOF", "EndOfFile")
using namespace ADOCG;	// 名前空間
#include "icrsint.h"	// ADOを使って取得したフィールドのデータを変換するマクロなどが定義されたヘッダー


//ADO用の条件文構築クラス
//	概要
//		IWhereCondからADOのSQL文のためのWHERE節文字列を生成します。
//		WHERE節文字列は、IWhereCond::AcceptCondBuilderに渡すと、QueryStringとして取り出せます。
// 	フィールド
//		QueryString
class AdoCondBuilder : public GimLib::CondBuilder
{
public:
	std::wstring QueryString;
	virtual void visit(const GimLib::LeafCond*	cond) override;
	virtual void visit(const GimLib::AndCond*	cond) override;
	virtual void visit(const GimLib::OrCond*	cond) override;
};

//RepositoryADOの実処理をするクラス
class RepositoryADOImpl
{
	template <typename ENTITY> friend class RepositoryADO;
private:
	RepositoryADOImpl(const std::wstring& dbPath, const std::wstring& tblName);
public:
	~RepositoryADOImpl();
	class DbOpener
	{
		RepositoryADOImpl& zzOwner;
		//_ConnectionPtr	zzConnect;
	public:
		DbOpener(RepositoryADOImpl& owner);
		~DbOpener();
	};
private:
	_ConnectionPtr	zzConnect;
	const std::wstring	zzTableName;
	const std::wstring	zzConnString;

	//variantのNULL型を表現する定数
	static const _variant_t VAR_NULL;

	bool FindAllRecordset	(const std::shared_ptr<GimLib::IWhereCond>& cond, _RecordsetPtr& outRecordset);
	bool FindFirstRecordset	(const std::shared_ptr<GimLib::IWhereCond>& cond, _RecordsetPtr& outRecordset);
	bool DeleteImpl(const std::shared_ptr<GimLib::IWhereCond>& cond);
	bool InsertImpl(const std::vector<GimLib::RPS_FieldInfo>& fieldInfoList, SAFEARRAY* valueArray);
	bool UpdateImpl(const std::wstring& updateFields, const std::shared_ptr<GimLib::IWhereCond>& cond);

	//条件に合うレコードを開いたレコードセットを取得
	bool zzFindRecordset(const std::shared_ptr<GimLib::IWhereCond>& cond, bool isOnlyFirst, _RecordsetPtr& outRecordset);

	//フィールドの値をSQL文で使うための形式にする（具体的には文字列ならシングルクォートで囲む）
	static std::wstring MakeFieldValueForSQL(const GimLib::RPS_FieldData& field);
	//条件節からSELECT文を作成
	static std::wstring MakeSelectSQL(const std::shared_ptr<GimLib::IWhereCond>& cond, const std::wstring& tblName, bool isOnlyFirst);
	// 条件節オブジェクトからWHERE節の文字列を作成
	static std::wstring MakeWhereSection(const std::shared_ptr<GimLib::IWhereCond>& cond);
};


//ADOでデータベースアクセス
template <typename ENTITY> class RepositoryADO : public GimLib::IRepository<ENTITY>
{
private:
	RepositoryADOImpl zzImpl;
	std::wstring zzDbPath;
	typedef RepositoryADOImpl::DbOpener DbOpener;
public:
	RepositoryADO(const std::wstring& dbPath)
		: zzDbPath(dbPath), zzImpl(dbPath, ENTITY::TABLE_NAME())
	{
	}
public:
	//全レコードの取得
	virtual std::vector<ENTITY> FindAll() override
	{
		return FindAll(std::shared_ptr<GimLib::IWhereCond>());
	}
	//条件に合うレコードを全取得
	virtual std::vector<ENTITY> FindAll(const std::shared_ptr<GimLib::IWhereCond>& cond) override
	{
		DbOpener opener(zzImpl); //接続をオープン(スコープを越えたら自動クローズ)
		// レコードセットの取得
		_RecordsetPtr recordset(__uuidof(Recordset));
		if (!zzImpl.FindAllRecordset(cond, recordset)) {
			recordset->Close();
			recordset = NULL;
			return std::vector<ENTITY>(); //見つからなかった
		}

		// 先頭レコードから末尾まで舐める
		std::vector<ENTITY> ret;
		for(recordset->MoveFirst(); !recordset->GetEndOfFile(); recordset->MoveNext())
		{
			ENTITY entity;
			const auto fieldsInfo = ENTITY::FIELD_INFO_LIST();
			ExtractRecordset(recordset, fieldsInfo, entity);
			ret.push_back(entity);
		}
		recordset->Close();
		recordset = NULL;
		return ret;
	}

	//条件に合うレコードを１件取得
	virtual std::shared_ptr<ENTITY> FindFirst(const std::shared_ptr<GimLib::IWhereCond>& cond) override
	{
		DbOpener opener(zzImpl); //接続をオープン(スコープを越えたら自動クローズ)
		// レコードセットの取得
		_RecordsetPtr recordset(__uuidof(Recordset));
		if (!zzImpl.FindFirstRecordset(cond, recordset)) {
			recordset->Close();
			recordset = NULL;
			return std::shared_ptr<ENTITY>(); //見つからなかった
		}

		//データの取り出し
		auto ret = std::shared_ptr<ENTITY>(new ENTITY());
		const auto fieldsInfo = ENTITY::FIELD_INFO_LIST();
		ExtractRecordset(recordset, fieldsInfo, *ret);
		recordset->Close();
		recordset = NULL;
		return ret;
	}

	//挿入
	virtual bool Insert(const ENTITY& entity) override
	{
		DbOpener opener(zzImpl); //接続をオープン(スコープを越えたら自動クローズ)
		// Create a SafeArray for the field values.
		SAFEARRAY * psaValues;
		SAFEARRAYBOUND aDimValues[1];
		aDimValues[0].lLbound = 0;
		aDimValues[0].cElements = static_cast<int>(ENTITY::FIELD_INFO_LIST().size());
		psaValues = SafeArrayCreate(VT_VARIANT, 1, aDimValues);

		long ix[1];
		_variant_t var;
		int valCnt = 0;
		const auto fieldsInfo = ENTITY::FIELD_INFO_LIST();
		for(auto itr = fieldsInfo.begin(); itr != fieldsInfo.end(); ++itr)
		{
			ix[0] = valCnt;
			if (itr->ValueType == RPS_VT_INT) {
				var = entity[itr->Name].IntData;
			}
			else if (itr->ValueType == RPS_VT_DOUBLE) {
				var = entity[itr->Name].DoubleData;
			}
			else if (itr->ValueType == RPS_VT_STRING) {
				var = entity[itr->Name].StringData.GetString();
			}
			SafeArrayPutElement(psaValues, ix, (void*)(VARIANT *) &var);
			++valCnt;
		}
 
		//実行
		return zzImpl.InsertImpl(ENTITY::FIELD_INFO_LIST(), psaValues);
	}

	//更新
	virtual bool Update(const ENTITY& entity, const std::shared_ptr<GimLib::IWhereCond>& cond) override
	{
		DbOpener opener(zzImpl); //接続をオープン(スコープを越えたら自動クローズ)
		//UPDATEするフィールドのセット
		std::wstring updateFields;
		bool isFirstField = true;
		const auto fieldsInfo = ENTITY::FIELD_INFO_LIST();
		for(auto itr = fieldsInfo.begin(); itr != fieldsInfo.end(); ++itr)
		{
			const GimLib::RPS_FieldData& field = entity[itr->Name];
			if (!field.HasData) { continue; } //データの無い項目は飛ばす
			if (!isFirstField) {
				updateFields += L", ";
			}
			updateFields += itr->Name;
			updateFields += L" = ";
			updateFields += RepositoryADOImpl::MakeFieldValueForSQL(field);
			isFirstField = false;
		}

		//実行
		return zzImpl.UpdateImpl(updateFields, cond);
	}

	//削除
	virtual bool Delete(const std::shared_ptr<GimLib::IWhereCond>& cond) override
	{
		DbOpener opener(zzImpl); //接続をオープン(スコープを越えたら自動クローズ)
		return zzImpl.DeleteImpl(cond);
	}

private:
	void ExtractRecordset(const _RecordsetPtr& recordset, const std::vector<GimLib::RPS_FieldInfo>& fieldsInfo, ENTITY& outEntity)
	{
		for (auto field = fieldsInfo.cbegin(); field != fieldsInfo.cend(); ++field)
		{
			_variant_t v;
			v = recordset->GetCollect(_variant_t(field->Name.GetString()));
			switch(field->ValueType)
			{
			case RPS_VT_INT:
				if (v.vt == VT_NULL) {
					outEntity[field->Name].IntData = 0;
				} else {
					v.ChangeType(VT_I4);
					outEntity[field->Name].IntData = v.iVal;
				}
				break;
			case RPS_VT_DOUBLE:
				if (v.vt == VT_NULL) {
					outEntity[field->Name].DoubleData = 0.0;
				} else {
					v.ChangeType(VT_R8);
					outEntity[field->Name].DoubleData = v.dblVal;
				}
				break;
			case RPS_VT_STRING:
				if (v.vt == VT_NULL) {
					outEntity[field->Name].StringData = L"";
				} else {
					v.ChangeType(VT_BSTR);
					outEntity[field->Name].StringData = v.bstrVal;
				}
				break;
			}
		}
	}
};

