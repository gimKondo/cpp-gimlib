#include "stdafx.h"

#define ZLIB_MODULE

#include "RepositoryADO.h"

using namespace GimLib;

//----------------------------------------
// AdoCondBuilderクラス
//----------------------------------------
void AdoCondBuilder::visit(const LeafCond* cond)
{
	std::wstring condTxt = cond->Key;
	switch(cond->Cmpr)
	{
	case COND_EQ : condTxt += L" = " ; break;
	case COND_NEQ: condTxt += L" <> "; break;
	case COND_GT : condTxt += L" > " ; break;
	case COND_GTE: condTxt += L" >= "; break;
	case COND_LT : condTxt += L" < " ; break;
	case COND_LTE: condTxt += L" <= "; break;
	default		 : condTxt += L" = " ; break;//とりあえず 
	}
	if (cond->Value.ValueType == RPS_VT_STRING) {
		condTxt += L"'";
		condTxt += cond->Value.ToString();
		condTxt += L"'";
	} else {
		condTxt += cond->Value.ToString();
	}
	QueryString += condTxt;
}

void AdoCondBuilder::visit(const AndCond* cond)
{
	QueryString += L"(";
	cond->Cond1->acceptCondBuilder(*this);
	QueryString += L" AND ";
	cond->Cond2->acceptCondBuilder(*this);
	QueryString += L")";
}

void AdoCondBuilder::visit(const OrCond* cond)
{
	QueryString += L"(";
	cond->Cond1->acceptCondBuilder(*this);
	QueryString += L" OR ";
	cond->Cond2->acceptCondBuilder(*this);
	QueryString += L")";
}


//----------------------------------------
// RepositoryADOImplクラス
//----------------------------------------
//内部クラスDbOpener
RepositoryADOImpl::DbOpener::DbOpener(RepositoryADOImpl& owner)
	: zzOwner(owner)
{
	// ADOオブジェクトの生成
	zzOwner.zzConnect->Open( owner.zzConnString.c_str(), L"", L"", adOpenUnspecified);
}
RepositoryADOImpl::DbOpener::~DbOpener()
{
	zzOwner.zzConnect->Close();
}

//コンストラクタ：接続をオープン
RepositoryADOImpl::RepositoryADOImpl(const std::wstring& dbPath, const std::wstring& tblName)
	: zzConnect(NULL)
	, zzTableName(tblName)
	, zzConnString(L"Driver={Microsoft Access Driver (*.mdb, *.accdb)}; DBQ=" + dbPath + L";")
{
	zzConnect.CreateInstance(__uuidof(Connection));
}

//デストラクタ：接続をクローズ
RepositoryADOImpl::~RepositoryADOImpl()
{
	zzConnect = NULL;
}

//variantのNULL型を表現する定数
const _variant_t RepositoryADOImpl::VAR_NULL = []()->_variant_t {
		_variant_t vNull;
		vNull.vt = VT_ERROR;
		vNull.scode = DISP_E_PARAMNOTFOUND;
		return vNull;
	}();

//条件に合う全てのレコードを開いたレコードセットを取得
bool RepositoryADOImpl::FindAllRecordset(const std::shared_ptr<IWhereCond>& cond, _RecordsetPtr& outRecordset)
{
	//レコードセットの取得
	if (!zzFindRecordset(cond, false, outRecordset)) {
		return false;
	}
	return true;
}

//条件に合う最初のレコードを開いたレコードセットを取得
bool RepositoryADOImpl::FindFirstRecordset(const std::shared_ptr<IWhereCond>& cond, _RecordsetPtr& outRecordset)
{
	//レコードセットの取得
	if (!zzFindRecordset(cond, true, outRecordset)) {
		return false;
	}
	//最初のレコードに戻して返す
	outRecordset->MoveFirst();
	return true;
}

//DELETEの実処理
bool RepositoryADOImpl::DeleteImpl(const std::shared_ptr<IWhereCond>& cond)
{
    _RecordsetPtr recordset(__uuidof(Recordset));
	_CommandPtr command(__uuidof(Command));

	command->ActiveConnection = zzConnect;

	//DELETE文生成
	std::wstring cmdTxt = L"DELETE FROM ";
	cmdTxt += zzTableName;

	//WHERE節をセット
	cmdTxt += MakeWhereSection(cond);

	//実行
	command->CommandText = cmdTxt.c_str();
	recordset->PutRefSource(command);
	recordset->Open(VAR_NULL, VAR_NULL, adOpenDynamic, adLockOptimistic, adCmdUnknown);

	recordset->Close();
	recordset = NULL;

	return true;
}

