#ifndef CRAWLER_H
#define CRAWLER_H
#include<iostream>
#include<algorithm>
#include<vector>
#include<string>
#include<fstream>
#include<Windows.h>
#include<urlmon.h>
#include<regex>
#include<unordered_map>
#include<utility>
#include"Bloom_Filter.h"
#include<queue>



//DONT FORGET TO URLMON.LIB INTO THE LINKER SECTION OTHERWISE IT WILL GIVE UNRESOLVE EXTERNAL ERROR

using namespace std;



class iCrawlEngine
{

public :
	iCrawlEngine()
	{

	}
	~iCrawlEngine()
	{

	}

	virtual void initialize() = 0;
	virtual void processing() = 0;
	virtual bool finished() = 0;

};


struct urlNode
{
	int id;
	string url;
	urlNode* parent;
	vector<urlNode*>children;
	int depth;
	string tag = "";
	urlNode(int id, string url, urlNode* parent,int depth,string tag)
	{
		this->id = id;
		this->url = url;
		this->parent = parent;
		this->depth = depth;
		this->tag = tag;
	}
};


struct Extension
{
	const string MP4 = "mp4";
	const string MKV = "mkv";
	const string JPG = "jpg";
};


class Crawler : public iCrawlEngine
{
	Extension ex;
	urlNode* root;
	wstring filePath;
	wstring p;
	int index = 0;
	int depth = 0;
	ofstream writer;
	Bloom* bloom;
	unsigned int docID = 0;
	vector<string>rooturlList;
	vector<string>externalLinkList;
	string rootList;
	const int MAX_CRAWL_LINK = 100;
	const int MAX_CRAWL_DEPTH = 5;
	int crawl_link_count = 0;
	int list_url_count;

public:
	Crawler()
	{
		filePath = L"C:\\Users\\PRIYASH\\Documents\\Visual Studio 2013\\Projects\\WebCrawler\\Data\\";
		writer.open("C:\\Users\\PRIYASH\\Documents\\Visual Studio 2013\\Projects\\WebCrawler\\Data\\check.txt", ios::out);
		bloom = new Bloom();
		rootList = "urlRootList.txt";
		list_url_count = 0;
	
	}
	~Crawler()
	{
		writer.close();
		delete bloom;
		filePath.clear();
	}
	
	
	
	void initialize();
	void processing();
	bool finished();
	

private:
	void downloadPage(urlNode* node,string url);
	void parseLink(urlNode* node,string url);
	void extractURL(urlNode* node,string line);
	void crawl(urlNode* node);
	bool isExist(urlNode* root, urlNode* node);
	string getBaseUrl(urlNode*);

	void crawl(){ crawl(root); }
	string extractBaseUrl(string url);

	void fetchRootList();
	void init();

};





//MAIN CLASS FOR OTHER COMPONENT CLASS

class CrawlEngineManager
{
	iCrawlEngine* engine;
	const int MAX_SITE = 2;
public:
	CrawlEngineManager()
	{
		engine = new Crawler();
		
	}

	~CrawlEngineManager()
	{
		delete engine;
	}

	void Initiate_First_Stage();


};


	
#endif