
	#include "FirmwareComparison.h"
	#include "myView.cpp"
	#include <list>
	#include <iterator>
	
	using namespace std;
	//list<char*> lt;

	int main(int argc, char *argv[])
	{
		list<string> Nmach;
		list <string>::iterator Nmach_Iter;
		if (argc!=3)
		{
			cout<<"Argument error";
			
			return -1;
		};
//		system(("rm -r ./_"+string(argv[1])+".extracted").c_str());
//		system(("rm -r ./_"+string(argv[2])+".extracted").c_str());

		//system("rm -r ./NEqueF1");
		//system("rm -r ./NEqueF2");
		system("rm -r ./DifferentsFile");
		system("rm -r ./BinDiFW1");
		system("rm -r ./BinDiFW2");

//		system("mkdir ./NEqueF1");
//		system("mkdir ./NEqueF2");
		system("mkdir ./DifferentsFile");
		system("mkdir ./BinDiFW1");
		system("mkdir ./BinDiFW2");


		system(("binwalk -e " + string(argv[1])).c_str());
		system(("binwalk -e " + string(argv[2])).c_str());
		system("clear");

		cout<<"-->"<<endl;
       	Nmach = myView(true, ("./_"+string(argv[1])+".extracted").c_str(), ("./_"+string(argv[2])+".extracted").c_str(),true);
        
        for (Nmach_Iter = Nmach.begin();Nmach_Iter != Nmach.end();Nmach_Iter++)
        	cout <<"  "<< string(*Nmach_Iter) << endl;
        cout<<"<--"<<endl;
        cllist();

        Nmach = myView(true, ("./_"+string(argv[2])+".extracted").c_str(), ("./_"+string(argv[1])+".extracted").c_str(),false);
        for (Nmach_Iter = Nmach.begin();Nmach_Iter != Nmach.end();Nmach_Iter++)
        	cout <<"  "<< string(*Nmach_Iter) << endl;
 		
 		printDif();
		system(("rm -r ./_"+string(argv[1])+".extracted").c_str());
		system(("rm -r ./_"+string(argv[2])+".extracted").c_str());

}