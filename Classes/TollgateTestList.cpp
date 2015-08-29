#include "TollgateTestList.h"
#include "TollgateHelper.h"
#include "GameDirector.h"

using namespace cocos2d;
using namespace cocos2d::extension;


bool getInforByIdx(int idx, int& seasonId, int& sectionId, Section::Difficulty& diffi, string& difDsc)
{
	int temp = 0;
	const vector<Season> *vec = TollgateHelper::getHelper()->getAllTollgate();
	for (int i=0,count=vec->size(); i<count; ++i)
	{
		for (int j=0,sectionNum=vec->at(i).sectionList.size(); j<sectionNum; ++j)
		{
			if (idx < temp + vec->at(i).sectionList[j].levels.size()){
				seasonId = vec->at(i).seasonId;
				sectionId = vec->at(i).sectionList[j].sectionId;
				if (idx-temp == 0) diffi = Section::eNormal;
				else if (idx-temp == 1) diffi = Section::eDifficult;
				else CCASSERT(false, "");
				difDsc = vec->at(i).sectionList[j].levels[idx-temp].lvDescribe;
				return true;
			}
			temp += vec->at(i).sectionList[j].levels.size();
		}
	}
	CCASSERT(false, "");
	return false;
}

bool TollgateList::init()
{
	TableView* tableView = TableView::create(this, Size(180, 480));
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	tableView->setPosition(Vec2(0,0));
	tableView->setDelegate(this);
	tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	this->addChild(tableView);
	tableView->reloadData();

	return true;
}


void TollgateList::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
	CCLOG("cell touched at index: %ld", cell->getIdx());

	int seasonId,sectionId;
	Section::Difficulty dif;
	string dsc;
	getInforByIdx(cell->getIdx(),seasonId,sectionId,dif,dsc);
	GameDirector::getDirector()->loadingToRunningScene(RunningMode::tollgate,seasonId,sectionId,dif);
}
cocos2d::Size TollgateList::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	return Size(180, 80);
}
ssize_t TollgateList::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
	int count = 0;
	const vector<Season> * vec = TollgateHelper::getHelper()->getAllTollgate();
	for (int i=0,c=vec->size(); i<c; ++i)
	{
		for (size_t j=0; j<vec->at(i).sectionList.size(); ++j)
		{
			count += vec->at(i).sectionList[j].levels.size();
		}
	}
	return count;
}
cocos2d::extension::TableViewCell* TollgateList::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	TableViewCell *cell = table->dequeueCell();
	if (!cell) {
		cell = new TableViewCell();
		cell->autorelease();
	}
	else
	{
		cell->removeAllChildren();
	}
	if (idx < 0 || idx >= this->numberOfCellsInTableView(table))
		return cell;

	int seasonId,sectionId;
	Section::Difficulty dif;
	string dsc;
	getInforByIdx(idx,seasonId,sectionId,dif,dsc);

	char buf[50];
	sprintf(buf, "%d--%d %s", seasonId, sectionId, dsc.c_str());
	Label * label = Label::createWithSystemFont(buf, "Arial", 36);
	cell->addChild(label);
	label->setColor(Color3B::BLACK);
	label->setPosition(this->tableCellSizeForIndex(table,idx)/2);

	return cell;
}
