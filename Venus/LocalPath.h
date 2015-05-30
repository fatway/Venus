#pragma once

#include <map>
#include <vector>
#include <string>
#include <io.h>
#include <direct.h>
#include <ShlObj.h>

using namespace std;

typedef struct tagShortCut
{
	string fileName;
	string filePath;
}ShortCut;

// 本地资源表
class LocalPath
{
public:
	LocalPath(void);
	~LocalPath(void);

	// 初始化本地索引表
	void InitIndex();
	// 左匹配键值
	bool FindIndex(CString key, vector<CString> &matchs);
	// 精确获取值
	CString GetLocalPath(CString key);
	// 清空缓存记录
	void CleanIndex();

private:
	// 读取本地文件及信息类函数
	string GetExePath(void);
	void CheckFolderExist(string lnkpath);
	int endsWith(char s1[],char s2[]);
	void getFiles(string path, vector<ShortCut>& files);
	wchar_t* CharToWchar(const char* c);
	bool ReadShortcut(LPWSTR lpwLnkFile, LPSTR lpDescFile);

private:
	map<CString, CString> index;
	string strLocalPath;
};

