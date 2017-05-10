#ifndef FUSEKI_CLIENT_H_
#define FUSEKI_CLIENT_H_
#include <string>
#include <vector>
#include "triple.h"
#include <stdio.h>


using namespace std;

typedef wchar_t WCHAR;
typedef unsigned char BYTE;

class FusekiClient
{
public:
	int init_client(const char* conf_path);
	string get_object(char *subject, char *relation);
	//get all triples, do not forget to free the memory after finishing your work
	vector<triple*> get_all_triples();
	//get all distinct relations,return a string seperated by '\n' for each relation
	string get_all_relations();
	vector<triple*> get_by_subs_like(vector<string>);

private:
	string host_name;
	string port;
	string service_name;
	BYTE toHex(const BYTE &x);
	string URLEncode(const string &sIn);
	vector<triple*> get_by_sub_like(string);
};
#endif // !FUSEKI_CLIENT_H_

