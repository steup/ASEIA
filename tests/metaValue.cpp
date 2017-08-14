
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
  MetaValue i33 ({{{1234         }                 }                                    }, Int32::value() , 3, 3);
  MetaValue i42u({{{1234         }, {1235         }}                                    }, Int32::value() , 4, 2, true);
  MetaValue u22 ({{{1234         }, {1235         }}, {{1235         }, {1236         }}}, UInt32::value(), 1, 1);
  MetaValue u22u({{{1234   , 1   }, {1235   , 2   }}, {{1235   , 3   }, {1236   , 4   }}}, UInt32::value(), 2, 2, false);
  EXPECT_EQ((ValueType)d11 , ValueType(id::type::Double::value(), 1, 1, false));
  EXPECT_EQ((ValueType)d12 , ValueType(id::type::Double::value(), 1, 2, false));
  EXPECT_EQ((ValueType)d21 , ValueType(id::type::Double::value(), 2, 1, false));
  EXPECT_EQ((ValueType)d22 , ValueType(id::type::Double::value(), 2, 2, false));
  EXPECT_EQ((ValueType)d11u, ValueType(id::type::Double::value(), 1, 1, true ));
  EXPECT_EQ((ValueType)d12u, ValueType(id::type::Double::value(), 1, 2, true ));
  EXPECT_EQ((ValueType)d21u, ValueType(id::type::Double::value(), 2, 1, true ));
  EXPECT_EQ((ValueType)d22u, ValueType(id::type::Double::value(), 2, 2, true ));
  EXPECT_EQ((ValueType)f11 , ValueType(id::type::Float ::value(), 1, 1, false));
  EXPECT_EQ((ValueType)f12 , ValueType(id::type::Float ::value(), 1, 2, false));
  EXPECT_EQ((ValueType)f21 , ValueType(id::type::Float ::value(), 2, 1, false));
  EXPECT_EQ((ValueType)f22 , ValueType(id::type::Float ::value(), 2, 2, false));
  EXPECT_EQ((ValueType)f11u, ValueType(id::type::Float ::value(), 1, 1, true ));
  EXPECT_EQ((ValueType)f12u, ValueType(id::type::Float ::value(), 1, 2, true ));
  EXPECT_EQ((ValueType)f21u, ValueType(id::type::Float ::value(), 2, 1, true ));
  EXPECT_EQ((ValueType)f22u, ValueType(id::type::Float ::value(), 2, 2, true ));
  EXPECT_EQ((ValueType)i33 , ValueType(id::type::Int32 ::value(), 3, 3, false));
  EXPECT_EQ((ValueType)i42u, ValueType(id::type::Int32 ::value(), 4, 2, true ));
  EXPECT_EQ((ValueType)u22 , ValueType(id::type::UInt32::value(), 2, 2, false));
  EXPECT_EQ((ValueType)u22u, ValueType(id::type::UInt32::value(), 2, 2, true ));
}

TEST(MetaValueSuite, constantTest) {
  MetaValue i11(id::type::Int32::value(), 1, 1, false);
  MetaValue i12(id::type::Int32::value(), 1, 2, false);
  MetaValue i21(id::type::Int32::value(), 2, 1, false);
  MetaValue i22(id::type::Int32::value(), 2, 2, false);
  EXPECT_EQ(i11.identity(), MetaValue({{{1}}}, Int32::value()));
  EXPECT_EQ(i11.zero(), MetaValue({{{0}}}, Int32::value()));
  EXPECT_EQ(i12.identity(), MetaValue({{{1}, {0}}}, Int32::value()));
  EXPECT_EQ(i12.zero(), MetaValue({{{0}, {0}}}, Int32::value()));
  EXPECT_EQ(i21.identity(), MetaValue({{{1}}, {{0}}}, Int32::value()));
  EXPECT_EQ(i21.zero(), MetaValue({{{0}}, {{0}}}, Int32::value()));
  EXPECT_EQ(i22.identity(), MetaValue({{{1}, {0}}, {{0}, {1}}}, Int32::value()));
  EXPECT_EQ(i22.zero(), MetaValue({{{0}, {0}}, {{0}, {0}}}, Int32::value()));
}

