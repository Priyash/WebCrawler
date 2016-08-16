#include"Crawler.h"



void Crawler::crawl(urlNode* node)
{
	if (crawl_link_count == MAX_CRAWL_LINK)return;
	if (node->tag == "internal" || node->tag == "root")
	{
		if (node->depth == MAX_CRAWL_DEPTH)return;
		if (node->children.empty() && node->depth < MAX_CRAWL_DEPTH)
		{
			downloadPage(node, node->url);
			parseLink(node, node->url);
			crawl_link_count = 0;
		
		}
		for (auto i : node->children)
		{
			crawl(i);
		}
	}
	else
	{
		return;
	}
}




void Crawler::downloadPage(urlNode* node,string url)
{
	string ext = url.substr(url.find_last_of(".") + 1);
	
	wstring wurl = wstring(url.begin(), url.end());
	LPCWSTR lurl = wurl.c_str();
	wstring id = to_wstring(docID).c_str();
	if (ext==ex.MP4||ext==ex.MKV||ext==ex.JPG)
	{
		return;
	}
	else
	{
		p = filePath + id + L".html";
	}


	LPCWSTR path = p.c_str();
	HRESULT result = URLDownloadToFileW(NULL, lurl, path, 0, NULL);
	cout <<"id: "<<docID<< " Downloading " << string(wurl.begin(), wurl.end()) << endl;
	if (SUCCEEDED(result))
	{
		cout << "Download Complete " << endl;
		docID++;
		crawl_link_count++;
		writer << node->url << endl;
	}
	else
	{
		cout << "FILE DOWNLOAD ERROR " << endl;
	}
}

//THIS FUNCTION WILL PARSE ALL THE LINKS FOUND IN THE DOWNLOADED WEB PAGE
void Crawler::parseLink(urlNode* node,string url)
{
	ifstream reader;
	string spath = string(p.begin(),p.end());
	reader.open(spath.c_str(), ios::in);
	string line;
	depth = node->depth + 1;
	while (getline(reader, line))
	{
		extractURL(node, line);
	}
	reader.close();
	line.clear();
}


//THIS FUNCTION IS CHECK THE  DUPLICATE URL EXIST OR NOT 
bool Crawler::isExist(urlNode* _root,urlNode* node)
{
	
	if (_root->url == node->url)
	{
		return true;
	}
	for (auto i : _root->children)
	{
		if(isExist(i,node))return true;
	}

	return false;
}

string Crawler::getBaseUrl(urlNode* node)
{
	string str = node->url;
	if (str[0] == '/')
	{
		getBaseUrl(node->parent);
	}
	else
	{
		str = str.substr(str.find("//") + 2);
		str = "http://" + str.substr(0, str.find_first_of('/'));
		return str;
	}
	return "No Base URL Found";
}


//UTILITY FUNCTION FOR EXTRACTING THE DOMAIN NAME OF THE GIVEN URL
string Crawler::extractBaseUrl(string url)
{
	string n1 = url.substr(0, url.find_first_of("//") + 2);
	string n2 = url.substr(url.find_first_of("//") + 2);
	string n3 = n2.substr(0, n2.find_first_of("/"));
	string domain = n1 + n3;
	return domain;
}

//THIS MODULE WILL EXTRACT ALL THE VALID URLs FROM THE DOWNLOADED PAGE
void Crawler::extractURL(urlNode* node,string line)
{
	string reg = "<a\\s[^>]*href=\"([^\"]*?)\"[^>]*>";
	regex e(reg);
	smatch sm;
	string res;
	while (regex_search(line, sm, e))
	{
		int i = 0;
		for (auto s : sm)
		{
			i++;
			if (i % 2 == 0)
			{
				//cout << s << endl;
				string str = s.str();

				if (str.length()>0&&str[str.length() - 1] == '/')
				{
					str = str.substr(0, str.length() - 1);
				}
				if (str == "")continue;
				transform(str.begin(), str.end(),str.begin(), ::tolower);
				char* url = new char[str.length() + 1];
				strcpy(url, str.c_str());
				if (str[0] == '/'&&str.length() > 1&&str[0]!='#'&&str!="/")
				{

					//HAVE TO FIND THE BASE URL BY BACKTRACKING THE TREE
					string burl = getBaseUrl(node);
					string newURL = burl + str;
					char* s = new char[newURL.length() + 1];
					strcpy(s, newURL.c_str());

					if (!bloom->contain(s))
					{
						index++;
						crawl_link_count++;
						bloom->add(s);
						//HERE I HAVE TO PUT CONDITION NO MORE THAN 100 LINKS WILL BE STORED IN 
						//AS A CHILDREN NODEs
						if (crawl_link_count <= MAX_CRAWL_LINK)
						{
							node->children.push_back(new urlNode(index, burl + str, node, depth, "internal"));
						}
						delete url;
						delete s;
					}
				}
				else if (str[0]!='#'&&str!="/"&&str[0]!='?')
				{
					if (!bloom->contain(url))
					{
						index++;
						bloom->add(url);
						crawl_link_count++;
						if (str.find(root->url) != string::npos)
						{
							if (crawl_link_count <= MAX_CRAWL_LINK)
							{
								node->children.push_back(new urlNode(index, str, node, depth, "internal"));
							}
						}
						else
						{
							node->children.push_back(new urlNode(index, str, node, depth,"external"));
						}
					}
				}
			}
			else continue;
		}
		line = sm.suffix().str();
	}
}



void Crawler::fetchRootList()
{
	ifstream reader;
	reader.open(rootList.c_str(), ios::in);
	string line = "";
	while (getline(reader, line))
	{
		rooturlList.push_back(line);
	}
}





void Crawler::init()
{
	
	root = new urlNode(0, rooturlList[list_url_count], NULL, 0, "root");
	char* root_url = new char[root->url.length() + 1];
	strcpy(root_url, root->url.c_str());
	bloom->add(root_url);

}



//THIS MODULE WILL INITIATE THE CRAWLING

void Crawler::initialize()
{
	fetchRootList();
	init();
	list_url_count++;
}

void Crawler::processing()
{
	crawl();
}


bool Crawler::finished()
{

	delete root;

	//HAVE TO WRITE A MODULE WHICH WILL UPDATE THE NEWLY FOUNDED EXTERNAL URL INTO URL LIST

	return false;
}


void CrawlEngineManager::Initiate_First_Stage()
{
	int site_count = 0;
	/*while (site_count<MAX_SITE)
	{
		engine->initialize();
		engine->processing();
		engine->finished();
		site_count++;
		
	}*/


	engine->initialize();
	engine->processing();
	engine->finished();
}