#include "MyPack.h"
#include "windows.h"
#include <queue>
void doPackNowDir(const char * fileName)
{
	MyPack ress;
	ress.load(fileName);
	std::queue<TCHAR*> Dirs;
	
	TCHAR * lpszPath = ".";
    TCHAR *tmp=new TCHAR[lstrlen(lpszPath)+1];
    lstrcpy(tmp,lpszPath);

    if(tmp[lstrlen(tmp)-1]=='//')
        tmp[lstrlen(tmp)-1]='/0';

    Dirs.push(tmp);

    TCHAR szFind[MAX_PATH];

    for(;!Dirs.empty();)
    {
        lstrcpy(szFind,Dirs.front());
        lstrcat(szFind, "//*.*"); // �������ļ�
        WIN32_FIND_DATA wfd;

        HANDLE hFind = FindFirstFile(szFind, &wfd);
		
        if (hFind != INVALID_HANDLE_VALUE) // ���û���ҵ������ʧ��
        {
			do
            {
                if (wfd.cFileName[0] == '.')
                    continue; // ����"." ".." ������Ŀ¼
                if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
					TCHAR szFile[MAX_PATH*2];

                    wsprintf(szFile, "%s/%s", Dirs.front(), wfd.cFileName);

                    TCHAR* tmp=new TCHAR[lstrlen(szFile)+1];
                    lstrcpy(tmp,szFile);
                    Dirs.push(tmp);
					printf("DO PACK DIR %s\n", szFile);
                }
                else 
                {
					TCHAR szFile[MAX_PATH*2];

					wsprintf(szFile, "%s/%s", Dirs.front(), wfd.cFileName);
					printf("DO PACK FILE %s\n", szFile);
					ress.addFile(&szFile[2]);
                }
            } while (FindNextFile(hFind, &wfd));
        }
        
        delete [] Dirs.front();
        Dirs.pop();
        FindClose(hFind); // �رղ��Ҿ��

    }
	ress.save();
}


int main(int argc,char * args[])
{
	if (argc == 2)
	{
		doPackNowDir(args[1]);
	}
	else
		doPackNowDir("test.bin");
}