TEST(MetaValueSuite, addTest) {
  MetaValue a = 5;
  MetaValue b = 7;
  EXPECT_EQ(a+b, MetaValue(+12));
  EXPECT_TRUE((a+b).valid()) << (a+b);
  EXPECT_EQ((ValueType)(a+b), ValueType(Double::value(), 1, 1, false));
  EXPECT_EQ(a+=b, MetaValue(+12));
  EXPECT_EQ((ValueType)a, ValueType(Double::value(), 1, 1, false));
  EXPECT_TRUE(a.valid()) << a;
  MetaValue c = {{{5, 1}, {6, 0}}};
  MetaValue d = {{{7, 0}, {8, 0}}};
  EXPECT_EQ(c+d, MetaValue({{{+12, 1}, {+14, 0.1}}}));
  EXPECT_TRUE((c+d).valid()) << (c+d);
  EXPECT_EQ((ValueType)(c+d), ValueType(Double::value(), 1, 2, true));
  MetaValue e = {{{5, 1}}, {{6, 0}}};
  MetaValue f = {{{7, 0}}, {{8, 0}}};
  EXPECT_EQ(e+f, MetaValue({{{+12, 1}}, {{+14, 0.1}}}));
  EXPECT_TRUE((e+f).valid()) << (e+f);
  EXPECT_EQ((ValueType)(e+f), ValueType(Double::value(), 2, 1, true));
  MetaValue g = {{{5, 1}, {6, 0}}, {{6, 0}, {7, 0}}};
  MetaValue h = {{{7, 0}, {8, 0}}, {{8, 0}, {9, 0}}};
  EXPECT_EQ(g+h, MetaValue({{{+12, 1}, {+14, 0.1}}, {{+14, 0.1}, {+16, 0.1}}}));
  EXPECT_TRUE((g+h).valid()) << (g+h);
  EXPECT_EQ((ValueType)(g+h), ValueType(Double::value(), 2, 2, true));
  EXPECT_FALSE((a+c).valid()) << (a+c);
  EXPECT_FALSE((c+e).valid()) << (c+e);
  EXPECT_FALSE((e+g).valid()) << (e+g);
  MetaValue i({{{5, 1}, {6, 0}}, {{6, 0}, {7, 0}}}, Float::value());
  MetaValue j({{{7, 0}, {8, 0}}, {{8, 0}, {9, 0}}}, Float::value());
  EXPECT_EQ(i+j, MetaValue({{{+12, 1}, {+14, 0.1}}, {{+14, 0.1}, {+16, 0.1}}}, Float::value()));
  EXPECT_TRUE((i+j).valid()) << (i+j);
  EXPECT_EQ((ValueType)(i+j), ValueType(Float::value(), 2, 2, true));
  MetaValue k({{{5, 1}, {6, 0}}, {{6, 0}, {7, 0}}}, Int8::value());
  MetaValue l({{{7, 0}, {8, 0}}, {{8, 0}, {9, 0}}}, Int8::value());
  EXPECT_EQ(k+l, MetaValue({{{+12, 1}, {+14, 0}}, {{+14, 0}, {+16, 0}}}, Int8::value()));
  EXPECT_TRUE((k+l).valid()) << (k+l);
  EXPECT_EQ((ValueType)(k+l), ValueType(Int8::value(), 2, 2, true));
  MetaValue m({{{250, 1}, {6, 0}}, {{6, 0}, {7, 0}}}, UInt8::value());
  MetaValue n({{{7, 0}, {8, 0}}, {{8, 0}, {9, 0}}}, UInt8::value());
  EXPECT_EQ(m+n, MetaValue({{{255, 3}, {14, 2}}, {{14, 2}, {16, 2}}}, UInt8::value()));
  EXPECT_TRUE((m+n).valid()) << (m+n);
  EXPECT_EQ((ValueType)(k+l), ValueType(Int8::value(), 2, 2, true));
}

