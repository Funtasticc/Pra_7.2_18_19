#include <fstream>
#include <iostream>
#include <string>
using namespace std;

const char seperator = ',';

struct Person { 
	string nachName;
	string vorName;
	string geburtsDatum;
};

string trimme(string myString)
{
	string trimmed;
	for (int i = 0; i < myString.length(); i++)
	{
		if (myString[i] != ' ') trimmed += myString[i];
	}
	return trimmed;
}

string spalteAbErstem(string eingabe, string &rest)
{
	string ersteSpalte;
	bool foundSeperator = false;
	for (int i = 0; i < eingabe.length(); i++)
	{
		if (eingabe[i] != seperator && foundSeperator == false) ersteSpalte += eingabe[i]; 
		else if (foundSeperator == false) foundSeperator = true;
		else rest += eingabe[i];
	}

	return trimme(ersteSpalte);
}

Person extrahiere_person(string eingabeZeile)
{
	Person p;
	string rest;
	p.nachName = spalteAbErstem(eingabeZeile,rest);
	eingabeZeile = rest;
	rest = "";
	p.vorName = spalteAbErstem(eingabeZeile, rest);
	eingabeZeile = rest;
	rest = "";
	p.geburtsDatum = spalteAbErstem(eingabeZeile, rest);
	return p;
}

int main()
{
	string zeile;
	string kurzText, langText;
	string tmplText;
	Person p;

	fstream daten("personendaten.txt", ios::in);

	while (getline(daten, zeile))
	{	
		p = extrahiere_person(zeile);
		kurzText += "<b>" + p.nachName + "</b>, " + p.vorName + "<br/>\n";
		langText += "<b>" + p.vorName + " " + p.nachName + "</b>, " + p.geburtsDatum + "<br/>\n";
	}
	daten.close();

	fstream tmpl("webseite.html.tmpl", ios::in);

	while (getline(tmpl, zeile))
	{
		for (int i = 0; i < zeile.length(); i++)
		{
			if (zeile[i] == '%') tmplText += kurzText;
			else if (zeile[i] == '$') tmplText += langText;
			else tmplText += zeile[i];
		}
		tmplText += '\n';
	}
	tmpl.close();

	fstream htmlOut;
	htmlOut.open("webseite.html", ios::out);
	htmlOut << tmplText;
	htmlOut.close();

	cout << tmplText;

	system("pause");
	return 0;
}