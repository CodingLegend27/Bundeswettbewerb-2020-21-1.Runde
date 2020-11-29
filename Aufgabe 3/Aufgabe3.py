from random import random, randrange

def get_winner(i: int, j: int, strength_i: int, strength_j: int):
    """ Spieler mit den Nummer i und j spielen gegeneinander
    Parameter sind ebenfalls die Spielerstärken beider Spieler
    Rückgabewert ist der Gewinner der beiden Spieler (entweder i oder j)
    """
    # Summe der Gewichtungen der beiden Spieler
    s = strength_i + strength_j
    
    # Zufallszahl zwischen 0 und 1 wird erzeugt
    r = random()
    
    # der kleinere Wert von beiden wird ermittelt
    if strength_i < strength_j:
        min_index = i
        max_index = j
        min_value = strength_i
    else:
        min_index = j
        max_index = i
        min_value = strength_j
    
    quot = min_value/s 
    
    if r < quot:
        # Spieler mit der kleineren Spielstärke gewinnt
        winner = min_index
    else:
        # anderer Spieler ist Gewinner (Spieler mit der größeren Spielstärke)
        winner = max_index
    
    return winner    

def liga(n: int, arr: list):
    # Anzahl der Siege der einzelnen Spieler
    num_siege = []
    for i in range(n):
        num_siege.append(0)
    
    # Jeder Spieler spielt gegen jeden anderne genau ein Mal
    for i in range(n):
        for j in range(i+1, n):
            w = get_winner(i, j, arr[i], arr[j])
            # Anzahl der Siege des Gewinners wird erhöht
            num_siege[w] += 1
    
    max_element = max(num_siege)
    winner = num_siege.index(max_element)
        
    return winner
        
def k_o(n: int, arr: list, type: int, num_spiele: int):
    """ Je nach Typ wird die Wartschlange unterschiedlich gebildet 
        und der Turnierplan nimmt eine unterschiedliche Form an
    """
    # Wartschlange q
    q = []
    
    # Typ 1: Sortierte Reihenfolge: 1 gegen 2, 3 gegen 4, etc.
    if type == 0:
        for i in range(n):
            q.append(i)
    
    # Typ 2: Spieler 1 gegen Spieler n, 2 gegen n-1, etc.
    elif type == 1:
        for i in range(int(n/2)):
            q.append(i)
            q.append((n-(i+1)))
    
    # Typ 3: Spieler werden zufällig zusammen kombiniert
    # der Index wird zufällig ausgewählt
    elif type == 2:
        uebrige = []
        for i in range(n):
            uebrige.append(i)
        
        for i in range(n):
            index = randrange(0, len(uebrige))
            q.append(uebrige[index])
            
            uebrige.remove(uebrige[index])
            
        
    # Solange die Warteschlange noch nicht leer ist
    while len(q) > 1:
        spieler1 = q.pop(0)
        spieler2 = q.pop(0)
        
        win_counter = []
        for _ in range(n):
            win_counter.append(0)
        # Es wird so oft gespielt, wie es der Methode übergeben wird
        # also kann das K.O.- und das K.O.x5-System darstellen
        for _ in range(num_spiele):
            # Anzahl der gewonnenen Spiele des Gewinners wird um 1 erhöht
            w = get_winner(spieler1, spieler2, arr[spieler1], arr[spieler2])
            win_counter[w] += 1
        
        # Spieler, der öfters gewonnen hat, wird zur Warteschlange wieder hinzugefügt
        max_value = max(win_counter)
        winner = win_counter.index(max_value)
        
        q.append(winner)
        
    # Gewinner ist der letzte verbleibende Spieler in der Warteschlange
    winner_player = q.pop()
            
    return winner_player

def ausgabe(games: int, wins: int):
    """ kleine Helper-Methode zum Vereinfachen der Ausgabe """
    perc = (wins/games)*100
    rounded = round(perc, 4)
    print(f">> {wins} Siege bei {games} Spielen --> {rounded} % ")

def main():
    n = int(input())
    arr = []
    for i in range(n):
        s = int(input())
        arr.append(s)
    
    best_value = 0
    best_player = -1
    for i in range(n):
        if arr[i] > best_value:
            best_value = arr[i]
            best_player = i
    print(f"bester Spieler: Nr. {best_player+1} mit {arr[best_player]}")
    
    games = 1000
    print(f"\nAlle Variationen werden {games} Mal durchgeführt")
    print("Ausgabe, wie oft der spielstärkste Spieler im Durchschnitt gewonnen hat (mit Angabe in Prozent)\n")
    
    # Liga wird gespielt
    wins = 0
    print("Spiel mit Liga-System:")
    for _ in range(games):
        winner_liga = liga(n, arr)
        if best_player == winner_liga:
            wins += 1  
    ausgabe(games, wins)
    
    
    # K.O. (1-fach) wird gespielt
    print("\n Verschiedene Varianten des einfachen K.O.-Systems:")
    print("\n> sortiertes Ausgangs-Matching 1 vs 2, 3 vs 4, ...")
    wins = 0
    for _ in range(games):
        winner = k_o(n, arr, 0, 1)
        if best_player == winner:
            wins += 1
    ausgabe(games, wins)
    
    print("\n> sortiertes Ausgangs-Matching 1 vs n, 2 vs n-1 ")
    wins = 0
    for _ in range(games):
        winner = k_o(n, arr, 1, 1)
        if best_player == winner:
            wins += 1
    ausgabe(games, wins)
    
    print("\n> randomisiertes Ausgangsmatching")
    wins = 0
    for _ in range(games):
        winner = k_o(n, arr, 2, 1)
        if best_player == winner:
            wins += 1
    ausgabe(games, wins)
    
    
    # K.O.x5
    print("\n\nVerschiedene Varianten des K.0.x5-Systems:")
    print("\n> sortiertes Ausgangs-Matching 1 vs 2, 3 vs 4, ... ")
    wins = 0
    for _ in range(games):
        winner = k_o(n, arr, 0, 5)
        if best_player == winner:
            wins += 1
    ausgabe(games, wins)
    
    print("\n> sortiertes Ausgangs-Matching 1 vs n, 2 vs n-1, ...")
    wins = 0
    for _ in range(games):
        winner = k_o(n, arr, 1, 5)
        if best_player == winner:
            wins += 1
    ausgabe(games, wins)
    
    print("\n> randomisiertes Ausgangs-Matching")
    wins = 0
    for _ in range(games):
        winner = k_o(n, arr, 2, 5)
        if best_player == winner:
            wins += 1
    ausgabe(games, wins)
    
        

if __name__ == "__main__":
    main()