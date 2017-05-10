#ifndef TRIPLE_H_
#define TRIPLE_H_

#include <string>

using namespace std;

class triple
{
public:
	triple()
	{
	}
	triple(string subject, string relation, string object)
	{
		this->subject = subject;
		this->relation = relation;
		this->object = object;
	}
	~triple()
	{
	}
	string get_subject()
	{
		return this->subject ;
	}
	string get_relation()
	{
		return this->relation ;
	}
	string get_object()
	{
		return this->object;
	}

private:
	string subject;
	string relation;
	string object;
};
/*
triple()
{
	this->subject = "";
	this->relation = "";
	this->object = "";
}


triple(string subject, string relation, string object)
{
	this->subject = subject;
	this->relation = relation;
	this->object = object;
}

~triple()
{
}
string get_subject()
{
	return this->subject;
}
string get_relation()
{
	return this->relation;
}
string get_object()
{
	return this->object;
}

*/
#endif // !TRIPLE_H_


