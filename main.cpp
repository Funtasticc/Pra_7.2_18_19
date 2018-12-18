#include <iostream>
#include <string>
#include <fstream>
using namespace std;

// Strukturierter Datentyp
struct Person{
	string vorname, nachname, geburtsdatum;
};

// Trimmfunktion um die ungewollten leerstellen rauszunehmen 
string trimme(string s) // Diese funktion klappt nur wenn man sie außerhalb von anderen Funktionen, 
{ // wie globale Variablen definiert da sie sonst nur als Prototyp erkannt wird.
	string trimme; // eine Stringvariable wird definiert.
	for (int i = 0; i < s.length(); i++) // zählt mit i die länge der Zeichenkette s durch 
	{ // um als index genutzt zu werden damit er jedes zeichen von der Zeichenkette s überprüfen kann.
		if (s[i] != ' ') // schließlich prüft er die Zeichenkette falls an der Stelle i in der Z.kette keine Leerstell ' ' vorkommt,
			trimme += s[i]; // nimmt er das Zeichen an der besagten Stelle und fügt es in die Variable trimme rein alles andere beachtet er nicht
	}
	return trimme; // nun gibt er trimme aus, damit man halt diese gesammte Funktion als trimme(irgendwas) später im main benutzen kann.
}

// Funktion zum umdrehehn der Namen 
void spalte_ab_erstem(string& eingabezeile, char zeichen, string& erster_Teil, string& zweiter_Teil) // hier einige Stringvariablen mit & (Referenzparameter) um
{ // die Werte direkt an den Aufrufer zu senden.
	zeichen = ',';	// Neuzuweisung für die Var. zeichen mit dem Wert Komma.
	bool zeichenGefunden = false;	// Schaltung ist auf falsch,
	for (unsigned int i = 0; i < eingabezeile.length(); i++) // durchläuft die länge d.Zeichenkette, jedoch soll er einen Wert vor der nullterminierung aufhören,
	{ // deswegen das i < eingabezeile.length() sonst müsste man i <= eingabezeile.length() - 1 schreiben, damit er keinen Feheler ausgibt.
		if (eingabezeile[i] == zeichen && !zeichenGefunden) // falls die Z.kette an pos. i gleich dem Zeichen ist, und die Schaltung gleichzeitig auf falsch ist,
			zeichenGefunden = true;	// setzt er sie auf wahr, damit er im nächstem Schritt prüfen kann ob ein Komma bereits vorkam.
		else if (zeichenGefunden)	// wenn die Schaltung wahr ist,
			zweiter_Teil += eingabezeile[i]; // nimmt er alle Zeichen aus eingab. für pos.i in den zweiten_Teil da ja bereits ein Komma gefunden wurde,
		else // andernfalls hatte er kein Komma gefunden
			erster_Teil += eingabezeile[i];	// und setzt deswegen alles von eing. für pos.i in den erstenTeil.
	}
}

Person extrahiere_person(string eingabezeile) // er nimmt die Zeichenkette oder Personendaten aus der Variablen ,,eingabezeile" welche im main() ausgelesen wurden anhand fstream
{
	Person p; // hier greift er auf das p von struct zu, welche die Personendaten entahlten
	string rest; // diese rest braucht er um schlißlich den rest der Zeichenkette abzuspeichern
	spalte_ab_erstem(eingabezeile, ',', p.nachname, rest);	// nun wendet er die Funktion spalte_ab_erstem an, auf die Zeichenkette die er rausgelesen hat aus main
	spalte_ab_erstem(rest, ',', p.vorname, p.geburtsdatum); // hier wendet er die funktion nochmal an um Geburtstag hinten ausgebenzulassen.
	return p; // nun gibt er schließlich die neuangeordneten Daten in p wieder aus.
}

// Hauptprogramm
int main()
{
	string eingabezeile, kurzText, langText, zwichenspeicherText;
	fstream Datendatei("personendaten.txt", ios::in); 
	while (getline(Datendatei, eingabezeile)) 
	{
		Person person = extrahiere_person(eingabezeile); // wendet er die funktion extrahier.... an und speichert alles in die person und
		kurzText += "<b>" + trimme(person.nachname) + "</b>, " + trimme(person.vorname) + "<br/>\n";// das (Bold) = <b></b> und (Breakline) <br/> sind HTML Befehle,
		// für Fettschrift und Zeilenumbruch also Enter.
		langText += "<b>" + trimme(person.vorname) + " " + trimme(person.nachname) + "</b>, " + trimme(person.geburtsdatum) + "<br/>\n"; // greift hier auf die getrimmten 
	} // struktur Daten zu, um alles ohne leerstelle auszugeben, es müssen aber zwichen den Worten auch leerstellen sein deswegen hab ich manuell " " (Leer) im cout eingefügt. 
	Datendatei.close(); 

	fstream Datendatei_zugriff_2("webseite.html.tmpl", ios::in); // nun öffnet er zum 2ten mal erst um zu lesen
	while (getline(Datendatei_zugriff_2, eingabezeile)) // hier wieder solange er in die Variable eingabezeile  einliest, 
	{
		for (int i = 0; i < eingabezeile.length(); i++) // zählt er wieder hoch
		{
			if (eingabezeile[i] == '%') // und prüft ob an der Stelle i im eingabe... das Zeichen % vorkommt
				zwichenspeicherText += kurzText; // falls ja Speichert er den oben erstellten kurzText(Zeile 56) und seine Werte in den zwichenspeicher
			else if (eingabezeile[i] == '$') 
				zwichenspeicherText += langText; // falls aber das $ Zeichen vorkommt Speichert er den inhalt von langText in den zwich...
			else 
				zwichenspeicherText += eingabezeile[i]; // ansonsten Speichert er in inhalt von eingabezeile[i] in den zwichenspeicher. Um das Template usw auszugeben, sonst wäre nur Liste da
		}
		zwichenspeicherText += '\n'; 
	}
	Datendatei_zugriff_2.close(); 

	fstream HTML("webseite.html", ios::out); // ab hier erstellt er zum schreiben (anhand des ios::out befehls) die html datei und
	HTML << zwichenspeicherText; // und übernimmt alle Werte und den gesamten Inhalt von zwichenspeicherText in das html dokument
	HTML.close(); // nun Schließt er diese Datei auch wieder.

	cout << zwichenspeicherText; // hier kann man sich das halt auch ausgeben lassen um zu sehen was er übernommen hat.

	system("PAUSE");
	return 0;
}
