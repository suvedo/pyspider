#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;

typedef unsigned char BYTE;  
   
    inline BYTE toHex(const BYTE &x)  
    {  
        return x > 9 ? x -10 + 'A': x + '0';  
    }  
  
    inline BYTE fromHex(const BYTE &x)  
    {  
        return isdigit(x) ? x-'0' : x-'A'+10;  
    }  
   
    inline string URLEncode(const string &sIn)  
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
    int main(int argc, char** argv)
    {
        string test_str1 = "http://www.baidu.com/haha#老同学";
 	string test_str2 = URLEncode(test_str1);
	std::cout<<test_str2<<endl; 
	return 0;
    }
