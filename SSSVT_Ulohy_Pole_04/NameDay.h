#pragma once
#include <iostream>
#include <Windows.h>
#include <string>

namespace NameDay
{
	std::string getCountry();
	unsigned short getHours();
	unsigned short getMinutes();
	bool isNumeric(const std::string& str);
	void ShowNameDay(unsigned short u_hours, unsigned short u_minutes, const std::string& country_code);
	void Run();
};

