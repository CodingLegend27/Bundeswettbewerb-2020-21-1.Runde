#include <bits/stdc++.h>

using namespace std;

#define vi vector<int>
#define vvi vector<vi>
#define vs vector<string>
#define vb vector<bool>

const int INF = numeric_limits<int>::max()/2;

// Löschen des gegebenen Elements aus allen Untervectoren von arr
vvi delete_element_vvi(vvi& arr, int e) {
    for (int i = 0; i < arr.size(); ++i) {
        auto it = find(arr[i].begin(), arr[i].end(), e);
        if (it != arr[i].end()) {
            arr[i].erase(it);
        }
    }
    return arr;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    cout << ">> Eingabe der Wünsche im BwInf-Format\n";

    // Einlesen der Daten
    int n;
    cin >> n;
    
    // Wünsche der n Personen werden gespeichert
    // Erstwunsch in arr[i][0]
    // Zweitwunsch in arr[i][1]
    // Drittwunsch in arr[i][2]
    vvi arr(n, vi(3));

    // Jeder eingelesene Index wird um 1 reduziert, 
    // da Arrays bei 0 starten
    for (int i = 0; i < n; ++i) {
        int first, sec, third;
        cin >> first >> sec >> third;
        arr[i][0] = first - 1;
        arr[i][1] = sec - 1;
        arr[i][2] = third - 1;
    }

    // Für die drei verschiedenen Wünsche gibt es für jedes Geschenk
    // eine Liste mit Bewerbern, die sich das Geschenk wünschen
    // z.B. erste_geschenke[2] ist ein Vector mit allen Bewerbern für das Geschenk Nr. 2
    vvi erste_geschenke(n, vi());
    vvi zweite_geschenke(n, vi());
    vvi dritte_geschenke(n, vi());

    // Für jedes Geschenk werden die potentiellen Empfänger gespeichert
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (j == 0) erste_geschenke[arr[i][j]].push_back(i);
            else if (j == 1) zweite_geschenke[arr[i][j]].push_back(i);
            else if (j == 2) dritte_geschenke[arr[i][j]].push_back(i);
        }
    }


    // in verteilung wird die finale Verteilung der Geschenke gespeichert
    // verteilung[2] beinhaltet das Geschenk, das Person Nr. 2 erhält
    vi verteilung(n, -1);

    // Alle Geschenke mit mindestens einen Bewerber als Erstwunsch werden sicher als Erstwunsch vergeben werden
    // Geschenk kann somit nicht als Zweit- oder Drittwunsch verwendet werden
    for (int i = 0; i < n; ++i) {
        if (erste_geschenke[i].size() >= 1) {
            zweite_geschenke[i].clear();
            dritte_geschenke[i].clear();
        }
    }

    // Zuweisung aller Geschenke, die nur einen Bewerber haben
    // hier: Erstwunsch
    for (int i = 0; i < n; ++i) {

        // Falls das Geschenk nur einen Bewerber hat wird es sofort zugewiesen
        if (erste_geschenke[i].size() == 1) {
            int person = erste_geschenke[i][0];
            // Die Person bekommt das Geschenk sicher
            verteilung[person] = i;

            // Liste für das aktuelle Geschenk wird gelöscht, da es vergeben wurde
            erste_geschenke[i].clear();

            // Person ist kein potenzieller Bewerber für andere Geschenke mehr
            zweite_geschenke = delete_element_vvi(zweite_geschenke, person);
            dritte_geschenke = delete_element_vvi(dritte_geschenke, person);
            
        }
    }


    // Ermitteln der optimalsten Empfänger für alle Geschenke
    // hier: Erstwunsch
    for (int i = 0; i < n; ++i) {
        // Geschenk hat mehrere Bewerber
        if (erste_geschenke[i].size() > 1) {
            // Diejenige Person mit den meisten Konkurrenten beim Zweitwunsch erhalten ihren Erstwunsch
            int max_value_ges = -1;
            int max_person_idx_ges;

            // Iteration über alle unterschiedliche Bewerber p
            for (int p : erste_geschenke[i]) {

                // mit max_value werden die Anzahl der Konkurrenten beim Zweitwunsch oder ggf. Drittwunsch gespeichert
                int max_value = -1;
                int max_person_idx;

                // Zweitwünsche werden überprüft
                for (int j = 0; j < n;++j) {
                    if (find(zweite_geschenke[j].begin(), zweite_geschenke[j].end(), p) != zweite_geschenke[j].end()) {
                        if (zweite_geschenke[j].size() > max_value) {
                            max_value = zweite_geschenke[j].size();
                            max_person_idx = p;

                            // Abbruch, da man sich nur ein Geschenk wünschen kann
                            break;
                        }
                    }
                }

                // Person kann keinen Zweitwunsch erfüllt bekommen --> Überprüfung des Drittwunsches
                if (max_value == -1) {
                    for (int j = 0; j < n;++j) {
                        if (find(dritte_geschenke[j].begin(), dritte_geschenke[j].end(), p) != dritte_geschenke[j].end()) {
                            if (dritte_geschenke[j].size() > 0) {
                                max_value = dritte_geschenke[j].size();
                                max_person_idx = p;

                                // Abbruch, da man sich nur ein Geschenk wünschen kann
                                break;
                            }
                        }
                    }
                }

                // Person kann auch keinen Drittwunsch erfüllt bekommen
                // --> Person bekommt Erstwunsch
                if (max_value == -1) {
                    max_value = INF;
                    max_person_idx = p;
                }

                // Aktualisierung des optimalsten Empfängers
                if (max_value > max_value_ges) {
                    max_value_ges = max_value;
                    max_person_idx_ges = max_person_idx;
                }
            }

            // falls alle Personen gleich geeignet sind, wird die erste Person ausgewählt
            if (max_value_ges == -1) {
                max_person_idx_ges = erste_geschenke[i][0];
            }

            // optimalste Person erhält den Erstwunsch
            verteilung[max_person_idx_ges] = i;
            
            // restliche potentielle Empfänger werden entfernt
            erste_geschenke[i].clear();

            // Person ist kein potenzieller Empfänger für andere Geschenke mehr 
            zweite_geschenke = delete_element_vvi(zweite_geschenke, max_person_idx_ges);
            dritte_geschenke = delete_element_vvi(dritte_geschenke, max_person_idx_ges);
        }
    }



    // Alle Geschenke mit mindestens einem Bewerber als Zweitwunsch werden sicher als Zweitwunsch vergeben werden
    // Geschenk kann somit nicht als Drittwunsch verwendet werden
    for (int i = 0; i < n; ++i) {
        if (zweite_geschenke[i].size() >= 1) {
            dritte_geschenke[i].clear();
        }
    }


    // Zuweisung aller Geschenke, die nur einen Bewerber haben
    // hier: Zweitwunsch
    for (int i = 0; i < n; ++i) {
        // Falls das Geschenk nur einen Bewerber hat, wird es sofort zugewiesen
        if (zweite_geschenke[i].size() == 1) {
            int person = zweite_geschenke[i][0];
            // person bekommt das Geschenk sicher
            verteilung[person] = i;

            // Liste für das aktuelle Geschenk wird gelöscht, da es vergeben wurde
            zweite_geschenke[i].clear();

            // person ist kein potenzieller Bewerber für andere Geschenke mehr
            dritte_geschenke = delete_element_vvi(dritte_geschenke, person);

        }
    }

    // Ermitteln der optimalsten Empfänger für alle Geschenke
    // hier: Zweitwunsch
    for (int i = 0; i < n; ++i) {
        // Geschenk hat mehrere Bewerber
        if (zweite_geschenke[i].size() >= 1) {
            // Jene Person mit den meisten Konkurrenten beim Drittwunsch erhält ihren Zweitwunsch
            int max_value_ges = -1;
            int max_person_idx_ges;

            // Iteration über alle unterschiedliche Bewerber p
            for (int p : zweite_geschenke[i]) {
                
                // mit max_value werden die Anzahl der Konkurrenten beim Drittwunsch gespeichert
                int max_value = -1;
                int max_person_idx;

                // Drittwünsche werden überprüft
                for (int j = 0; j < n; ++j) {
                    if (find(dritte_geschenke[j].begin(), dritte_geschenke[j].end(), p) != dritte_geschenke[j].end()) {
                        if (dritte_geschenke[j].size() > max_value) {
                            max_value = dritte_geschenke[j].size();
                            max_person_idx = p;

                            // Abbruch, da man sich nur ein Geschenk wünschen kann
                            break;
                        }
                    }
                }

                // Person kann keinen Drittwunsch erfüllt bekommen
                // --> Person bekommt Zweitwunsch
                if (max_value == -1) {
                    max_value = INF;
                    max_person_idx = p;
                }

                // Aktualisieren des optimalsten Empfängers
                if (max_value > max_value_ges) {
                    max_value_ges = max_value;
                    max_person_idx_ges = max_person_idx;
                }
            }

            // falls alle Personen gleich geeignet sind, wird die erste Person ausgewählt
            if (max_value_ges == -1) {
                max_person_idx_ges = zweite_geschenke[i][0];
            }

            verteilung[max_person_idx_ges] = i;

            // restliche potenzielle Empfänger werden entfernt
            zweite_geschenke[i].clear();

            // Person ist kein potenzieller Empfänger für andere Geschenke mehr
            dritte_geschenke = delete_element_vvi(dritte_geschenke, max_person_idx_ges);
        }
    }

    // Zuweisung aller Geschenke beim Drittwunsch, egal wer das Geschenk bekommt
    for (int i = 0; i < n; ++i) {
        if (dritte_geschenke[i].empty()) continue; // Geschenk hat keinen gewünschten Empfänger
        int person = dritte_geschenke[i][0];
        verteilung[person] = i;
        dritte_geschenke[i].clear();
    }

    // Nicht verwendete Geschenke werden ermittelt
    vb used(n, false);
    for (int i = 0; i < n; ++i) {
        if (verteilung[i] < 0) continue; // continue, falls verteilung[i] = -1 ,d.h. noch nicht vergeben
        used[verteilung[i]] = true;
    }

    // nicht verwendete Geschenke werden im Stack gespeichert
    stack<int> s; 
    for (int i = 0; i < n; ++i) {
        if (!used[i]) s.push(i);
    }

    // restliche Personen bekommen die restlichen Geschenke vom stack
    for (int i = 0; i < n; ++i) {
        if (verteilung[i] < 0) {
            verteilung[i] = s.top();
            s.pop();
        }
    }

    // Ausgabe
    cout << "\n\n> Ausgabe der Verteilung: \n";
    cout << "Schüler Nr. -> Geschenk Nr. \n";

    for (int i = 0; i < n; ++i) {
        cout << i+1 << " -> " << verteilung[i]+1 << "\n";
    }


    // Auswerten der Verteilung
    // Ermitteln der Anzahl der erfüllten Erst-, Zweit- und Drittwünsche
    int s1=0;
    int s2=0;
    int s3=0;
    int rest=0;
    for (int i = 0; i < n; ++i) {
        if (verteilung[i] == arr[i][0]) s1++;
        else if (verteilung[i] == arr[i][1]) s2++;
        else if (verteilung[i] == arr[i][2]) s3++;
        else rest++;
    }
    cout << "Anzahl an erfüllten 1. Wünschen: " << s1 << "\n";
    cout << "Anzahl an erfüllten 2. Wünschen: " << s2 << "\n"; 
    cout << "Anzahl an erfüllten 3. Wünschen: " << s3 << "\n";
    cout << "Anzahl an Geschenk, die nicht gewünscht waren: "<<rest;

    return 0;
}