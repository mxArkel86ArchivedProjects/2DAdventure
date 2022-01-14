#include "ConfigMgr.h"
#include <algorithm>

void saveconfig(wstring path, vector<GameObject *>& newcolliders, vector<LevelProp *>& newprops, vector<ColorRect *> newcolorrect, vector<GameObject *>& colliders, vector<LevelProp *>& props, vector<ColorRect *>& colorrect)
{
	ofstream myfile(path);
	if (myfile.is_open())
	{
		myfile << "--Collisions--\n";
		for(GameObject* o : newcolliders){
			Rect r = o->getRect();
			myfile<<to_string((int)r.left())<<","<<to_string((int)r.top())<<","<<to_string((int)r.right())<<","<<to_string((int)r.bottom())<<"\n";
			
			std::vector<GameObject *>::iterator position = std::find(colliders.begin(), colliders.end(), o);
			if (position != colliders.end()) // == myVector.end() means the element was not found
    		colliders.erase(position);
			delete o;
		}
		

		myfile << "--assets--\n";
		for(LevelProp* p : newprops){
			Rect r = p->rect();
			myfile<<p->res()<<","<<to_string((int)r.left())<<","<<to_string((int)r.top())<<","<<to_string((int)r.right())<<","<<to_string((int)r.bottom())<<","<<to_string(p->getZ())<<"\n";
			std::vector<LevelProp *>::iterator position = std::find(props.begin(), props.end(), p);
			if (position != props.end()) // == myVector.end() means the element was not found
    		props.erase(position);
			delete p;
		}

		myfile << "--colorrect--\n";
		for(ColorRect* p : newcolorrect){
			Rect r = p->rect();
			myfile<<p->getColor()<<","<<to_string((int)r.left())<<","<<to_string((int)r.top())<<","<<to_string((int)r.right())<<","<<to_string((int)r.bottom())<<"\n";
			std::vector<ColorRect *>::iterator position = std::find(colorrect.begin(), colorrect.end(), p);
			if (position != colorrect.end()) // == myVector.end() means the element was not found
    		colorrect.erase(position);
			delete p;
		}
		newcolliders.clear();
		newprops.clear();
		newcolorrect.clear();

		myfile.close();
	}
	return;
}

string category;
void readconfig(wstring path, vector<GameObject *>& colliders, vector<LevelProp *>& props, vector<ColorRect *>& colorrect)
{
	string line;
	ifstream myfile(path);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			
			if (starts_with(line, "["))
			{ // new category
				category = line.substr(1, line.length() - 2);
				continue;
			}
			if (category == "nul")
				continue;
			if (starts_with(line, "#") || line.length() == 0)
				continue;

			if(category=="collisions")
			{

				vector<string> SplitVec; // #2: Search for tokens
				split(SplitVec, line, is_any_of(","), token_compress_on);
				double x1 = std::stod(SplitVec[0]);
				double y1 = std::stod(SplitVec[1]);
				double x2 = std::stod(SplitVec[2]);
				double y2 = std::stod(SplitVec[3]);

				X::Point topleft = X::Point(min(x1, x2), min(y1, y2));
				X::Point bottomright = Point(max(x1, x2), max(y1, y2));

				colliders.push_back(new GameObject(topleft, bottomright, true));
			}else
			if(category=="assets")
			{

				vector<string> SplitVec; // #2: Search for tokens
				split(SplitVec, line, is_any_of(","), token_compress_on);
				string asset = SplitVec[0];
				double x1 = std::stod(SplitVec[1]);
				double y1 = std::stod(SplitVec[2]);
				double x2 = std::stod(SplitVec[3]);
				double y2 = std::stod(SplitVec[4]);
				double z = std::stod(SplitVec[5]);

				X::Point topleft = X::Point(min(x1, x2), min(y1, y2));
				X::Point bottomright = Point(max(x1, x2), max(y1, y2));

				props.push_back(new LevelProp(asset,topleft, bottomright, z));
			}else
			if(category=="colorrect")
			{

				vector<string> SplitVec; // #2: Search for tokens
				split(SplitVec, line, is_any_of(","), token_compress_on);
				string asset = SplitVec[0];
				double x1 = std::stod(SplitVec[1]);
				double y1 = std::stod(SplitVec[2]);
				double x2 = std::stod(SplitVec[3]);
				double y2 = std::stod(SplitVec[4]);

				X::Point topleft = X::Point(min(x1, x2), min(y1, y2));
				X::Point bottomright = Point(max(x1, x2), max(y1, y2));

				colorrect.push_back(new ColorRect(asset, topleft, bottomright));
			}
			else if(category=="player")
			{
				vector<string> SplitVec; // #2: Search for tokens
				split(SplitVec, line, is_any_of("="), token_compress_on);
				string key = SplitVec[0];
				string val = SplitVec[1];
				if (key._Equal("ipos"))
				{

					vector<string> SplitVec2; // #2: Search for tokens
					split(SplitVec2, val, is_any_of(","), token_compress_on);

					//double x1 = std::stod(SplitVec2[0]) * GRIDSIZE - SCREEN_BOUNDS.right() / 2 + PLAYER_SIZE;
					//double y1 = (std::stod(SplitVec2[1]) + 1) * GRIDSIZE - SCREEN_BOUNDS.bottom() / 2;
					//location = X::Point(x1, y1);
				}
			}
		}
	}
	myfile.close();
}