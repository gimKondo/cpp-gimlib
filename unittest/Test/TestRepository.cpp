#include "stdafx.h"
#include "picotest.h"
#include "include/Repository.h"
#include "include/RepositoryEntityMacro.h"
#include "include/container_make.h"
#include "util/RepositoryMock.h"

using namespace GimLib;

//////////////////////////////////////////////////
namespace {
//////////////////////////////////////////////////

// generate Profile table entity
RPS_ENTITY_CLASS5(BirdEntity, Profile, INT, ID, STRING, Species, STRING, JapaneseName, STRING, Family, DOUBLE, BodyLength);

//initialize repository for test
std::vector<BirdEntity> makeProfileTableMock()
{
	return GimLib::make_vector<BirdEntity>()
		(BirdEntity(1, L"Passer montanus"		, L"スズメ"			, L"Passeridae"	, 14.0))
		(BirdEntity(2, L"Erithacus akahige"		, L"コマドリ"		, L"Turdidae"	, 14.0))
		(BirdEntity(3, L"Sturnus cineraceus"	, L"ムクドリ"		, L"Sturnidae"	, 24.0))
		(BirdEntity(4, L"Turdus naumanni"		, L"ツグミ"			, L"Turdidae"	, 24.0))
		(BirdEntity(5, L"Parus varius"			, L"ヤマガラ"		, L"Paridae"	, 14.0))
		(BirdEntity(6, L"Parus minor"			, L"シジュウカラ"	, L"Paridae"	, 14.5));
}


enum RepositoryType
{
//	REPO_TYPE_MDB ,
	REPO_TYPE_TEST,
};

class RepositoryManager
{
public:
	static std::shared_ptr<IRepository<BirdEntity>> GetBziRepository(RepositoryType type)
	{
		if (type == REPO_TYPE_TEST) {
			static auto repo = std::shared_ptr<IRepository<BirdEntity>>(new MockRepository<BirdEntity>(makeProfileTableMock()));
			return repo;
		}
		return std::shared_ptr<IRepository<BirdEntity>>();
	}
};


//-----------------------------------
// Test
//-----------------------------------
TEST(RepositoryTest, FindAll)
{
	auto repo = RepositoryManager::GetBziRepository(REPO_TYPE_TEST);
	auto recs = repo->findAll();
	EXPECT_EQ( 6, recs.size());
	EXPECT_EQ(L"Passer montanus", recs[0].Species		);
	EXPECT_EQ(L"スズメ"			, recs[0].JapaneseName	);
	EXPECT_EQ(L"Passeridae"		, recs[0].Family		);
	EXPECT_DOUBLE_EQ(14.0		, recs[0].BodyLength	);
}

TEST(RepositoryTest, FindWithCondition)
{
	auto repo = RepositoryManager::GetBziRepository(REPO_TYPE_TEST);
	auto recs = repo->findAll(LeafCond(L"BodyLength", 20.0, COND_GT));
	EXPECT_EQ( 2, recs.size());
	EXPECT_EQ(L"Sturnus cineraceus"	, recs[0].Species		);
	EXPECT_EQ(L"ムクドリ"			, recs[0].JapaneseName	);
	EXPECT_EQ(L"Sturnidae"			, recs[0].Family		);
	EXPECT_DOUBLE_EQ(24.0			, recs[0].BodyLength	);
}

TEST(RepositoryTest, FindWithAndConditions)
{
	auto repo = RepositoryManager::GetBziRepository(REPO_TYPE_TEST);
	auto recs = repo->findAll(LeafCond(L"Family", L"Turdidae") & LeafCond(L"BodyLength", 20.0, COND_GT));
	EXPECT_EQ( 1, recs.size());
	EXPECT_EQ(L"Turdus naumanni", recs[0].Species		);
	EXPECT_EQ(L"ツグミ"			, recs[0].JapaneseName	);
	EXPECT_EQ(L"Turdidae"		, recs[0].Family		);
	EXPECT_DOUBLE_EQ(24.0		, recs[0].BodyLength	);
}

//////////////////////////////////////////////////
} // end no name namespace
//////////////////////////////////////////////////