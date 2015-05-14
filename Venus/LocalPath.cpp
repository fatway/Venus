#include "StdAfx.h"
#include "LocalPath.h"
#include <shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")

// 构造
LocalPath::LocalPath(void)
{
	//InitIndex();
}

// 析构
LocalPath::~LocalPath(void)
{
	index.clear();
}


string LocalPath::GetExePath(void)
{
	char szFilePath[MAX_PATH + 1]={0};
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	(strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串
	string path = szFilePath;
	return path + "\\lnk\\";
}

// 文件夹是否存在，不存在，创建之
void LocalPath::CheckFolderExist(string lnkpath)
{
	if (PathFileExists(lnkpath.data()))
	{
		//存在
		int a = 1;
	}
	else //不存在
	{
		CreateDirectory(lnkpath.data(), NULL);
	}
}

/** 
 * 字符串s1是否以s2结束 
 */  
int LocalPath::endsWith(char s1[],char s2[]){  
	int len1 = strlen(s1);
	int len2 = strlen(s2);
	int i=len1-1,j=len2-1;

	if( len1 < len2 )
	{
		return 0;  
	}

	for( ; i>=0 && j>=0; i--,j-- )
	{
		if(s1[i]!=s2[j])
			return 0;
	}
	return 1;  
}

void LocalPath::getFiles(string path, vector<ShortCut>& files) {
	//文件句柄    
	long hFile = 0;    
	//文件信息
	struct _finddata_t fileinfo;    
	string p;  
  
	if ((hFile = _findfirst(p.assign(path).append("/*").c_str(),&fileinfo)) != -1)
	{    
		do{    
			if ((fileinfo.attrib & _A_SUBDIR)) {    
				// 跳过子目录
			}  
			else
			{
				// 只读取lnk文件
				if ( endsWith(fileinfo.name, ".lnk"))
				{
					ShortCut shortCut;
					char *lp = strstr(fileinfo.name, ".lnk");
					char tmp[MAX_PATH] = {0};
					if (lp != NULL)
					{
						memcpy(tmp, fileinfo.name, lp-fileinfo.name);
					}

					shortCut.fileName = tmp;
					shortCut.filePath = p.assign(path).append(fileinfo.name);
					//files.push_back(p.assign(path).append(fileinfo.name));  
					files.push_back(shortCut);
				}
			}
		}
		while(_findnext(hFile, &fileinfo) == 0);    
  
		_findclose(hFile);    
	}
}

wchar_t* LocalPath::CharToWchar(const char* c)  
{  
	int len = MultiByteToWideChar(CP_ACP,0,c,strlen(c),NULL,0);  
	wchar_t *m_wchar = new wchar_t[len+1];  
	MultiByteToWideChar(CP_ACP,0,c,strlen(c),m_wchar,len);  
	m_wchar[len]='\0';  
	return m_wchar;  
}

bool LocalPath::ReadShortcut(LPWSTR lpwLnkFile, LPSTR lpDescFile)
{
	bool bReturn = true;
	IShellLink *pShellLink;

	if(bReturn)
	{
		bReturn = (CoInitialize(NULL) == S_OK);
		if(bReturn)
		{
			bReturn = CoCreateInstance (CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
				IID_IShellLink, (void **)&pShellLink) >= 0;
			if(bReturn)
			{
				IPersistFile *ppf;
				bReturn = pShellLink->QueryInterface(IID_IPersistFile, (void **)&ppf) >= 0;
				if(bReturn)
				{
					bReturn = ppf->Load(lpwLnkFile, TRUE) >= 0;
					if(bReturn)
					{
						pShellLink->GetPath(lpDescFile, MAX_PATH, NULL, 0);
					}
					ppf->Release ();
				}
				pShellLink->Release ();
			}
			CoUninitialize();
		}
	}
	return bReturn;
}



// 初始化本地表
void LocalPath::InitIndex()
{
	index.clear();

	// 常用系统组件

	index[";calc"] = "calc";
	index[";cmd"] = "cmd";
	index[";mstsc"] = "mstsc";
	index[";notepad"] = "notepad";
	index[";regedit"] = "regedit";
	index[";taskmgr"] = "taskmgr";

	string path = GetExePath();

	// 确保lnk文件夹存在
	CheckFolderExist(path);

	CString lnk_explorer = "";
	lnk_explorer.Format("explorer /e, %s", path.data());
	index[";lnk"] = lnk_explorer;

	vector<ShortCut> lnkfiles;
	getFiles(path, lnkfiles);

	for (auto iter=lnkfiles.begin(); iter!=lnkfiles.end(); iter++)
	{
		/*
		// 从lnk中读取文件实际路径
		wchar_t *lnk = CharToWchar(iter->filePath.c_str());
		char szBuf[MAX_PATH];
		ReadShortcut(lnk, szBuf);
		index[iter->fileName.data()] = szBuf;
		delete lnk;
		*/
		index[iter->fileName.data()] = iter->filePath.c_str();
	}
	lnkfiles.clear();
}

// 左匹配键值，得到有效键值列表
bool LocalPath::FindIndex(CString key, vector<CString> &matchs)
{
	key.MakeLower();
	if (key.IsEmpty())
	{
		return false;
	}
	for (auto iter=index.begin(); iter != index.end(); iter++)
	{
		CString mapKey = iter->first;
		mapKey.MakeLower();
		if (! mapKey.Find(key, 0))
		{
			matchs.push_back(iter->first);
		}
	}
	return true;
}

// 精确获取值
CString LocalPath::GetLocalPath(CString key)
{
	return index[key];
}
