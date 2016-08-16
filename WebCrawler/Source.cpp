#include<iostream>
#include"Crawler.h"
using namespace std;




int main()
{

	//I HAVE DIVIDED THE ENTIRE FETECHED LINK INTO TWO CATEGORIES ONE IS FOR INTERNAL AND OTHER IS EXTERNAL
	//CRAWLER's JOB IS TO CRAWL ENTIRE WEBSITE EXPLORE INTERNAL LINKS AND THEN AFTER THAT GO FOR EXTERNAL LINKS
	CrawlEngineManager* manager = new CrawlEngineManager();
	manager->Initiate_First_Stage();

	
	return 0;
}