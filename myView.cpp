//#ifndef _VIEW_H
//#define  _VIEW_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <iostream>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <list>
#include <iterator>
#include <fstream>      
#include <iostream>
#include <iterator>
#define TEXT "txet"
using namespace std;

list<string> noMach;

int GL = 0;
list<string> P1;
list<string> P2;
list <string>::iterator P1_Iter;
list <string>::iterator P2_Iter;

string GLOBAL_PATH="";




void DiffFile(string path1, string path2)
{

	if ((path1!="") && (path2 != ""))
	{
		string dir_path ="";

		int i = path1.length() - 1;
		while(path1[i--]!='/');
		//while (path1[i++] != '.' )
		while(i<path1.length() - 1)
		{
			dir_path += path1[i+1];
			i++;
		}

	
	//system(("touch ./DifferentsFile/"+dir_path).c_str());
	system(("diff -y -B -b -w --suppress-common-lines " + path1 +" " + path2 + " " + ">> " + " ./DifferentsFile/"+dir_path+".txt").c_str());	
	

	}
}

unsigned int CRC(string path)
{
    const char* FName = path.c_str();
    unsigned int sum = 0;
    unsigned int x = 0;
    char NP;
    char* charBuf;
    ifstream in(FName,ios::binary);
    in.read((char*)&x,sizeof(x));
    sum = x;
    if(sum == 1179403647){
		system(("objcopy -O binary -I elf32-little --only-section=.text ./" + path + (" "+path+".text")).c_str());
				
		ifstream inNew((path+".text").c_str(),ios::binary);
		sum = 0;


    	while (inNew.read((char*)&x, sizeof(x)))
    	{
      		sum = (sum + x) % 4294967294;
    	} 
    	system(("rm "+path+".text").c_str());
    	inNew.close();
    	GL=0;
    }
    else
    { 	
    	sum = 0;
    	in.seekg(0, in.beg);
    	while (in.read(&NP, sizeof(NP)))
    	{
    		if ((NP!=' ') || (NP!='\n'))
    		{
    			
      			sum = (sum + int(NP)) % 4294967294;
    		}
    	} 
   		GL=1;
    }
    in.close();
    return sum;
}

void cllist()
{
  noMach.clear();
}

bool diffCRC(string path1, string path2)
{
    if (CRC(path1) == CRC(path2))
      return true;
    return false;
}


bool searchFile(bool r,const char* path, char filename[256])
{ 
  
  DIR* dir = opendir(path);
  if(!dir)
    return 1;

  struct dirent *rd;

  while((rd = readdir(dir)))
  {
    if(!strcmp(rd->d_name,".") || !strcmp(rd->d_name,".."))
    {
      continue;
    }
    struct stat entryInfo;
    char pathName[PATH_MAX+1];

    strncpy(pathName,path,PATH_MAX);
    
    strncat(pathName,"/",PATH_MAX);
    strncat(pathName,rd->d_name,PATH_MAX);
    

    if(!lstat(pathName,&entryInfo))
    {
    

      if(S_ISDIR(entryInfo.st_mode))
      {
      	
      	if (!strcmp(rd->d_name, filename))
      	{

      		closedir(dir);
      		return true;
      	}

        if(r)
        {
          if(searchFile(r, pathName, filename))
          {
          	closedir(dir);
      		return true;
          }
        }
      }else
      if(S_ISREG(entryInfo.st_mode))
      {
      	
      	if (!strcmp(rd->d_name, filename))
      	{
      		closedir(dir);
          GLOBAL_PATH = pathName;
      		return true;
      	}
      }else

      if(S_ISSOCK(entryInfo.st_mode))
      {

      	if (!strcmp(rd->d_name, filename))
      	{
      		closedir(dir);
      		return true;
      	}
      }
    }
    //Добавил проверку здесь, для предотвращения падения стека, подробности ниже
    else
    {
      exit(1);
    }

  }
  
  closedir(dir);
  return false;
}



list<string> myView(bool r,const char* path, const char* path2, bool diflag)
{


  char* retmas = (char*) calloc(200,sizeof(char*));
  int count=0;
  DIR* dir = opendir(path);
  if(!dir)
  	exit(1);
  
  struct dirent *rd;

  while((rd = readdir(dir)))
  {
  	
    if(!strcmp(rd->d_name,".") || !strcmp(rd->d_name,".."))
    {
      continue;
    }
   
    struct stat entryInfo;
    char pathName[PATH_MAX+1];

    strncpy(pathName,path,PATH_MAX);
    
    strncat(pathName,"/",PATH_MAX);
    strncat(pathName,rd->d_name,PATH_MAX);
    

    if(!lstat(pathName,&entryInfo))
    {
    

      if(S_ISDIR(entryInfo.st_mode))
      {
      	if (!searchFile(r, path2, rd->d_name))
      	{

      		noMach.push_back(rd->d_name);
      	}
      	

      	if(r)
        {
          myView(r,pathName,path2,diflag);
        }
       // cout << rd->d_name << endl;
      }else
      if(S_ISREG(entryInfo.st_mode))
      {
        GLOBAL_PATH = "";
      	if (!searchFile(r, path2, rd->d_name))
      	{
      		noMach.push_back(rd->d_name);
      	}
        else
        {
            if((!diffCRC(pathName, GLOBAL_PATH)) && (diflag == true) && (pathName!="") && (GLOBAL_PATH!=""))
            {
                P1.push_back(pathName);
                P2.push_back(GLOBAL_PATH);
                if(GL==1)
                {
                	DiffFile(pathName, GLOBAL_PATH);
                }
                else
                {
            		system (("cp "+ string(pathName) + " ./BinDiFW1").c_str());
  					system (("cp "+ GLOBAL_PATH + " ./BinDiFW2").c_str());

                }
                
            }
        }
      }else
      if(S_ISSOCK(entryInfo.st_mode))
      {
      	if (!searchFile(r, path2, rd->d_name))
      	{
      		noMach.push_back(rd->d_name);
      	}
      }
    }
    
    else
    {
      cout << rd->d_name << " " << strerror(errno) << endl;
      exit(1);
    }
  }
  closedir(dir);

  return noMach;
}

void printDif()
{
      cout<<"Different files:\n";
      for (P1_Iter = P1.begin(), P2_Iter=P2.begin();P1_Iter != P1.end();P1_Iter++,P2_Iter++)
      cout<<string(*P1_Iter)<<" and "<<string(*P2_Iter)<<endl;
}

