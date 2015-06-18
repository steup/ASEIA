#include <KnowledgeGraph.h>

#include <iostream>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

using namespace std;
using namespace boost;

class  IncompleteCmdError : public std::exception {
	const char* msg;
	public:
		IncompleteCmdError(const string& desc) : msg(desc.c_str()){}
		virtual const char* what() const throw() { return msg; }
};

namespace po = boost::program_options;

string parseOptions(int argc, char** argv){
	po::options_description desc("commandline options");
	desc.add_options()
    ("help", "produce help message")
    ("akg", po::value<string>(), "aseia knowledge graph file");

	po::variables_map vm;
	po::store(parse_command_line(argc, argv, desc), vm);
	po::notify(vm);    

	if( !vm.count("akg") || vm.count("help")) {
		ostringstream o;
		o << desc;
    throw IncompleteCmdError(o.str());
	} else
		return vm["akg"].as<string>();
}

int main(int argc, char** argv){
	KnowledgeGraph g;
	try {
		g.import(parseOptions(argc, argv));
	}
	catch(const IncompleteCmdError& e){
		cerr << argv[0] << " " << e.what() << endl;
		return -1;
	}
	catch(const graph_exception& e){
		cerr << argv[0] << ": Error on parsing graphml: " << endl << e.what() << endl;
		return -1;
	}
	catch(const std::exception& e){
		cerr << argv[0] << ": Unknown exception occured" << endl << e.what() << endl;
		return -1;
	}

	cout << g << endl;

  cout << "Events:" << endl;
  for(const auto& event : g.events()) {
    cout << "\t" << event << endl;
  }

  cout << "Rules:" << endl;
  for(const auto& rule : g.rules())
    cout << "\t" << rule << endl;
 
  auto posIter = g.findEvent("Position");

  if(g.valid(posIter)) {
    cout << "Interesting rules for Position:" << endl;
    for(const auto& rule : (*posIter).incoming())
      cout << "\t" << rule.source() << endl;
	  return 0;
  }
}
