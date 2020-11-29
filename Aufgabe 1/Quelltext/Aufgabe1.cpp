//
// Created by Christoph Waffler on 01.09.20.
//
#include <bits/stdc++.h>
#include <codecvt>
#include <locale>
using namespace std;

#define vs vector<string>
#define vvs vector<vector<string>>
#define vi vector<int>

// Konvertiert einen UTF-8 String zu UTF-16
std::u16string utf8_to_utf16(std::string const& utf8)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff,
            std::codecvt_mode::little_endian>, char16_t> cnv;
    std::u16string s = cnv.from_bytes(utf8);
    if(cnv.converted() < utf8.size())
        throw std::runtime_error("incomplete conversion");
    return s;
}

// Konvertiert einen UTF-16 String zu UTF-8
std::string utf16_to_utf8(std::u16string const& s)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff,
            std::codecvt_mode::little_endian>, char16_t> cnv;
    std::string utf8 = cnv.to_bytes(s);
    if(cnv.converted() < s.size())
        throw std::runtime_error("incomplete conversion");
    return utf8;
}

// Strings werden in einem Vector als Paramater übergeben.
// Die Methode gibt eine neuen Vector zurück, die all jene Strings enthält, 
// die den char c and der Stelle index besitzen.
vs filter_words(const vs& list, char c, int index) {
    vs new_list;
    for (const string& word : list) {
        u16string w = utf8_to_utf16(word);
        if (w[index] == c) {
            new_list.push_back(word);
        }
    }
    return new_list;
}

vs merge_vs(vs punctuations, vs without_punctuations) {
    vs new_one;
    for (int i = 0; i < punctuations.size(); ++i) {
        u16string w = utf8_to_utf16(without_punctuations[i]);
        u16string ww = utf8_to_utf16(punctuations[i]);
        u16string cut (ww.end()-(ww.size()-w.size()), ww.end());
        u16string s = w+cut;
        string new_string = utf16_to_utf8(s);
        new_one.push_back(new_string);
    }
    return new_one;
}

// aus dem gebenen 2D-Vector mit Strings, wird das mit übergebene Wort entfernt
// falls breakFirst = true ist, bricht die Suche in dem aktuellen Vector ab und der nächste wird bearbeitet
vvs remove_word_vvs(vvs liste, const string& word, bool breakFirst=false) {
    for (int i = 0; i < liste.size(); ++i) {
        vs list = liste[i];
        for (int j = 0; j < list.size(); ++j) {
            string wort = list[j];
            if (wort == word) {
                liste[i].erase(liste[i].begin()+j);

                if (breakFirst) {
                    return liste;
                }

                // Einmal gefunden --> Abbruch aus dieser Unterliste --> nächste Unterliste
                break;
            }
        }
    }
    return liste;
}

