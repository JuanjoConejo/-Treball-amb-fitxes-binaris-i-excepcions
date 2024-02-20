#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <stdexcept>

using namespace std;

struct Serie {
    string title; // títol de la sèrie
    int num_episodes; // nombre d'episodis
    int characters_per_episode; // nombre de personatges per episodi
};

// Funció per desar les dades de les sèries a un fitxer binari
void save_data(const vector<Serie>& series) {
    ofstream file("series_data.bin", ios::binary);
    if (!file.is_open()) {
        throw runtime_error("No s'ha pogut obrir el fitxer per a l'escriptura.");
    }

    for (const Serie& serie : series) {
        file.write(reinterpret_cast<const char*>(&serie), sizeof(Serie));
    }
}

// Funció per carregar les dades de les sèries des d'un fitxer binari
vector<Serie> load_data() {
    vector<Serie> series;
    ifstream file("series_data.bin", ios::binary);
    if (!file.is_open()) {
        return series;
    }

    Serie serie;
    while (file.read(reinterpret_cast<char*>(&serie), sizeof(Serie))) {
        series.push_back(serie);
    }

    return series;
}

// Funció per afegir una nova sèrie
void insert_serie(vector<Serie>& series, const string& title, int num_episodes, int characters_per_episode) {
    series.push_back({ title, num_episodes, characters_per_episode });
    save_data(series);
}

// Funció per esborrar una sèrie
void delete_serie(vector<Serie>& series, const string& title) {
    series.erase(remove_if(series.begin(), series.end(),
        [&title](const Serie& s) { return s.title == title; }),
        series.end());
    save_data(series);
}

// Funció per calcular estadístiques sobre el nombre de personatges per episodi
void calculate_statistics(const vector<Serie>& series) {
    if (series.empty()) {
        cout << "No hi ha dades disponibles." << endl;
        return;
    }

    vector<int> characters_counts;
    for (const Serie& serie : series) {
        characters_counts.push_back(serie.characters_per_episode);
    }

    double sum = 0;
    for (int count : characters_counts) {
        sum += count;
    }
    double average_characters = sum / characters_counts.size();

    auto mode = [&characters_counts]() {
        int mode_value = 0;
        int max_count = 0;
        for (int value : characters_counts) {
            int count = count_if(characters_counts.begin(), characters_counts.end(), [&value](int x) { return x == value; });
            if (count > max_count) {
                max_count = count;
                mode_value = value;
            }
        }
        return mode_value;
        };

    sort(characters_counts.begin(), characters_counts.end());
    int mid = characters_counts.size() / 2;
    double median_characters = (characters_counts.size() % 2 != 0) ? characters_counts[mid] : (characters_counts[mid - 1] + characters_counts[mid]) / 2.0;

    cout << "Mitjana de personatges per episodi: " << average_characters << endl;
    cout << "Moda de personatges per episodi: " << mode() << endl;
    cout << "Mediana de personatges per episodi: " << median_characters << endl;
}

// Funció principal del programa
int main() {
    vector<Serie> series = load_data();

    while (true) {
        cout << "\n1. Afegir sèrie" << endl;
        cout << "2. Esborrar sèrie" << endl;
        cout << "3. Calcular estadístiques" << endl;
        cout << "4. Sortir" << endl;

        int choice;
        cout << "Selecciona una opció: ";
        cin >> choice;

        if (choice == 1) {
            string title;
            int num_episodes, characters_per_episode;
            cout << "Introdueix el títol de la sèrie: ";
            cin.ignore();
            getline(cin, title);
            cout << "Introdueix el nombre d'episodis: ";
            cin >> num_episodes;
            cout << "Introdueix el nombre de personatges per episodi: ";
            cin >> characters_per_episode;
            insert_serie(series, title, num_episodes, characters_per_episode);
        }
        else if (choice == 2) {
            string title;
            cout << "Introdueix el títol de la sèrie a esborrar: ";
            cin.ignore();
            getline(cin, title);
            delete_serie(series, title);
        }
        else if (choice == 3) {
            calculate_statistics(series);
        }
        else if (choice == 4) {
            break;
        }
        else {
            cout << "Opció no vàlida." << endl;
        }
    }

    return 0;
}
