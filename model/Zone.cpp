#include "Zone.h"
#include <string>

Zone::Zone(unsigned int id, std::string name, int x, int y, int radius) {
	this->id = id;
	this->name = name;
	this->x = x;
	this->y = y;
	this->radius = radius;
}

void Zone::setId(unsigned int id) {
	this->id = id;
}

unsigned int Zone::getId() {
	return this->id;
}

void Zone::setName(std::string name) {
	this->name = name;
}

std::string Zone::getName() {
	return this->name;
}

void Zone::setX(int x) {
	this->x = x;
}

int Zone::getX() {
	return this->x;
}

void Zone::setY(int y) {
	this->y = y;
}

int Zone::getY() {
	return this->y;
}

void Zone::setRadius(int radius) {
	this->radius = radius;
}

int Zone::getRadius() {
	return this->radius;
}