#include <Attribute.h>
#include <Value.h>
#include <IO.h>
#include <ID.h>

#include <iostream>

using namespace std;

using Vector3iu=Value<float,3,1,true>;

using Position=Attribute<id::attribute::Position, Vector3iu, Meter, std::ratio<1,10>>;

int main()
{
  Position pos={{{0.0f,0.0f}},
								{{0.0f, 0.0f}},
								{{1.0f,0.0f}}};

  cout << endl << pos << endl; 
  return 0;
}
