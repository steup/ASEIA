#include <KnowledgeBase.h>

namespace test {

 using namespace ::id::attribute; 

  struct KnowledgeBaseTestSuite : public ::testing::Test {
    
    struct Test0: public Base {
      static constexpr const ID value()  { return 252; }
    };

    struct Test1 : public Base {
      static constexpr const ID value()  { return 253; }
    };

    struct Test2 : public Base {
      static constexpr const ID value()  { return 254; }
    };

    struct Test3 : public Base {
      static constexpr const ID value()  { return 255; }
    };
    
    EventType

    KnowledgeBaseTestSuite() {
      
      struct HetTrans1 : public Transformation {
        
      };
      
      struct HetTrans2 : public Transformation {

      };
      
      struct HomTrans1 : public Transformation {

      };
      
      struct HomTrans2 : public Transformation {

      };
      
    }
  };

}
