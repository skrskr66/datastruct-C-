#pragma once
#include "Common.h"

class SqlManager
{
public:
	SqlManager()
		:_db(nullptr)
	{}
	~SqlManager()
	{
		Close();
	}
	void Open(const string &path);//打开数据库
	void ExcutedSql(const string &sql);//执行sql语句
	void GetTable(const string &sql, int& row, int& col, char**& pResult);//得到表中数据
	void Close();//关闭数据库
private:
	sqlite3 *_db;
};

//RAII
class AutoGetTable
{
public:
	AutoGetTable(SqlManager& db_obj,const string& sql, int& row, int& col, char**& pResult)
	{
		db_obj.GetTable(sql, row, col, pResult);
		pp_obj = pResult;//指针帮忙释放！！地址同时被释放（想通）！！
	}
	~AutoGetTable()
	{
		//cout << "析构完成" << endl;
		sqlite3_free_table(pp_obj);
	}

	AutoGetTable(const AutoGetTable&) = delete;
	AutoGetTable& operator=(const AutoGetTable&) = delete;
private:
	//SqlManager db_obj;
	char** pp_obj;
};

/*数据管理模块
对数据进行管理
*/
class DataManager
{
public:

	static DataManager* GetInstance()
	{
		
		// datamgr.Init();

		return &datamgr;
	}
	void Init();//建表，连接，打开数据库
	void GetData(const string& path, std::set<string>& Data);//寻找某个目录下的所有文件
	void InsertData(const string& path, const string& Data);
	void DeleteData(const string& path, const string& Data);
	int OpenThing();
	void CloseThing();
	void CallBack();
	void Search(const string& key, vector<std::pair<string, string>>&doc_paths);
	void SetHighLight(const string& str, const string& key, string& prefix, string& highlight, string& suffix);
private:

	DataManager()
	{}
	static DataManager datamgr;
	SqlManager _dbmgr;
	std::mutex _mtx;
};
