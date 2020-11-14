#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <algorithm>
using namespace std;
#include <d3dx9.h>
#include "./../High Performance Timer/HighPerformanceTimer.h"
#include <stdio.h>

class CDebugCSV
{
public:
	CDebugCSV(const char* filename,vector<const char*> names);
	~CDebugCSV(void);

	void writeLine(vector<D3DXVECTOR3> name);
private:
	CDebugCSV(void) {}
	void writeHeader(vector<const char*> name);

	fstream* debugFile; //the file to be saved to
	float start_time;

};

