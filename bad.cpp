// Expression.cpp : This file contains the 'main' function. Program execution begins and ends there.

//



#include <iostream>

#include <string>



using namespace std;





string digits = "0123456789";


string BODMAS(string subexp)

{

	int i,minj,maxj;

	string buildstr;

	while (find(subexp.begin(),subexp.end(), "*") != string::npos)
	{

		buildstr = "";

		int leftnum, rightnum;

		i = subexp.find("*");

		minj = i-1;

		do

		{

			if (digits.find(subexp[minj]) == string::npos)

			{

				break;

			}

			minj = minj - 1;

		} while (minj >= 0);

		if (minj < 0) { minj = 0; }

		//cout << "j="<<j << "  j-i+1=" << i-j << "\n";

		leftnum =  stoi(subexp.substr(minj, i - minj));

		maxj = i + 1;

		do

		{

			if (digits.find(subexp[maxj]) == string::npos)

			{

				break;

			}

			maxj = maxj + 1;

		} while (maxj <= subexp.length() -1 );

		rightnum = stoi(subexp.substr(i+1, maxj-i));

		if (minj > 0) 

		{

			buildstr = subexp.substr(0,minj+1);

		}

		buildstr = buildstr + to_string(leftnum * rightnum);

		if (maxj < subexp.length() - 1)

		{

			buildstr = buildstr + subexp.substr(maxj, subexp.length() - maxj);

		}

		subexp =  buildstr;
	}

	return subexp;

}



int main()

{
	freopen("input","r",stdin);
	string Expr;
	cin >> Expr;

    cout << "exp = " << BODMAS(Expr);

}


