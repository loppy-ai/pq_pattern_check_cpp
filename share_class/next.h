#pragma once
#include <iostream>
#include <string>
#include "size.h"
#include "element.h"

class Next
{
private:
	int next_entity[next_size] = { 0 };
	int *next;

protected:
	void setNext(const int*);

public:
	Next();
	~Next();
	int getNextElement(const int) const;
	void setNextElement(const int, const int);
	bool isNone(const int) const;

	void print() const;
};