#include<fstream>
#include<iostream>
#include<cstdlib>

using namespace std;

int main()
{
	ifstream fin;
	fin.open("NL6.txt");
	if(fin.fail())
	{
		cout << "Input file opening failed.\n";
		exit(1);
	}
    int a=1;
    int b=0;
    int count =0;
    int countol=0;
    std::ofstream myfile;
    myfile.open ("guards.txt");
	while((!fin.eof())&&(true))
	{
		string temp = "";
		getline(fin,temp);
        int fl=0;
        size_t pos=0;
        while (fl<=3){
            pos = temp.find('|',pos+1);
            if (fl==0){
                myfile<<temp.substr(0,pos)<<endl;
            }
            fl++;
        }
        if (pos<500){
        size_t bufpos =pos;
        pos = temp.find('|',pos+1);
        string flags = temp.substr(bufpos,pos-bufpos);
        if(flags.find('G')<=300){
            count++
            ;
            cout<<flags<<" "<< flags.find('G')<<endl;            
        }
        }
    countol++;
	}
    cout<<count;
    myfile.close();
	fin.close();

	return 0;
}