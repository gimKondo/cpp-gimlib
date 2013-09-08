#ifndef INCLUDED_GIMLIB_TEST_REPOSITORY_MOCK
#define INCLUDED_GIMLIB_TEST_REPOSITORY_MOCK

#include "include/Repository.h"

namespace GimLib {

//--------------------------------
// テスト用のモックリポジトリ
//--------------------------------
template <typename ENTITY> class MockRepository : public IRepository<ENTITY>
{
private:
	class MockCondBuilder : public CondBuilder
	{
	private:
		static bool RetTrue(const ENTITY&) { return true; }
		// クエリに応じてフィールド比較するファンクタ
		struct FieldCompare
		{
			FieldCompare(const LeafCond* cond)
				: zzCond(cond)
			{}
			const LeafCond* zzCond;
			bool operator()(const ENTITY& e)
			{
				const auto& field = e[zzCond->Key];
				//文字列データ比較
				if (field.ValueType == RPS_VT_STRING)
				{
					switch(zzCond->Cmpr)
					{
					case COND_EQ : return field.StringData == zzCond->Value.StringData;
					case COND_NEQ: return field.StringData != zzCond->Value.StringData;
					case COND_GT : return field.StringData >  zzCond->Value.StringData;
					case COND_GTE: return field.StringData >= zzCond->Value.StringData;
					case COND_LT : return field.StringData <  zzCond->Value.StringData;
					case COND_LTE: return field.StringData <= zzCond->Value.StringData;
					}
				}
				else if (field.ValueType == RPS_VT_INT) {
					//整数データ比較
					switch(zzCond->Cmpr)
					{
					case COND_EQ : return field.IntData == zzCond->Value.IntData;
					case COND_NEQ: return field.IntData != zzCond->Value.IntData;
					case COND_GT : return field.IntData >  zzCond->Value.IntData;
					case COND_GTE: return field.IntData >= zzCond->Value.IntData;
					case COND_LT : return field.IntData <  zzCond->Value.IntData;
					case COND_LTE: return field.IntData <= zzCond->Value.IntData;
					}
				}
				else if (field.ValueType == RPS_VT_DOUBLE) {
					//浮動小数データ比較
					double fieldData = field.DoubleData;
					double condData = zzCond->Value.DoubleData;
					switch(zzCond->Cmpr)
					{
					case COND_EQ : return fieldData == condData;
					case COND_NEQ: return fieldData != condData;
					case COND_GT : return fieldData >  condData;
					case COND_GTE: return fieldData >= condData;
					case COND_LT : return fieldData <  condData;
					case COND_LTE: return fieldData <= condData;
					}
				}
				return true;
			}
		};
	public:
		MockCondBuilder() : QueryExpr(RetTrue) {}
		std::function<bool(const ENTITY&)> QueryExpr;
		virtual void visit(const LeafCond* cond) override
		{
			QueryExpr = FieldCompare(cond);
		}
		virtual void visit(const AndCond* cond) override
		{
			auto builder1 = std::shared_ptr<MockCondBuilder>(new MockCondBuilder());
			cond->Cond1->acceptCondBuilder(*builder1);
			auto builder2 = std::shared_ptr<MockCondBuilder>(new MockCondBuilder());
			cond->Cond2->acceptCondBuilder(*builder2);
			QueryExpr = [=](const ENTITY& e){ return builder1->QueryExpr(e) && builder2->QueryExpr(e); };
			//※スマートポインタの参照カウントを上げるために値キャプチャ
		}
		virtual void visit(const OrCond* cond) override
		{
			auto builder1 = std::shared_ptr<MockCondBuilder>(new MockCondBuilder());
			cond->Cond1->acceptCondBuilder(*builder1);
			auto builder2 = std::shared_ptr<MockCondBuilder>(new MockCondBuilder());
			cond->Cond2->acceptCondBuilder(*builder2);
			QueryExpr = [=](const ENTITY& e){ return builder1->QueryExpr(e) || builder2->QueryExpr(e); };
			//※スマートポインタの参照カウントを上げるために値キャプチャ
		}
	};
public:
	std::vector<ENTITY> zzMockTable;
	explicit MockRepository(std::vector<ENTITY>&& table)
		: zzMockTable(std::move(table))
	{}
	virtual std::vector<ENTITY> findAll() override
	{
		return zzMockTable;
	}
	virtual std::vector<ENTITY> findAll(const IWhereCond& cond) override
	{
		//条件クエリを構築
		MockCondBuilder builder;
		cond.acceptCondBuilder(builder);
		//条件に合うものを取り出す
		std::vector<ENTITY> ret;
		for(auto itr = zzMockTable.begin(); itr != zzMockTable.end(); ++itr)
		{
			if (builder.QueryExpr(*itr)) {
				ret.push_back(*itr);
			}
		}
		return ret;
	}

	virtual std::shared_ptr<ENTITY> findFirst(const IWhereCond& cond) override
	{
		//条件クエリを構築
		MockCondBuilder builder;
		cond.acceptCondBuilder(builder);
		//条件に合うものを取り出す
		for(auto itr = zzMockTable.begin(); itr != zzMockTable.end(); ++itr)
		{
			if (builder.QueryExpr(*itr)) {
				return std::shared_ptr<ENTITY>(new ENTITY(*itr));
			}
		}
		return std::shared_ptr<ENTITY>();
	}

	virtual bool insert(const ENTITY& entity) override
	{
		zzMockTable.push_back(entity);
		return true;
	}

	virtual bool update(const IWhereCond& cond, const ENTITY& entity) override
	{
		//条件クエリを構築
		MockCondBuilder builder;
		cond.acceptCondBuilder(builder);

		//条件に合うものを更新
		for(auto itr = zzMockTable.begin(); itr != zzMockTable.end(); ++itr)
		{
			if (builder.QueryExpr(*itr)) {
				const auto fieldsInfo = ENTITY::FIELD_INFO_LIST();
				for(auto key = fieldsInfo.begin(); key != fieldsInfo.end(); ++key)
				{
					const RPS_FieldData& field = entity[key->Name]; 
					if (!field.HasData) { continue; }
					(*itr)[key->Name] = field;
				}
				return true;
			}
		}
		return false;
	}

	//削除
	virtual bool remove(const IWhereCond& cond) override
	{
		//条件クエリを構築
		MockCondBuilder builder;
		cond.acceptCondBuilder(builder);

		//条件を満たすものを削除
		zzMockTable.erase(std::remove_if(zzMockTable.begin(), zzMockTable.begin(), builder.QueryExpr), zzMockTable.end());

		return false;
	}


private:
};

} // end namespace GimLib

#endif