TEST(MetaValueSuite, subTest) {
  MetaValue a = 5;
  MetaValue b = 7;
  EXPECT_EQ(a-b, MetaValue(-2));
  EXPECT_TRUE((a-b).valid()) << (a-b);
  EXPECT_EQ((ValueType)(a-b), ValueType(Double::value(), 1, 1, false));
  EXPECT_EQ(a-=b, MetaValue(-2));
  EXPECT_EQ((ValueType)a, ValueType(Double::value(), 1, 1, false));
  EXPECT_TRUE(a.valid()) << a;
  MetaValue c = {{{5, 1}, {6, 0}}};
  MetaValue d = {{{7, 0}, {8, 0}}};
  EXPECT_EQ(c-d, MetaValue({{{-2, 1}, {-2, 0.1}}}));
  EXPECT_TRUE((c-d).valid()) << (c-d);
  EXPECT_EQ((ValueType)(c-d), ValueType(Double::value(), 1, 2, true));
  MetaValue e = {{{5, 1}}, {{6, 0}}};
  MetaValue f = {{{7, 0}}, {{8, 0}}};
  EXPECT_EQ(e-f, MetaValue({{{-2, 1}}, {{-2, 0.1}}}));
  EXPECT_TRUE((e-f).valid()) << (e-f);
  EXPECT_EQ((ValueType)(e-f), ValueType(Double::value(), 2, 1, true));
  MetaValue g = {{{5, 1}, {6, 0}}, {{6, 0}, {7, 0}}};
  MetaValue h = {{{7, 0}, {8, 0}}, {{8, 0}, {9, 0}}};
  EXPECT_EQ(g-h, MetaValue({{{-2, 1}, {-2, 0.1}}, {{-2, 0.1}, {-2, 0.1}}}));
  EXPECT_TRUE((g-h).valid()) << (g-h);
  EXPECT_EQ((ValueType)(g-h), ValueType(Double::value(), 2, 2, true));
  EXPECT_FALSE((a-c).valid()) << (a-c);
  EXPECT_FALSE((c-e).valid()) << (c-e);
  EXPECT_FALSE((e-g).valid()) << (e-g);
  MetaValue i({{{5, 1}, {6, 0}}, {{6, 0}, {7, 0}}}, Float::value());
  MetaValue j({{{7, 0}, {8, 0}}, {{8, 0}, {9, 0}}}, Float::value());
  EXPECT_EQ(i-j, MetaValue({{{-2, 1}, {-2, 0.1}}, {{-2, 0.1}, {-2, 0.1}}}, Float::value()));
  EXPECT_TRUE((i-j).valid()) << (i-j);
  EXPECT_EQ((ValueType)(i-j), ValueType(Float::value(), 2, 2, true));
  MetaValue k({{{5, 1}, {6, 0}}, {{6, 0}, {7, 0}}}, Int8::value());
  MetaValue l({{{7, 0}, {8, 0}}, {{8, 0}, {9, 0}}}, Int8::value());
  EXPECT_EQ(k-l, MetaValue({{{-2, 1}, {-2, 0}}, {{-2, 0}, {-2, 0}}}, Int8::value()));
  EXPECT_TRUE((k-l).valid()) << (k-l);
  EXPECT_EQ((ValueType)(k-l), ValueType(Int8::value(), 2, 2, true));
  MetaValue m({{{5, 1}, {6, 0}}, {{6, 0}, {7, 0}}}, UInt8::value());
  MetaValue n({{{7, 0}, {8, 0}}, {{8, 0}, {9, 0}}}, UInt8::value());
  EXPECT_EQ(m-n, MetaValue({{{0, 3}, {0, 2}}, {{0, 2}, {0, 2}}}, UInt8::value()));
  EXPECT_TRUE((m-n).valid()) << (m-n);
  EXPECT_EQ((ValueType)(k-l), ValueType(Int8::value(), 2, 2, true));
}

TEST(MetaValueSuite, mulTest) {
  MetaValue a = 5;
  MetaValue b = 6;
  EXPECT_TRUE((a*b).valid()) << (a*b);
  EXPECT_EQ(a*b, MetaValue(30));
  EXPECT_EQ((ValueType)(a*b), ValueType(Double::value(), 1, 1 ,false));
  MetaValue c(5, Int32::value());
  MetaValue d(6, Int32::value());
  EXPECT_TRUE((c*d).valid()) << (c*d);
  EXPECT_EQ(c*d, MetaValue(30, Int32::value()));
  EXPECT_EQ((ValueType)(c*d), ValueType(Int32::value(), 1, 1 ,false));
  MetaValue e = {{{5, 1}, {6, 0}}};
  MetaValue f = {{{6, 0}}, {{5, 1}}};
  EXPECT_TRUE((e*f).valid()) << (e*f);
  EXPECT_EQ(e*f, MetaValue({{{60, 12}}}));
  EXPECT_EQ((ValueType)(e*f), ValueType(Double::value(), 1, 1 ,true));
  MetaValue g = {{{5, 1}, {6, 0}}};
  MetaValue h = {{{6, 0}, {5, 1}}};
  EXPECT_TRUE((g*h).valid()) << (g*h); //Element wise multiplication, because no other valid operation defined for v*v
  EXPECT_EQ(g*h, MetaValue({{{30, 6}, {30, 6}}}));
  EXPECT_EQ((ValueType)(g*h), ValueType(Double::value(), 1, 2 ,true));
  MetaValue i = {{{5, 1}, {6, 0}}, {{1, 0}, {0, 0}}};
  MetaValue j = {{{6, 0}}, {{5, 1}}};
  EXPECT_TRUE((i*j).valid()) << (i*j);
  EXPECT_EQ(i*j, MetaValue({{{60, 12}}, {{6, 0}}}));
  EXPECT_EQ((ValueType)(i*j), ValueType(Double::value(), 2, 1 ,true));
  MetaValue k = {{{6, 0}, {5, 1}}};
  MetaValue l = {{{5, 1}, {6, 0}, {0, 1}}, {{1, 0}, {0, 0}, {4,0}}};
  EXPECT_TRUE((k*l).valid()) << (k*l);
  EXPECT_EQ(k*l, MetaValue({{{35, 7}, {36, 0.1}, {20, 10}}}));
  EXPECT_EQ((ValueType)(k*l), ValueType(Double::value(), 1, 3 ,true));
}

