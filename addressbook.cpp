#include<iostream>
#include <cstdlib>
#include<string>
#include<cstring>
#include <fstream>
#include <stdlib.h>
#include <Windows.h>
using namespace std;
ofstream out("out.txt");
int level;
string GbkToUtf8(const char* src_str)//GBK转换到UTF - 8
{
	int len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, src_str, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	string strTemp = str;
	if (wstr) delete[] wstr;
	if (str) delete[] str;
	return strTemp;
}
string Utf8ToGbk(const char* src_str)//UTF - 8转换到GBK
{
	int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	return strTemp;
}
string put_name(string a)
{
	unsigned int i;
	for (i = 0; i < a.size() - 1; i++)//提取名字
	{
		if (a[i] == ',')
		{
			break;
		}
	}
	out << '"' << "姓名" << '"' << ':' << '"' << a.substr(0, i) << '"' << ',' << endl;
	a.erase(0, i + 1);
	return a;
}
string put_phonenumber(string a)
{
	unsigned int i, begin = 0, count = 0;
	int mark = 0;
	for (i = 0; i < a.size() - 1; i++)//提取电话号码
	{
		if (a[i] >= '0'&&a[i] <= '9')
		{
			count++;
			if (mark == 0)
			{
				begin = i;
				mark = 1;
			}
		}
		else
		{
			count = 0;
			mark = 0;
		}
		if (count == 11)
		{
			out << '"' << "手机" << '"' << ':' << '"' << a.substr(begin, 11) << '"' << ',' << endl;
			a.erase(begin, 11);
		}
	}
	return a;
}
bool is_zhixiashi(string a)
{
	int t = 0, check = 0;
	unsigned int i;
	string key = "市";
	string zhixiashi[4] = { "北京","天津","上海","重庆" };
	for (i = 0; i < a.size() - 1; i += 2)
	{
		if (check == 1)
			break;
		while (t < 4)
		{
			if (a.substr(i, 4) == zhixiashi[t].substr(0, 4))
			{
				out << '"' << zhixiashi[t] << '"' << ',' << endl;
				out << '"' << zhixiashi[t] + key << '"' << ',' << endl;
				check = 1;
				break;
			}
			t++;
		}
	}
	if (check == 1)
		return true;
	return false;
}
string put_province(string str1)
{
	string province;
	string str2;
	string key = "省";
	unsigned int i;
	int check = 0;

	for (i = 0; i < str1.size() - 1; i += 2)
	{
		if (check == 1)
			break;
		ifstream p("province.txt");
		while (!p.eof())
		{
			getline(p, str2);
			if (str1.substr(i, 4) == str2.substr(0, 4))
			{
				province = str2;
				int l = str2.size() - 2;
				i += l;
				if (str1.substr(i, 2) != key)
					i -= 2;
				check = 1;
				break;
			}
		}
		p.close();
	}
	out << '"';
	if (!province.empty())
		out << province;
	out << '"' << ',' << endl;
	if (check == 1)
		str1.erase(0, i);
	return str1;
}
string put_city(string str1)
{
	string city;
	string str2;
	string key = "市";
	unsigned int i;
	int check = 0;
	int t = 0;
	for (i = 0; i < str1.size() - 1; i += 2)
	{
		if (check == 1)
			break;
		ifstream ci("city.txt");
		while (!ci.eof())
		{
			getline(ci, str2);
			if (str1.substr(i, 4) == str2.substr(0, 4))
			{
				city = str2 + key;
				int l = str2.size();
				i += l;
				if (str1.substr(i, 2) != key)
					i -= 2;
				check = 1;
				break;
			}
		}
		ci.close();
	}
	out << '"';
	if (!city.empty())
		out << city;
	out << '"' << ',' << endl;
	if (check == 1)
		str1.erase(0, i);
	return str1;
}
string put_county(string a)
{
	string county;
	string key[2] = { "县","区" };
	unsigned int i;
	int check = 0;
	for (i = 0; i < a.size() - 1; i += 2)
	{
		if (a.substr(i, 2) == key[0])
		{
			county = a.substr(0, i + 2);
			check = 1;
			break;
		}
		if (a.substr(i, 2) == key[1])
		{
			county = a.substr(0, i + 2);
			check = 1;
			break;
		}
	}
	if (check == 1)
		a.erase(0, i + 2);
	out << '"';
	if (!county.empty())
		out << county;
	out << '"' << ',' << endl;
	return a;
}
string put_town(string a)
{
	string town;
	string key[3] = { "乡","镇","街道" };
	unsigned int i;
	int check = 0;
	for (i = 0; i < a.size() - 1; i += 2)
	{
		if (a.substr(i, 2) == key[0])
		{
			i += 2;
			town = a.substr(0, i);
			check = 1;
			break;
		}
		if (a.substr(i, 2) == key[1])
		{
			i += 2;
			town = a.substr(0, i);
			check = 1;
			break;
		}
		if (a.substr(i, 4) == key[2])
		{
			i += 4;
			town = a.substr(0, i);
			check = 1;
			break;
		}
	}
	if (check == 1)
		a.erase(0, i);
	out << '"';
	if (!town.empty())
		out << town;
	out << '"' << ',' << endl;
	return a;
}
string put_road(string a)
{
	string road;
	string key[4] = { "路","街" ,"巷","村" };
	unsigned int i;
	int check = 0;
	for (i = 0; i < a.size() - 1; i += 2)
	{
		if (a.substr(i, 2) == key[0])
		{
			road = a.substr(0, i + 2);
			check = 1;
			break;
		}
		if (a.substr(i, 2) == key[1])
		{
			road = a.substr(0, i + 2);
			check = 1;
			break;
		}
		if (a.substr(i, 2) == key[2])
		{
			road = a.substr(0, i + 2);
			check = 1;
			break;
		}
		if (a.substr(i, 2) == key[3])
		{
			road = a.substr(0, i + 2);
			check = 1;
			break;
		}
	}
	if (check == 1)
		a.erase(0, i + 2);
	out << '"';
	if (!road.empty())
		out << road;
	out << '"' << ',' << endl;
	return a;
}
string put_number(string a)
{
	string number;
	string key = "号";
	unsigned int i;
	int check = 0;
	for (i = 0; i < a.size() - 1; i += 2)
	{
		if (a.substr(i, 2) == key)
		{
			number = a.substr(0, i + 2);
			check = 1;
			break;
		}
	}
	if (check == 1)
		a.erase(0, i + 2);
	out << '"';
	if (!number.empty())
		out << number;
	out << '"' << ',' << endl;
	return a;
}
void put_detail(string str1)
{
	string detail;
	if (level == 1)
	{
		detail = str1.substr(0, str1.size() - 1);
	}
	else
	{
		str1 = put_road(str1);
		str1 = put_number(str1);
		detail = str1.substr(0, str1.size() - 1);
	}
	out << '"';
	if (!detail.empty())
		out << detail;
	out << '"' << endl;
}
void put_address(string a)
{
	string str1, str2;
	string key = "市";
	int l = 0;
	out << '"' << "地址" << '"' << ':' << '[' << endl;
	if (!is_zhixiashi(a))
	{
		a = put_province(a);
		a = put_city(a);
	}
	else
	{
		if (a.substr(4, 2) == key)
			a.erase(0, 6);
		else
			a.erase(0, 4);
	}
	a = put_county(a);
	a = put_town(a);
	put_detail(a);
	out << ']' << endl << "}";
}
void put_information(string a)
{
	out << "{";
	int i = 0;
	while (a[i] != '!')
	{
		i++;
	}
	level = (int)a[i - 1] - 48;//提取难度等级
	a.erase(0, i + 1);
	a = put_name(a);
	a = put_phonenumber(a);
	put_address(a);
}
int main(int argc, char** argv)
{
	string infor;
	ifstream in(argv[1]);
	out << '[' << endl;
	while (!in.eof())
	{
		getline(in, infor);
		infor = Utf8ToGbk(infor.c_str());
		put_information(infor);
		if (in.peek() != EOF)
			out << ',' << endl;
	}
	in.close();
	out << endl << "]";
	out.close();
	ifstream c("out.txt");
	ofstream e(argv[2]);
	while (!c.eof())
	{
		getline(c, infor);
		infor = GbkToUtf8(infor.c_str());
		e << infor << endl;
	}
	c.close();
	e.close();
	return 0;
}