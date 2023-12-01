#pragma once
#include <string>

class Subscriber {
private:
	std::string id;
	int x;
	int y;
	
public:
	Subscriber(std::string id, int x, int y);
	void setId(std::string id);
	void setX(int x);
	void setY(int y);
	void setCoordinates(int x, int y);
	std::string getId();
	int getX();
	int getY();
};