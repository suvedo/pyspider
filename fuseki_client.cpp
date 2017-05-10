#include "client_http.h"
#include <iostream>
#include <boost/regex.hpp>
#include <unordered_set>
#include <fstream>
#include "fuseki_client.h"

using namespace std;

typedef SimpleWeb::Client<SimpleWeb::HTTP> HttpClient;

BYTE FusekiClient::toHex(const BYTE &x)  
{  
	return x > 9 ? x -10 + 'A': x + '0';  
}

string FusekiClient::URLEncode(const string &sIn)  
{
        string sOut;  
        for( size_t ix = 0; ix < sIn.size(); ix++ )  
        {        
        	BYTE buf[4];  
        	memset( buf, 0, 4 );  
        	if( isalnum( (BYTE)sIn[ix] ) )  
        	{        
        		buf[0] = sIn[ix];  
        	}
		else  
        	{  
        		buf[0] = '%';  
               		buf[1] = toHex( (BYTE)sIn[ix] >> 4 );  
                	buf[2] = toHex( (BYTE)sIn[ix] % 16);  
        	}  
        	sOut += (char *)buf;  
    	}  
        return sOut;  
}

string FusekiClient::get_object(char *subject, char *relation)
{
	HttpClient client(this->host_name +":"+ this->port);
	char *s = subject;
	char *r = relation;

	string  query = "/"+this->service_name+"?query=select+%3Fo+where+%7B%3Chttp%3A%2F%2Fwww.hobot.com%2Frenwu%23"
		+ URLEncode(s)
		+ "%3E+%3Chttp%3A%2F%2Fwww.hobot.com%2Frenwu%23"
		+ URLEncode(r)
		+ "%3E+%3Fo%7D";

	auto r2 = client.request("GET", query);
	istream &stream = r2->content;

	string temp;
	string result;
	while (stream >> temp) {
		result += temp;
	}
	if (result.find("<literal>") != std::string::npos && result.find("</literal>") != std::string::npos) {

		result.erase(0, result.find("<literal>") + 9);
		result.erase(result.find("</literal>"), result.length());

		return result;
	}
	return "";
}

string FusekiClient::get_all_relations()
{
	HttpClient client(this->host_name + ":" + this->port);
	string  query = "/"+this->service_name+"?query=select+%3Fr+where+%7B%3Fs+%3Fr+%3Fo%7D";

	auto r2 = client.request("GET", query);
	istream &stream = r2->content;
	string temp;
	string result;
	unordered_set<string> rset;
	while (stream >> temp) {
		if (temp.find("http://www.hobot.com/renwu#") != std::string::npos && temp.find("</uri>") != std::string::npos) {
			temp.erase(0, temp.find("http://www.hobot.com/renwu#") + 27);
			temp.erase(temp.find("</uri>"), temp.length());
			if (rset.find(temp) == rset.end()) {
				rset.insert(temp);
				result += (temp + "\n");
			}
		}
	}
	return result;

}

vector<triple*> FusekiClient::get_by_subs_like(vector<string> subjects)
{
	vector<triple*> triples;
	for(auto iter = subjects.cbegin(); iter != subjects.cend(); iter++)
	{
		vector<triple*> tmp =  get_by_sub_like(*iter);
		triples.insert(triples.end(),tmp.begin(),tmp.end());
	}
	return triples;
}

