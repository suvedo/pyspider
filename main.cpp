#include "fuseki_client.h"
#include <iostream>
#include "triple.h"
#include <vector>

using namespace std;

int main()
{
	char* conf_path = "./fuseki.conf";
	FusekiClient fc;
	if(fc.init_client(conf_path) == 0){
		vector<string> subjects;
		subjects.push_back("朝伟");
		subjects.push_back("李小璐");
		vector<triple*> triples = fc.get_by_subs_like(subjects);
		for(auto iter = triples.cbegin(); iter != triples.cend(); iter ++)
		{
			cout<< (*iter)->get_subject()<<"\t"<<(*iter)->get_relation()<<"\t"<<(*iter)->get_object()<<endl;
			delete (*iter);
		}

//		cout<<fc.get_object("蔡卓妍","性别")<<endl;
//		cout<<fc.get_all_relations()<<endl;
/*		vector<triple*> triples = fc.get_all_triples();
		cout<<"total num:"<<triples.size()<<endl ;
		cout<<triples[0]->get_subject()<<endl;
		cout<<triples[0]->get_relation()<<endl;
		cout<<triples[0]->get_object()<<endl;
		//in case of memory leak
		for(auto iter = triples.cbegin(); iter != triples.cend(); iter++)
		{
			delete (*iter);
		}*/
	}
	return 0;
}
