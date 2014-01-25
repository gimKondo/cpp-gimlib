#include "stdafx.h"

#define ZLIB_MODULE

#include "RepositoryADO.h"

using namespace GimLib;

//----------------------------------------
// AdoCondBuilder�N���X
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
	default		 : condTxt += L" = " ; break;//�Ƃ肠���� 
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
// RepositoryADOImpl�N���X
//----------------------------------------
//�����N���XDbOpener
RepositoryADOImpl::DbOpener::DbOpener(RepositoryADOImpl& owner)
	: zzOwner(owner)
{
	// ADO�I�u�W�F�N�g�̐���
	zzOwner.zzConnect->Open( owner.zzConnString.c_str(), L"", L"", adOpenUnspecified);
}
RepositoryADOImpl::DbOpener::~DbOpener()
{
	zzOwner.zzConnect->Close();
}

//�R���X�g���N�^�F�ڑ����I�[�v��
RepositoryADOImpl::RepositoryADOImpl(const std::wstring& dbPath, const std::wstring& tblName)
	: zzConnect(NULL)
	, zzTableName(tblName)
	, zzConnString(L"Driver={Microsoft Access Driver (*.mdb, *.accdb)}; DBQ=" + dbPath + L";")
{
	zzConnect.CreateInstance(__uuidof(Connection));
}

//�f�X�g���N�^�F�ڑ����N���[�Y
RepositoryADOImpl::~RepositoryADOImpl()
{
	zzConnect = NULL;
}

//variant��NULL�^��\������萔
const _variant_t RepositoryADOImpl::VAR_NULL = []()->_variant_t {
		_variant_t vNull;
		vNull.vt = VT_ERROR;
		vNull.scode = DISP_E_PARAMNOTFOUND;
		return vNull;
	}();

//�����ɍ����S�Ẵ��R�[�h���J�������R�[�h�Z�b�g���擾
bool RepositoryADOImpl::FindAllRecordset(const std::shared_ptr<IWhereCond>& cond, _RecordsetPtr& outRecordset)
{
	//���R�[�h�Z�b�g�̎擾
	if (!zzFindRecordset(cond, false, outRecordset)) {
		return false;
	}
	return true;
}

//�����ɍ����ŏ��̃��R�[�h���J�������R�[�h�Z�b�g���擾
bool RepositoryADOImpl::FindFirstRecordset(const std::shared_ptr<IWhereCond>& cond, _RecordsetPtr& outRecordset)
{
	//���R�[�h�Z�b�g�̎擾
	if (!zzFindRecordset(cond, true, outRecordset)) {
		return false;
	}
	//�ŏ��̃��R�[�h�ɖ߂��ĕԂ�
	outRecordset->MoveFirst();
	return true;
}

//DELETE�̎�����
bool RepositoryADOImpl::DeleteImpl(const std::shared_ptr<IWhereCond>& cond)
{
    _RecordsetPtr recordset(__uuidof(Recordset));
	_CommandPtr command(__uuidof(Command));

	command->ActiveConnection = zzConnect;

	//DELETE������
	std::wstring cmdTxt = L"DELETE FROM ";
	cmdTxt += zzTableName;

	//WHERE�߂��Z�b�g
	cmdTxt += MakeWhereSection(cond);

	//���s
	command->CommandText = cmdTxt.c_str();
	recordset->PutRefSource(command);
	recordset->Open(VAR_NULL, VAR_NULL, adOpenDynamic, adLockOptimistic, adCmdUnknown);

	recordset->Close();
	recordset = NULL;

	return true;
}

//INSERT�̎�����
bool RepositoryADOImpl::InsertImpl(const std::vector<RPS_FieldInfo>& fieldInfoList, SAFEARRAY* valueArray)
{
	// �t�B�[���h�������邽�߂̔z���Î~�G
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
		// �t�B�[���h���̃Z�b�g
		SafeArrayPutElement(psaFields, ix, (void*) (VARIANT *) (&var));
		++fieldCnt;
	}

	// ���R�[�h�Z�b�g�̃I�[�v��
	_RecordsetPtr recordset(__uuidof(Recordset));
	recordset->Open(zzTableName.c_str(), _variant_t((IDispatch *) zzConnect, true), adOpenKeyset, adLockOptimistic, adCmdTable);
 
	// Define VARIANTS that are SafeArrays of VARIANTS.
	_variant_t vtFields, vtValues;
	vtFields.vt = VT_ARRAY | VT_VARIANT;
	vtValues.vt = VT_ARRAY | VT_VARIANT;
	vtFields.parray = psaFields;
	vtValues.parray = valueArray;

	try {
		// ���R�[�h�̒ǉ�
		recordset->AddNew(vtFields, vtValues);
	}
	catch (_com_error& e) {
		//�ǉ��Ɏ��s�i���������͊��ɂ��郌�R�[�h�Ƃ̎�L�[�d���j
		e; assert(0);
		return false;
	}
		
	recordset->Close();
	recordset = NULL;

	return true;
}

//UPDATE�̎�����
bool RepositoryADOImpl::UpdateImpl(const std::wstring& updateFields, const std::shared_ptr<IWhereCond>& cond)
{
	//SQL���̍쐬
	std::wstring cmdTxt = L"UPDATE ";
	cmdTxt += zzTableName;
	cmdTxt += L" SET ";
	//UPDATE����t�B�[���h�̃Z�b�g
	cmdTxt += updateFields;
	//WHERE�߂��Z�b�g
	cmdTxt += MakeWhereSection(cond);

	//���s
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

//�����ɍ������R�[�h���J�������R�[�h�Z�b�g���擾
bool RepositoryADOImpl::zzFindRecordset(const std::shared_ptr<IWhereCond>& cond, bool isOnlyFirst, _RecordsetPtr& outRecordset)
{
	_CommandPtr command(__uuidof(Command));

	//SQL�̐ݒ�
	command->ActiveConnection = zzConnect;
	std::wstring cmdText = MakeSelectSQL(cond, zzTableName, isOnlyFirst);
	command->CommandText = cmdText.c_str();

	// ���R�[�h�Z�b�g�̎擾
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

	//�J�������R�[�h�Z�b�g���`�F�b�N
	if (outRecordset->GetEndOfFile()) {
		return false; //������Ȃ�����
	}

	return true;
}

//�t�B�[���h�̒l��SQL���Ŏg�����߂̌`���ɂ���i��̓I�ɂ͕�����Ȃ�V���O���N�H�[�g�ň͂ށj
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

//�����߂���SELECT�����쐬
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

// �����߃I�u�W�F�N�g����WHERE�߂̕�������쐬
std::wstring RepositoryADOImpl::MakeWhereSection(const std::shared_ptr<IWhereCond>& cond)
{
	AdoCondBuilder builder;
	cond->acceptCondBuilder(builder);
	std::wstring condTxt = L" WHERE ";
	condTxt += builder.QueryString;
	return condTxt;
}

