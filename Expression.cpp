// Expression.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <bits/stdc++.h>
using namespace std;


string digits = "n0123456789 ";
int64_t p;
char operation;
string subexp, evalstring = "", buildstr;
string Expr;
int64_t leftnum, rightnum, minj, maxj;

void findtimes()
{
	operation = '#';  // NULL operation
	while ((p < subexp.length()) && (operation != '*'))
	{
		p++;
		operation = subexp[p];
	}
}

void findplus()
{
	operation = '#';  // NULL operation
	while ((p < subexp.length()) && (operation != '+'))
	{
		p++;
		operation = subexp[p];
	}
}

void findminus()
{
	operation = '#';  // NULL operation
	while ((p < subexp.length()) && (operation != '-'))
	{
		p++;
		operation = subexp[p];
	}
}

void findleftnum()
{
	int64_t i;
	i = p-1;
	char digit = ' ';
	string left;
	while ( (i >= 0) && (digits.find(digit) != string::npos) )
	{
		digit = subexp[i];
		i--;
	}
	if (i < 0) { i = 0; }
	else
	{
		i = i + 2;
	}
	left = subexp.substr(i, p - i);
	if (left[0] == 'n')
	{
		left[0] = '-';
	}
    leftnum = stoll(left);
	//cout << "left="<< left << "\n";
	minj = i;
	//cout << "leftnum = " << leftnum << "\n";
	//cout << "minj = " << minj << "\n";
}

void findrightnum()
{
	int64_t i;
	i = p + 1;
	char digit = ' ';
	string right;
	while ((i < subexp.length()) && (digits.find(digit) != string::npos))
	{
		digit = subexp[i];
		i++;
	}
	if (i >= subexp.length()) { i = subexp.length()-1; }
	else
	{
		i = i - 2;
	}
	right = subexp.substr(p + 1, i - p);
	if (right[0] == 'n')
	{
		right[0] = '-';
	}
	assert(right != "" && right != " ");
	rightnum = stoll(right);
	//cout << "right = " << right << "\n";
	maxj = i;
	//cout << "maxj = " << maxj << "\n";
	//cout << "rightnum = " << rightnum << "\n";
}


void makebuildstr()
{
	buildstr = "";
	if (minj > 0)
	{
		buildstr = subexp.substr(0, minj);
	}
	buildstr = buildstr + evalstring;
	if (maxj < subexp.length() - 1)
	{
		buildstr = buildstr + subexp.substr(maxj+1, subexp.length() - maxj);
	}

}



string BODMAS()
{
	string test;


	//cout << "finding * \n";
	p = 0;
	while (p < subexp.length())
	{
		findtimes();
		if (operation == '*')
		{
			findleftnum();
			findrightnum();
			evalstring = to_string(leftnum * rightnum);
			makebuildstr();
			p = minj;
			subexp = buildstr;
		//	cout << buildstr << "   press c <enter> to continue";
		//	cin >> test;
		}
		else
		{
			p++;
		}
	}
	p = 0;
	//cout << "finding - \n";
	while (p < subexp.length())
	{
		findminus();
		if (operation == '-')
		{
			findleftnum();
			findrightnum();
			evalstring = to_string(leftnum - rightnum);
			if (leftnum - rightnum < 0)
			{
				evalstring[0] = 'n';
			}
			makebuildstr();
			p = minj;
			subexp = buildstr;
		//	cout << buildstr << "   press c <enter> to continue";
		//	cin >> test;
		}
		else
		{
			p++;
		}
	}
	p = 0;
	//cout << "finding + \n";
	while (p < subexp.length())
	{
		findplus();
		if (operation == '+')
		{
			findleftnum();
			findrightnum();
			evalstring = to_string(leftnum + rightnum);
			makebuildstr();
			p = minj;
			subexp = buildstr;
		//	cout << buildstr << "   press c <enter> to continue";
		//	cin >> test;
		}
		else
		{
			p++;
		}
	}
	return subexp;
}

int main()
{
	
	freopen("input","r",stdin);
	cin >> subexp;

	//cout << "exp = " << subexp << "\n";
    BODMAS();
	if (subexp[0] == 'n')
	{
		subexp[0] = '-';
	}
	cout << "exp = " << subexp;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
