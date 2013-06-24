#include "stdafx.h"
#include "picotest.h"
#include "include/GridSheet.h"
#include "GimLibTestHelper.h"

using GimLib::Grid;
using GimLib::Point;

//////////////////////////////////////////////////
namespace {
//////////////////////////////////////////////////

//-----------------------------------
// テストケース
//-----------------------------------
//
TEST(GridTest, Grid_3_4)
{
	Grid<3,4> g;
	g.AB(3);
	g.BC(5);
	g.ab(2);
	g.bc(3);
	g.cd(4);
	const Point Aa = g.Aa(); const Point Ba = g.Ba(); const Point Ca = g.Ca();
	const Point Ab = g.Ab(); const Point Bb = g.Bb(); const Point Cb = g.Cb();
	const Point Ac = g.Ac(); const Point Bc = g.Bc(); const Point Cc = g.Cc();
	const Point Ad = g.Ad(); const Point Bd = g.Bd(); const Point Cd = g.Cd();

	EXPECT_EQ(0, Aa.x); EXPECT_EQ(0, Aa.y);
	EXPECT_EQ(3, Ba.x); EXPECT_EQ(0, Ba.y);
	EXPECT_EQ(8, Ca.x); EXPECT_EQ(0, Ca.y);
	EXPECT_EQ(0, Ab.x); EXPECT_EQ(2, Ab.y);
	EXPECT_EQ(3, Bb.x); EXPECT_EQ(2, Bb.y);
	EXPECT_EQ(8, Cb.x); EXPECT_EQ(2, Cb.y);
	EXPECT_EQ(0, Ac.x); EXPECT_EQ(5, Ac.y);
	EXPECT_EQ(3, Bc.x); EXPECT_EQ(5, Bc.y);
	EXPECT_EQ(8, Cc.x); EXPECT_EQ(5, Cc.y);
	EXPECT_EQ(0, Ad.x); EXPECT_EQ(9, Ad.y);
	EXPECT_EQ(3, Bd.x); EXPECT_EQ(9, Bd.y);
	EXPECT_EQ(8, Cd.x); EXPECT_EQ(9, Cd.y);
}

TEST(GridTest, Grid_8_8)
{
	Grid<8,7> g;
	g.AB(1); g.BC(2); g.CD(3); g.DE(4); g.EF(5); g.FG(6); g.GH(7);
	g.ab(2); g.bc(3); g.cd(4); g.de(5); g.ef(6); g.fg(7);
	const Point Aa = g.Aa(); const Point Ba = g.Ba(); const Point Ca = g.Ca(); const Point Da = g.Da();
	const Point Ea = g.Ea(); const Point Fa = g.Fa(); const Point Ga = g.Ga(); const Point Ha = g.Ha();
	const Point Ab = g.Ab(); const Point Bb = g.Bb(); const Point Cb = g.Cb(); const Point Db = g.Db();
	const Point Eb = g.Eb(); const Point Fb = g.Fb(); const Point Gb = g.Gb(); const Point Hb = g.Hb();
	const Point Ac = g.Ac(); const Point Bc = g.Bc(); const Point Cc = g.Cc(); const Point Dc = g.Dc();
	const Point Ec = g.Ec(); const Point Fc = g.Fc(); const Point Gc = g.Gc(); const Point Hc = g.Hc();
	const Point Ad = g.Ad(); const Point Bd = g.Bd(); const Point Cd = g.Cd(); const Point Dd = g.Dd();
	const Point Ed = g.Ed(); const Point Fd = g.Fd(); const Point Gd = g.Gd(); const Point Hd = g.Hd();
	const Point Ae = g.Ae(); const Point Be = g.Be(); const Point Ce = g.Ce(); const Point De = g.De();
	const Point Ee = g.Ee(); const Point Fe = g.Fe(); const Point Ge = g.Ge(); const Point He = g.He();
	const Point Af = g.Af(); const Point Bf = g.Bf(); const Point Cf = g.Cf(); const Point Df = g.Df();
	const Point Ef = g.Ef(); const Point Ff = g.Ff(); const Point Gf = g.Gf(); const Point Hf = g.Hf();
	const Point Ag = g.Ag(); const Point Bg = g.Bg(); const Point Cg = g.Cg(); const Point Dg = g.Dg();
	const Point Eg = g.Eg(); const Point Fg = g.Fg(); const Point Gg = g.Gg(); const Point Hg = g.Hg();

	EXPECT_EQ(Point( 0, 0), Aa); EXPECT_EQ(Point( 1, 0), Ba); EXPECT_EQ(Point( 3, 0), Ca); EXPECT_EQ(Point( 6, 0), Da);
	EXPECT_EQ(Point(10, 0), Ea); EXPECT_EQ(Point(15, 0), Fa); EXPECT_EQ(Point(21, 0), Ga); EXPECT_EQ(Point(28, 0), Ha);
	EXPECT_EQ(Point( 0, 2), Ab); EXPECT_EQ(Point( 1, 2), Bb); EXPECT_EQ(Point( 3, 2), Cb); EXPECT_EQ(Point( 6, 2), Db);
	EXPECT_EQ(Point(10, 2), Eb); EXPECT_EQ(Point(15, 2), Fb); EXPECT_EQ(Point(21, 2), Gb); EXPECT_EQ(Point(28, 2), Hb);
	EXPECT_EQ(Point( 0, 5), Ac); EXPECT_EQ(Point( 1, 5), Bc); EXPECT_EQ(Point( 3, 5), Cc); EXPECT_EQ(Point( 6, 5), Dc);
	EXPECT_EQ(Point(10, 5), Ec); EXPECT_EQ(Point(15, 5), Fc); EXPECT_EQ(Point(21, 5), Gc); EXPECT_EQ(Point(28, 5), Hc);
	EXPECT_EQ(Point( 0, 9), Ad); EXPECT_EQ(Point( 1, 9), Bd); EXPECT_EQ(Point( 3, 9), Cd); EXPECT_EQ(Point( 6, 9), Dd);
	EXPECT_EQ(Point(10, 9), Ed); EXPECT_EQ(Point(15, 9), Fd); EXPECT_EQ(Point(21, 9), Gd); EXPECT_EQ(Point(28, 9), Hd);
	EXPECT_EQ(Point( 0,14), Ae); EXPECT_EQ(Point( 1,14), Be); EXPECT_EQ(Point( 3,14), Ce); EXPECT_EQ(Point( 6,14), De);
	EXPECT_EQ(Point(10,14), Ee); EXPECT_EQ(Point(15,14), Fe); EXPECT_EQ(Point(21,14), Ge); EXPECT_EQ(Point(28,14), He);
	EXPECT_EQ(Point( 0,20), Af); EXPECT_EQ(Point( 1,20), Bf); EXPECT_EQ(Point( 3,20), Cf); EXPECT_EQ(Point( 6,20), Df);
	EXPECT_EQ(Point(10,20), Ef); EXPECT_EQ(Point(15,20), Ff); EXPECT_EQ(Point(21,20), Gf); EXPECT_EQ(Point(28,20), Hf);
	EXPECT_EQ(Point( 0,27), Ag); EXPECT_EQ(Point( 1,27), Bg); EXPECT_EQ(Point( 3,27), Cg); EXPECT_EQ(Point( 6,27), Dg);
	EXPECT_EQ(Point(10,27), Eg); EXPECT_EQ(Point(15,27), Fg); EXPECT_EQ(Point(21,27), Gg); EXPECT_EQ(Point(28,27), Hg);
}

//////////////////////////////////////////////////
} // end no name namespace
//////////////////////////////////////////////////