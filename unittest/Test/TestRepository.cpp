#include "stdafx.h"
#include "picotest.h"
#include "include/Repository.h"
#include "include/RepositoryEntityMacro.h"
#include "include/container_make.h"

using namespace GimLib;

//////////////////////////////////////////////////
namespace {
//////////////////////////////////////////////////

//--------------------------------
// テスト用のモックリポジトリ
//--------------------------------
template <typename ENTITY> class MockRepository : public IRepository<ENTITY>
{
public:
	class MockCondBuilder : public CondBuilder
	{
	private:
		static bool RetTrue(const ENTITY&) { return true; }
	public:
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
				//compare string
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
				//compare integer
				else if (field.ValueType == RPS_VT_INT) {
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
				//compare double
				else if (field.ValueType == RPS_VT_DOUBLE) {
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
	static std::vector<ENTITY> MockTable;
	virtual std::vector<ENTITY> findAll() override
	{
		return findAll(std::shared_ptr<IWhereCond>());
	}
	virtual std::vector<ENTITY> findAll(const std::shared_ptr<IWhereCond>& cond) override
	{
		//条件クエリを構築
		MockCondBuilder builder;
		if(cond) {
			cond->acceptCondBuilder(builder);
		}
		//条件に合うものを取り出す
		std::vector<ENTITY> ret;
		for(auto itr = MockTable.begin(); itr != MockTable.end(); ++itr)
		{
			if (builder.QueryExpr(*itr)) {
				ret.push_back(*itr);
			}
		}
		return ret;
	}

	virtual std::shared_ptr<ENTITY> findFirst(const std::shared_ptr<IWhereCond>& cond) override
	{
		//条件クエリを構築
		MockCondBuilder builder;
		if(cond) {
			cond->acceptCondBuilder(builder);
		}
		//条件に合うものを取り出す
		for(auto itr = MockTable.begin(); itr != MockTable.end(); ++itr)
		{
			if (builder.QueryExpr(*itr)) {
				return std::shared_ptr<ENTITY>(new ENTITY(*itr));
			}
		}
		return std::shared_ptr<ENTITY>();
	}

	virtual bool insert(const ENTITY& entity) override
	{
		MockTable.push_back(entity);
		return true;
	}
	virtual bool update(const std::shared_ptr<IWhereCond>& cond, const ENTITY& entity) override
	{
		return false;
	}
	virtual bool remove(const std::shared_ptr<IWhereCond>& cond) override
	{
		return false;
	}
};




//
RPS_ENTITY_CLASS5(ProfileEntity, Profile, INT, ID, STRING, FirstName, STRING, LastName, DOUBLE, Height, DOUBLE, Weight);


//テスト用リポジトリの初期化
std::vector<ProfileEntity> MockRepository<ProfileEntity>::MockTable
	= GimLib::make_vector<ProfileEntity>()
		(ProfileEntity(1, L"Daisuke", L"Kondo"	, 185.0, 73.5))
		(ProfileEntity(2, L"Takashi", L"Kondo"	, 182.5, 67.3))
		(ProfileEntity(3, L"Yuka"	, L"Kanai"	, 162.2, 51.0))
		(ProfileEntity(4, L"Reiko"	, L"Kato"	, 163.5, 58.5));


enum RepositoryType
{
//	REPO_TYPE_MDB ,
	REPO_TYPE_TEST,
};

class RepositoryManager
{
public:
	static std::shared_ptr<IRepository<ProfileEntity>> GetBziRepository(RepositoryType type)
	{
		if (type == REPO_TYPE_TEST) {
			static auto repo = std::shared_ptr<IRepository<ProfileEntity>>(new MockRepository<ProfileEntity>());
			return repo;
		}
		return std::shared_ptr<IRepository<ProfileEntity>>();
	}
};


//-----------------------------------
// テストケース
//-----------------------------------
//
TEST(RepositoryTest, 全取得)
{
	MockRepository<ProfileEntity> repo;
	auto recs = repo.findAll();
	EXPECT_EQ( 4, recs.size());
	EXPECT_EQ(L"Daisuke", recs[0].FirstName);
	EXPECT_EQ(L"Kondo"	, recs[0].LastName);
	EXPECT_DOUBLE_EQ(185.0, recs[0].Height);
	EXPECT_DOUBLE_EQ( 73.5, recs[0].Weight);
}

TEST(RepositoryTest, 条件付取得)
{
	MockRepository<ProfileEntity> repo;
	auto cond = std::shared_ptr<IWhereCond>( new LeafCond(L"Height", 180.0, COND_GT) );
	auto recs = repo.findAll(cond);
	EXPECT_EQ( 2, recs.size());
	EXPECT_EQ(L"Takashi", recs[1].FirstName);
	EXPECT_EQ(L"Kondo"	, recs[1].LastName);
	EXPECT_DOUBLE_EQ(182.5, recs[1].Height);
	EXPECT_DOUBLE_EQ( 67.3, recs[1].Weight);
}

//////////////////////////////////////////////////
} // end no name namespace
//////////////////////////////////////////////////