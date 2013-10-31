#include <iostream>

/*#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>*/

using namespace std;
//using namespace boost::program_options;

#include <dataType.h>


int main(int argc, char** argv)
{
/*  options_description d("Usage");
  d.add_options()
    ("help", "this help text");
  variables_map vm;
  store(parse_command_line(argc, argv, d), vm);
  notify(vm);*/

  cout << "Attribute System test" << endl;

  Unit milliJoule=milli*kilograms*meters.pow(2)/seconds.pow(2);

  cout << "mJ = " << milliJoule << endl;

  StaticPacketBuffer<milliJoule.size()> buffer1;
/*  DynamicPacketBuffer buffer1;
  buffer1.resize(milliJoule.size());*/

  auto in1=buffer1.begin();
  milliJoule.serialize(in1);

  cout << "Serialized: [" << buffer1.size() << "]" << endl << buffer1 << endl;

  auto out1=buffer1.cbegin();
  Unit u;
  u.deserialize(out1);

  cout << "Deserialized: " << endl;
  cout << dec << u << endl;


  DataType<uint16_t, 1> dt({0}, milli*kilograms*meters/seconds.pow(2));

  cout << dt << endl;

  StaticPacketBuffer<dt.size()> buffer2;
/*  DynamicPacketBuffer buffer2;
  buffer2.resize(dt.size());*/

  auto in2=buffer2.begin();
  dt.serialize(in2);

  cout << "Serialized: ["  << buffer2.size() << "]" << endl << buffer2 << endl;

  auto out2=buffer2.cbegin();
  DataType<uint16_t, 1> ddt;
  ddt.deserialize(out2);

  cout << "Deserialized:" << endl;
  cout << dec << ddt  << endl;

  return 0;
}