vector<triple*> FusekiClient::get_by_sub_like(string subject)
{
	vector<triple*> triples;
	HttpClient client(this->host_name +":"+ this->port);
	string  query = "/"+this->service_name+"?query=select+%3Fs+%3Fr+%3Fo+where+%7B%3Fs+%3Fr+%3Fo+FILTER+regex(str(%3Fs)%2C\".*?"
		+ URLEncode(subject)
		+ ".*?\")%7D";
	auto r2 = client.request("GET", query);
	istream &stream = r2->content;

	string temp;
//	string result;

	string s = "", r = "", o = "";

	while (stream >> temp) {
		if (temp == "name=\"s\">")
		{
			if (stream >> temp)
			{
				if (temp.find("http://www.hobot.com/renwu#") != std::string::npos && temp.find("</uri>") != std::string::npos) {
					temp.erase(0, temp.find("http://www.hobot.com/renwu#") + 27);
					temp.erase(temp.find("</uri>"), temp.length());
					s = temp;
				}
			}
		}
		else if (temp == "name=\"r\">")
		{
			if (stream >> temp)
			{
				if (temp.find("http://www.hobot.com/renwu#") != std::string::npos && temp.find("</uri>") != std::string::npos) {
					temp.erase(0, temp.find("http://www.hobot.com/renwu#") + 27);
					temp.erase(temp.find("</uri>"), temp.length());
					r = temp;
				}
			}
		}
		else if (temp == "name=\"o\">")
		{
			string str = "";
			while (stream >> temp)
			{
				str += temp;
				if (temp.find("</literal>") != std::string::npos) break;
			}
			if (str.find("<literal>") != std::string::npos && str.find("</literal>") != std::string::npos) {

				str.erase(0, str.find("<literal>") + 9);
				str.erase(str.find("</literal>"), str.length());
				o = str;
			}
			triple* t = new triple(s, r, o);
			triples.push_back(t);
			s = r = o = "";
		}
	}
	return triples;
}

vector<triple*> FusekiClient::get_all_triples()
{
	HttpClient client(this->host_name + ":" + this->port);
	string  query = "/" + this->service_name + "?query=select+%3Fs+%3Fr+%3Fo+where+%7B%3Fs+%3Fr+%3Fo%7D";

	auto r2 = client.request("GET", query);
	istream &stream = r2->content;

	string temp;
	string result;
	string s = "", r = "", o = "";
	vector<triple*> triples;
	while (stream >> temp) {
		if (temp == "name=\"s\">")
		{
			if (stream >> temp)
			{
				if (temp.find("http://www.hobot.com/renwu#") != std::string::npos && temp.find("</uri>") != std::string::npos) {
					temp.erase(0, temp.find("http://www.hobot.com/renwu#") + 27);
					temp.erase(temp.find("</uri>"), temp.length());
					s = temp;
				}
			}
		}
		else if (temp == "name=\"r\">")
		{
			if (stream >> temp)
			{
				if (temp.find("http://www.hobot.com/renwu#") != std::string::npos && temp.find("</uri>") != std::string::npos) {
					temp.erase(0, temp.find("http://www.hobot.com/renwu#") + 27);
					temp.erase(temp.find("</uri>"), temp.length());
					r = temp;
				}
			}
		}
		else if (temp == "name=\"o\">")
		{
			string str = "";
			while (stream >> temp)
			{
				str += temp;
				if (temp.find("</literal>") != std::string::npos) break;
			}
			if (str.find("<literal>") != std::string::npos && str.find("</literal>") != std::string::npos) {

				str.erase(0, str.find("<literal>") + 9);
				str.erase(str.find("</literal>"), str.length());
				o = str;
			}
			triple* t = new triple(s, r, o);
			triples.push_back(t);
			s = r = o = "";
		}
	}
	return triples;
}

int FusekiClient::init_client(const char* conf_path)
{
	map<string, string> conf;
	string line;
	ifstream ifs(conf_path);
	if (ifs.is_open())
	{
		while (getline(ifs,line))
		{
			string key = "";
			string value = "";
			size_t pos = line.find("=");
			if ( pos != string::npos)
			{
				key = line.substr(0, pos);
				value = line.substr(pos + 1);
			} else {
				cout << "file content error!";
				cout << "initialazation failed!" << endl ;
				return -1;
			}
			if (key != "" && value != "") 
			{
				conf.insert(pair<string, string>(key, value));
			} else {
				cout << "file content error!";
				cout << "initialazation failed!" << endl ;
				return -1;
			}
		}
		if (conf.find("hostname") != conf.end())
		{
			this->host_name = conf["hostname"];
		}
		if (conf.find("port") != conf.end())
		{
			this->port = conf["port"];
		}
		if (conf.find("servicename") != conf.end())
		{
			this->service_name = conf["servicename"];
		}
		cout << "After initialization:" << endl ;
		cout << "\t" << "hostname = " << this->host_name << endl;
		cout << "\t" << "port = " << this->port << endl;
		cout << "\t" << "servicename = " << this->service_name << endl ;
		return 0;
	}
	else 
	{
		cout << "cannot open conf file!" << endl ;
		cout << "initialazation failed!" << endl ;
		return -1;
	}
	

}