// Prüfen, ob die Vectoren im Vector leer sind
bool check_vvs_empty(vvs liste) {
    return all_of(liste.begin(), liste.end(), [](const vs& a) {return a.empty();});
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    cout << ">> Geben Sie in der 1. Zeile den lückenhaften Text ein \n";
    cout << "und in der 2. Zeile die dazugehörigen Wörter" << endl;

    // Einlesen der 1. und 2. Zeile
    string lineOne, lineTwo;
    getline(cin, lineOne);
    getline(cin, lineTwo);

    // Es werden zum einen die Lücken und die Lücken mit Satzzeichen gespeichert
    vs spaces, spaces_with_punctuations;

    // Vector zum Speichern aller Wörter
    vs words;


    // nun werden beide eingelesenen Zeilen verarbeitet
    istringstream iss1(lineOne);
    istringstream iss2(lineTwo);

    // 1. Zeile
    for (string s; iss1 >> s;) {
        // String mit Satzzeichen wird gespeichert
        spaces_with_punctuations.push_back(s);

        // alle nicht alphabetischen Zeichen oder '_' (=Lücken) werden entfernt
        string new_s;
        for (char c : s) {
            if (isalpha(c) or c == '_') {
                new_s.push_back(c);
            }
        }

        // im String s befinden sich keine Satzzeichen
        // speichern im Vector spaces
        spaces.push_back(new_s);
    }

    // alle Wörter aus der 2. Zeile werden im Vector words hinzugefügt
    for (string s; iss2 >> s;) {
        words.push_back(s);
    }

    // Anzahl der gesamten Wörter ist die Größe des Vector spaces
    const int n_words = spaces.size();

    // Vector zu_viele, um für alle Lücken die möglich passenden Wörter zu speichern
    vvs zu_viele(n_words);

    // Vector zum Speichern des neuen vollständigen Satzes
    vs new_sentence(n_words);

    // ein Durchgang der for-loop für jedes Wort im Satz    
    for (int i = 0; i<spaces.size(); ++i) {
        string word = spaces[i];
        int length = word.length();
        
        // Speichern aller Wörter mit derselben Länge 
        vs same_length; 
        for (const string& w : words) {
            u16string ww = utf8_to_utf16(w);
            if (ww.length() == length) {
                same_length.push_back(w);
            }
        }

        // aus den vorher gespeicherten Wörtern mit derselben Länge
        // werden nun diejenigen rausgefiltert, die dieselben Buchstaben am selben Index haben
        vi index_chars;
        for (int j = 0; j<word.length(); ++j){
            char w = word[j];
            if (w != '_') {
                same_length = filter_words(same_length, w, j);
            }
        }

        if (same_length.size() > 1) {
            // die potenziell passenden Wörter werden im Vector zu_viele an der Stelle der Lücke im Satz gespeichert
            zu_viele[i] = same_length;
        } else {
            // falls nur ein Wort passt, wird dies als "Lösung" für diese Lücke übernommen

            string wort = same_length[0];
            // Entfernen des Wortes aus dem Speicher für alle potenziell passenden Wörter
            zu_viele = remove_word_vvs(zu_viele, wort);

            // Zuweisen der Lösung
            new_sentence[i] = same_length[0];

            // Entfernen des Wortes aus dem Wörterspeicher
            remove(words.begin(), words.end(), same_length[0]);
        }

    }


    bool wiederholen;
    LOOP:
    for (int i = 0; i< n_words; ++i) {
        wiederholen = false;
        // falls der Speicher der potenziellen Wörter für diese Lücke nicht leer ist
        if (!zu_viele[i].empty()) {

            if (zu_viele[i].size() <= 1) {
                // falls nur noch ein Wort potenziell für diese Lücke möglich ist
                string wort = zu_viele[i][0];
                
                // das Wort wird nicht mehr als potenzielles Wort für andere Lücken angesehen und entfernt
                zu_viele = remove_word_vvs(zu_viele, wort);

                // Wort wird als Lösung für diese Lücke gespeichert
                new_sentence[i] = wort;
            } else {
                // check ob alle dasselbe Wort sind
                string s = zu_viele[i][0];
                for (const string& wort : zu_viele[i]) {
                    if (s != wort) {
                        wiederholen = true;
                    }
                }

                if (wiederholen) {
                    continue;
                } else {
                    // falls alle potenziell möglichen Wörte für diese Lücke dasselbe sind, 
                    // wird das erste als Lösung verwendet
                    string wort = zu_viele[i][0];
                    // das Wort wird von anderen Speichern für ein potenzielles Wort entfernt
                    zu_viele = remove_word_vvs(zu_viele, wort);
                    zu_viele = remove_word_vvs(zu_viele, wort, true);

                    // Zuweisen des Lösungswortes für diese Lücke
                    new_sentence[i] = wort;
                    wiederholen = false;
                }
            }
        }
    }

    // die Loop (Schleife) wird solange wiederholt bis kein Wort mehr im Vector zu_viele
    // und die boolsche Variable wiederholen = true ist 
    if (wiederholen or !check_vvs_empty(zu_viele)) {
        goto LOOP;
    }

    // der neue Satz mit den passenden Lösungen für die Lücken wird noch mit den Satzzeichen zusammengefügt,
    // sodass an der passenden Stelle jene Satzzeichen sind, die dort auch mit der Eingabe übergeben wurden
    new_sentence = merge_vs(spaces_with_punctuations, new_sentence);


    // Ausgabe des neuen Satzes
    cout << "\n\n>> Folgender Satz wurde vervollständigt: " << endl;
    cout << "> ";

    for (const string& w : new_sentence) {
        cout << w << " ";
    }

}