TEST(MetaValueSuite, divTest) {
  MetaValue a = {{{5, 1}, {3,0}, {4, 0}}, {{6, 1}, {7, 0}, {8, 0}}};
  EXPECT_EQ(a/MetaValue({{{5, 0}}}), MetaValue({{{5.0/5, 1.0/5}, {3.0/5,0}, {4.0/5, 0}}, {{6.0/5, 1}, {7.0/5, 0}, {8.0/5, 0}}}));
  EXPECT_EQ(a/5, MetaValue({{{5.0/5, 1.0/5}, {3.0/5,0}, {4.0/5, 0}}, {{6.0/5, 1.0/5}, {7.0/5, 0}, {8.0/5, 0}}}));
  EXPECT_FALSE((a/MetaValue({{{5}, {1}}})).valid()) << (a/MetaValue({{{5}, {1}}}));
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
  MetaValue aiuRef = 1;
  EXPECT_TRUE((ai.norm() - aiRef)<0.1) << ai.norm() << " != " << aiRef;
  EXPECT_TRUE((aiu.norm().value() - aiRef).abs()<0.1) << aiu.norm() << " != " << aiRef;
  EXPECT_TRUE((aiuRef - aiu.norm().uncertainty()).abs()<0.1) << aiu.norm().uncertainty() << " != " << aiuRef;
}

TEST(MetaValueSuite, sqrtTest) {
  MetaValue ai = {{{5}}};
  MetaValue aiu = {{{5, 1}}};
  MetaValue bi = {{{5}, {6}}};
  MetaValue biu = {{{5, 1}, {6, 2}}};
  MetaValue ci = {{{5}}, {{6}}};
  MetaValue ciu = {{{5, 1}}, {{6, 2}}};
  MetaValue di = {{{5}}, {{6}}, {{7}}, {{8}}};
  MetaValue diu = {{{5, 1}}, {{6, 2}}, {{7, 1}}, {{8, 2}}};
  MetaValue aiRef = 2.2;
  MetaValue aiuRef =0.225;
  EXPECT_TRUE((ai.sqrt() - aiRef)<0.1) << ai << " != " << aiRef;
  EXPECT_TRUE((aiuRef - aiu.sqrt().uncertainty()).abs()<0.1) << aiu << " - " << aiu.uncertainty() << " != " << aiuRef;
  EXPECT_TRUE((aiu.value() - aiRef).abs()<0.1) << aiu.value() << " != " << aiRef;
}

TEST(MetaValueSuite, autoCastTest) {
  MetaValue d11 = 5;
  MetaValue d11u = {{{5, 1}}};
  EXPECT_TRUE((d11*d11u).valid()) << d11 << " x " << d11u << " = invalid";
  EXPECT_EQ((ValueType)(d11*d11u), ValueType(Double::value(), 1, 1, true));
  EXPECT_LE((d11*d11u).value()-MetaValue(25), 0.1);
  EXPECT_LE((d11*d11u).uncertainty()-MetaValue(5), 0.1);
  MetaValue f11(5, Float::value());
  MetaValue f11u({{{5, 1}}}, Float::value());
  EXPECT_TRUE((f11*f11u).valid()) << f11 << " x " << f11u << " = invalid";
  EXPECT_EQ((ValueType)(f11*f11u), ValueType(Float::value(), 1, 1, true));
  EXPECT_LE((f11*f11u).value()-MetaValue(25), 0.1);
  EXPECT_LE((f11*f11u).uncertainty()-MetaValue(5, Float::value()), 0.1);
  EXPECT_TRUE((f11*d11).valid()) << f11 << " x " << d11 << " = invalid";
  EXPECT_EQ((ValueType)(f11*d11), ValueType(Double::value(), 1, 1, false));
  EXPECT_LE((f11*d11).value()-MetaValue(25), 0.1);
  EXPECT_TRUE((f11u*d11u).valid()) << f11u << " x " << d11u << " = invalid";
  EXPECT_EQ((ValueType)(f11u*d11u), ValueType(Double::value(), 1, 1, true));
  EXPECT_LE((f11*d11u).value()-MetaValue(25), 0.1);
  EXPECT_LE((f11*d11u).uncertainty()-MetaValue(5), 0.1);
}

}}