//INSERTの実処理
bool RepositoryADOImpl::InsertImpl(const std::vector<RPS_FieldInfo>& fieldInfoList, SAFEARRAY* valueArray)
{
	// フィールド名を入れるための配列を静止絵
   	SAFEARRAY * psaFields; 
   	SAFEARRAYBOUND aDimFields[1]; 
   	aDimFields[0].lLbound = 0; 
	aDimFields[0].cElements = static_cast<int>(fieldInfoList.size());
	psaFields = SafeArrayCreate(VT_VARIANT, 1, aDimFields); 

	long ix[1];
	_variant_t var;
	int fieldCnt = 0;
	for(auto itr = fieldInfoList.begin(); itr != fieldInfoList.end(); ++itr)
	{
		ix[0] = fieldCnt;
		var = itr->Name.c_str();
		// フィールド名のセット
		SafeArrayPutElement(psaFields, ix, (void*) (VARIANT *) (&var));
		++fieldCnt;
	}

	// レコードセットのオープン
	_RecordsetPtr recordset(__uuidof(Recordset));
	recordset->Open(zzTableName.c_str(), _variant_t((IDispatch *) zzConnect, true), adOpenKeyset, adLockOptimistic, adCmdTable);
 
	// Define VARIANTS that are SafeArrays of VARIANTS.
	_variant_t vtFields, vtValues;
	vtFields.vt = VT_ARRAY | VT_VARIANT;
	vtValues.vt = VT_ARRAY | VT_VARIANT;
	vtFields.parray = psaFields;
	vtValues.parray = valueArray;

	try {
		// レコードの追加
		recordset->AddNew(vtFields, vtValues);
	}
	catch (_com_error& e) {
		//追加に失敗（だいたいは既にあるレコードとの主キー重複）
		e; assert(0);
		return false;
	}
		
	recordset->Close();
	recordset = NULL;

	return true;
}

//UPDATEの実処理
bool RepositoryADOImpl::UpdateImpl(const std::wstring& updateFields, const std::shared_ptr<IWhereCond>& cond)
{
	//SQL文の作成
	std::wstring cmdTxt = L"UPDATE ";
	cmdTxt += zzTableName;
	cmdTxt += L" SET ";
	//UPDATEするフィールドのセット
	cmdTxt += updateFields;
	//WHERE節をセット
	cmdTxt += MakeWhereSection(cond);

	//実行
	_RecordsetPtr recordset(__uuidof(Recordset));
	_CommandPtr command(__uuidof(Command));
	command->ActiveConnection = zzConnect;
	command->CommandText = cmdTxt.c_str();
	recordset->PutRefSource(command);
	recordset->Open(VAR_NULL, VAR_NULL, adOpenDynamic, adLockOptimistic, adCmdUnknown);

	recordset->Close();
	recordset = NULL;

	return true;
}

//条件に合うレコードを開いたレコードセットを取得
bool RepositoryADOImpl::zzFindRecordset(const std::shared_ptr<IWhereCond>& cond, bool isOnlyFirst, _RecordsetPtr& outRecordset)
{
	_CommandPtr command(__uuidof(Command));

	//SQLの設定
	command->ActiveConnection = zzConnect;
	std::wstring cmdText = MakeSelectSQL(cond, zzTableName, isOnlyFirst);
	command->CommandText = cmdText.c_str();

	// レコードセットの取得
	try
	{
		outRecordset->PutRefSource(command);
		outRecordset->Open(VAR_NULL, VAR_NULL, adOpenDynamic, adLockOptimistic, adCmdUnknown);
	}
	catch(_com_error &e)
	{
		e; assert(0);
		return false;
	}

	//開いたレコードセットをチェック
	if (outRecordset->GetEndOfFile()) {
		return false; //見つからなかった
	}

	return true;
}

//フィールドの値をSQL文で使うための形式にする（具体的には文字列ならシングルクォートで囲む）
std::wstring RepositoryADOImpl::MakeFieldValueForSQL(const RPS_FieldData& field)
{
	if (field.ValueType == RPS_VT_STRING) {
		std::wstring ret = L"'";
		ret += field.StringData;
		ret += L"'";
		return ret;
	}
	return field.ToString();
}

//条件節からSELECT文を作成
std::wstring RepositoryADOImpl::MakeSelectSQL(const std::shared_ptr<IWhereCond>& cond, const std::wstring& tblName, bool isOnlyFirst)
{
	std::wstring cmdTxt;
	if (isOnlyFirst) {
		cmdTxt += L"SELECT TOP 1 * FROM ";
	} else {
		cmdTxt += L"SELECT * FROM ";
	}
	cmdTxt += tblName;
	if (cond)
	{
		cmdTxt += MakeWhereSection(cond);
	}
	return cmdTxt;
}

// 条件節オブジェクトからWHERE節の文字列を作成
std::wstring RepositoryADOImpl::MakeWhereSection(const std::shared_ptr<IWhereCond>& cond)
{
	AdoCondBuilder builder;
	cond->acceptCondBuilder(builder);
	std::wstring condTxt = L" WHERE ";
	condTxt += builder.QueryString;
	return condTxt;
}

