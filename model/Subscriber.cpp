#include "Subscriber.h"
#include <string>

Subscriber::Subscriber(std::string, int x, int y) {
	this->id = id;
	this->x = x;
	this->y = y;
}

void Subscriber::setId(std::string id) {
	this->id = id;
}

void Subscriber::setX(int x) {
	this->x = x;
}

void Subscriber::setY(int y) {
	this->y = y;
}

void Subscriber::setCoordinates(int x, int y) {
	this->x = x;
	this->y = y;
}

std::string Subscriber::getId() {
	return this->id;
}

int Subscriber::getX() {
	return this->x;
}

int Subscriber::getY() {
	return this->y;
}
