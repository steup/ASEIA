
#include <gtest/gtest.h>
#include <MetaValue.h>
#include <MetaFactory.h>

#include <IO.h>

#include <iostream>

namespace tests {

namespace metaValueSuite {

using namespace std;
using namespace ::id::type;

TEST(MetaValueSuite, createTest) {
  MetaValue d11 = {{{1234.0}}};
  MetaValue d12 = {{{1234.0}, {1235.0}}};
  MetaValue d21 = {{{1234.0}}, {{1235.0}}};
  MetaValue d22 = {{{1234.0}, {1235.0}}, {{1235.0}, {1236.0}}};
  MetaValue d11u = {{{1234.0, 1.0}}};
  MetaValue d12u = {{{1234.0, 1.0}, {1235.0, 2.0}}};
  MetaValue d21u = {{{1234.0, 1.0}}, {{1235.0, 2.0}}};
  MetaValue d22u = {{{1234.0, 1.0}, {1235.0, 2.0}}, {{1235.0, 3.0}, {1236.0, 4.0}}};
  MetaValue f11 ({{{1234.0f      }                 }                                    }, Float::value());
  MetaValue f12 ({{{1234.0f      }, {1235.0f      }}                                    }, Float::value());
  MetaValue f21 ({{{1234.0f      }                 }, {{1235.0f      }                 }}, Float::value());
  MetaValue f22 ({{{1234.0f      }, {1235.0f      }}, {{1235.0f      }, {1236.0f      }}}, Float::value());
  MetaValue f11u({{{1234.0f, 1.0f}                 }                                    }, Float::value());
  MetaValue f12u({{{1234.0f, 1.0f}, {1235.0f, 2.0f}}                                    }, Float::value());
  MetaValue f21u({{{1234.0f, 1.0f}                 }, {{1235.0f, 2.0f}                 }}, Float::value());
  MetaValue f22u({{{1234.0f, 1.0f}, {1235.0f, 2.0f}}, {{1235.0f, 3.0f}, {1236.0f, 4.0f}}}, Float::value());
  MetaValue i33 ({{{1234         }                 }                                    }, Int32::value(), 3, 3);
  MetaValue i42u({{{1234         }, {1235         }}                                    }, Int32::value(), 4, 2, true);
  MetaValue u22 ({{{1234         }, {1235         }}, {{1235         }, {1236         }}}, UInt32::value(), 1, 1);
  MetaValue u22u({{{1234   , 1   }, {1235   , 2   }}, {{1235   , 3   }, {1236   , 4   }}}, UInt32::value(), 2, 2, false);
  EXPECT_EQ((ValueType)d11, ValueType(id::type::Double::value(), 1, 1, false));
  EXPECT_EQ((ValueType)d12, ValueType(id::type::Double::value(), 1, 2, false));
  EXPECT_EQ((ValueType)d21, ValueType(id::type::Double::value(), 2, 1, false));
  EXPECT_EQ((ValueType)d22, ValueType(id::type::Double::value(), 2, 2, false));
  EXPECT_EQ((ValueType)d11u, ValueType(id::type::Double::value(), 1, 1, true));
  EXPECT_EQ((ValueType)d12u, ValueType(id::type::Double::value(), 1, 2, true));
  EXPECT_EQ((ValueType)d21u, ValueType(id::type::Double::value(), 2, 1, true));
  EXPECT_EQ((ValueType)d22u, ValueType(id::type::Double::value(), 2, 2, true));
  EXPECT_EQ((ValueType)f11, ValueType(id::type::Float::value(), 1, 1, false));
  EXPECT_EQ((ValueType)f12, ValueType(id::type::Float::value(), 1, 2, false));
  EXPECT_EQ((ValueType)f21, ValueType(id::type::Float::value(), 2, 1, false));
  EXPECT_EQ((ValueType)f22, ValueType(id::type::Float::value(), 2, 2, false));
  EXPECT_EQ((ValueType)f11u, ValueType(id::type::Float::value(), 1, 1, true));
  EXPECT_EQ((ValueType)f12u, ValueType(id::type::Float::value(), 1, 2, true));
  EXPECT_EQ((ValueType)f21u, ValueType(id::type::Float::value(), 2, 1, true));
  EXPECT_EQ((ValueType)f22u, ValueType(id::type::Float::value(), 2, 2, true));
  EXPECT_EQ((ValueType)i33, ValueType(id::type::Int32::value(), 3, 3, false));
  EXPECT_EQ((ValueType)i42u, ValueType(id::type::Int32::value(), 4, 2, true));
  EXPECT_EQ((ValueType)u22, ValueType(id::type::UInt32::value(), 2, 2, false));
  EXPECT_EQ((ValueType)u22u, ValueType(id::type::UInt32::value(), 2, 2, true));
}

TEST(MetaValueSuite, constantTest) {
  MetaFactory& f = MetaFactory::instance();
  MetaValue i11 = f.create(id::type::Int32::value(), 1, 1, false);
  MetaValue i12 = f.create(id::type::Int32::value(), 1, 2, false);
  MetaValue i21 = f.create(id::type::Int32::value(), 2, 1, false);
  MetaValue i22 = f.create(id::type::Int32::value(), 2, 2, false);
  EXPECT_EQ(i11.identity(), MetaValue({{{1}}}, Int32::value()));
  EXPECT_EQ(i11.zero(), MetaValue({{{0}}}, Int32::value()));
  EXPECT_EQ(i12.identity(), MetaValue({{{1}, {0}}}, Int32::value()));
  EXPECT_EQ(i12.zero(), MetaValue({{{0}, {0}}}, Int32::value()));
  EXPECT_EQ(i21.identity(), MetaValue({{{1}}, {{0}}}, Int32::value()));
  EXPECT_EQ(i21.zero(), MetaValue({{{0}}, {{0}}}, Int32::value()));
  EXPECT_EQ(i22.identity(), MetaValue({{{1}, {0}}, {{0}, {1}}}, Int32::value()));
  EXPECT_EQ(i22.zero(), MetaValue({{{0}, {0}}, {{0}, {0}}}, Int32::value()));
}

TEST(MetaValueSuite, uInt8Test) {
	MetaFactory& f = MetaFactory::instance();
	MetaValue v0=f.create({{{13, 37}, {1, 0}, {3, 7}}}, id::type::UInt8::value());
	MetaValue v2=f.create({{{73, 1}, {3, 1}, {7, 1}}}, id::type::UInt8::value());
	//EXPECT_TRUE(v0.binaryConstOp(BCOp::Equal, v0)->prod()) << "Equally assigned values are not equal";
	//EXPECT_TRUE(v0.binaryConstOp(BCOp::NotEqual, v2)->prod()) << "Unequally assigned values are equal";
	//EXPECT_FALSE(v0.binaryConstOp(BCOp::Equal, v2)->prod()) << "Unequally assigned values are equal";
	//EXPECT_FALSE(v0.binaryConstOp(BCOp::NotEqual, v2)->prod()) << "Equally assigned values are not equal";
}

TEST(MetaValueSuite, subTest) {
  MetaValue a = 5;
  MetaValue b = 7;
  EXPECT_EQ(a-b, MetaValue(-2));
  EXPECT_EQ(a-=b, MetaValue(-2));
}

TEST(MetaValueSuite, normTest) {
  MetaValue ai = {{{5}}};
  MetaValue aiu = {{{5, 1}}};
  MetaValue bi = {{{5}, {6}}};
  MetaValue biu = {{{5, 1}, {6, 2}}};
  MetaValue ci = {{{5}}, {{6}}};
  MetaValue ciu = {{{5, 1}}, {{6, 2}}};
  MetaValue di = {{{5}}, {{6}}, {{7}}, {{8}}};
  MetaValue diu = {{{5, 1}}, {{6, 2}}, {{7, 1}}, {{8, 2}}};
  MetaValue aiRef = 5;
  EXPECT_TRUE((ai.norm() - aiRef)<0.1) << ai.norm() << " != " << aiRef;
}

}}
