#pragma once

#include <memory>
#include "include/Repository.h"

#define INITGUID		// ADO���`���邽�߂̒萔(GUID)�̏�����
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" rename_namespace("ADOCG") rename("EOF", "EndOfFile")
using namespace ADOCG;	// ���O���
#include "icrsint.h"	// ADO���g���Ď擾�����t�B�[���h�̃f�[�^��ϊ�����}�N���Ȃǂ���`���ꂽ�w�b�_�[


//ADO�p�̏������\�z�N���X
//	�T�v
//		IWhereCond����ADO��SQL���̂��߂�WHERE�ߕ�����𐶐����܂��B
//		WHERE�ߕ�����́AIWhereCond::AcceptCondBuilder�ɓn���ƁAQueryString�Ƃ��Ď��o���܂��B
// 	�t�B�[���h
//		QueryString
class AdoCondBuilder : public GimLib::CondBuilder
{
public:
	std::wstring QueryString;
	virtual void visit(const GimLib::LeafCond*	cond) override;
	virtual void visit(const GimLib::AndCond*	cond) override;
	virtual void visit(const GimLib::OrCond*	cond) override;
};

//RepositoryADO�̎�����������N���X
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

	//variant��NULL�^��\������萔
	static const _variant_t VAR_NULL;

	bool FindAllRecordset	(const std::shared_ptr<GimLib::IWhereCond>& cond, _RecordsetPtr& outRecordset);
	bool FindFirstRecordset	(const std::shared_ptr<GimLib::IWhereCond>& cond, _RecordsetPtr& outRecordset);
	bool DeleteImpl(const std::shared_ptr<GimLib::IWhereCond>& cond);
	bool InsertImpl(const std::vector<GimLib::RPS_FieldInfo>& fieldInfoList, SAFEARRAY* valueArray);
	bool UpdateImpl(const std::wstring& updateFields, const std::shared_ptr<GimLib::IWhereCond>& cond);

	//�����ɍ������R�[�h���J�������R�[�h�Z�b�g���擾
	bool zzFindRecordset(const std::shared_ptr<GimLib::IWhereCond>& cond, bool isOnlyFirst, _RecordsetPtr& outRecordset);

	//�t�B�[���h�̒l��SQL���Ŏg�����߂̌`���ɂ���i��̓I�ɂ͕�����Ȃ�V���O���N�H�[�g�ň͂ށj
	static std::wstring MakeFieldValueForSQL(const GimLib::RPS_FieldData& field);
	//�����߂���SELECT�����쐬
	static std::wstring MakeSelectSQL(const std::shared_ptr<GimLib::IWhereCond>& cond, const std::wstring& tblName, bool isOnlyFirst);
	// �����߃I�u�W�F�N�g����WHERE�߂̕�������쐬
	static std::wstring MakeWhereSection(const std::shared_ptr<GimLib::IWhereCond>& cond);
};


//ADO�Ńf�[�^�x�[�X�A�N�Z�X
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
	//�S���R�[�h�̎擾
	virtual std::vector<ENTITY> FindAll() override
	{
		return FindAll(std::shared_ptr<GimLib::IWhereCond>());
	}
	//�����ɍ������R�[�h��S�擾
	virtual std::vector<ENTITY> FindAll(const std::shared_ptr<GimLib::IWhereCond>& cond) override
	{
		DbOpener opener(zzImpl); //�ڑ����I�[�v��(�X�R�[�v���z�����玩���N���[�Y)
		// ���R�[�h�Z�b�g�̎擾
		_RecordsetPtr recordset(__uuidof(Recordset));
		if (!zzImpl.FindAllRecordset(cond, recordset)) {
			recordset->Close();
			recordset = NULL;
			return std::vector<ENTITY>(); //������Ȃ�����
		}

		// �擪���R�[�h���疖���܂��r�߂�
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

	//�����ɍ������R�[�h���P���擾
	virtual std::shared_ptr<ENTITY> FindFirst(const std::shared_ptr<GimLib::IWhereCond>& cond) override
	{
		DbOpener opener(zzImpl); //�ڑ����I�[�v��(�X�R�[�v���z�����玩���N���[�Y)
		// ���R�[�h�Z�b�g�̎擾
		_RecordsetPtr recordset(__uuidof(Recordset));
		if (!zzImpl.FindFirstRecordset(cond, recordset)) {
			recordset->Close();
			recordset = NULL;
			return std::shared_ptr<ENTITY>(); //������Ȃ�����
		}

		//�f�[�^�̎��o��
		auto ret = std::shared_ptr<ENTITY>(new ENTITY());
		const auto fieldsInfo = ENTITY::FIELD_INFO_LIST();
		ExtractRecordset(recordset, fieldsInfo, *ret);
		recordset->Close();
		recordset = NULL;
		return ret;
	}

	//�}��
	virtual bool Insert(const ENTITY& entity) override
	{
		DbOpener opener(zzImpl); //�ڑ����I�[�v��(�X�R�[�v���z�����玩���N���[�Y)
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
 
		//���s
		return zzImpl.InsertImpl(ENTITY::FIELD_INFO_LIST(), psaValues);
	}

	//�X�V
	virtual bool Update(const ENTITY& entity, const std::shared_ptr<GimLib::IWhereCond>& cond) override
	{
		DbOpener opener(zzImpl); //�ڑ����I�[�v��(�X�R�[�v���z�����玩���N���[�Y)
		//UPDATE����t�B�[���h�̃Z�b�g
		std::wstring updateFields;
		bool isFirstField = true;
		const auto fieldsInfo = ENTITY::FIELD_INFO_LIST();
		for(auto itr = fieldsInfo.begin(); itr != fieldsInfo.end(); ++itr)
		{
			const GimLib::RPS_FieldData& field = entity[itr->Name];
			if (!field.HasData) { continue; } //�f�[�^�̖������ڂ͔�΂�
			if (!isFirstField) {
				updateFields += L", ";
			}
			updateFields += itr->Name;
			updateFields += L" = ";
			updateFields += RepositoryADOImpl::MakeFieldValueForSQL(field);
			isFirstField = false;
		}

		//���s
		return zzImpl.UpdateImpl(updateFields, cond);
	}

	//�폜
	virtual bool Delete(const std::shared_ptr<GimLib::IWhereCond>& cond) override
	{
		DbOpener opener(zzImpl); //�ڑ����I�[�v��(�X�R�[�v���z�����玩���N���[�Y)
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

