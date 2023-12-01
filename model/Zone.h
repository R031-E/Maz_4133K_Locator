#pragma once
#include <string>

class Zone {
private:
	unsigned int id;
	std::string name;
	int x;
	int y;
	int radius;

public:
	Zone(unsigned int id, std::string name, int x, int y, int radius);
	void setId(unsigned int id);
	unsigned int getId();
	void setName(std::string name);
	std::string getName();
	void setX(int x);
	int getX();
	void setY(int y);
	int getY();
	void setRadius(int radius);
	int getRadius();
};