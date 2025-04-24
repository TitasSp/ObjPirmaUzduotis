#include "funkcijos.h"

float Vidurkis(vector<int> pazymiai) {
    if (pazymiai.empty()) {
        throw invalid_argument("Pazymiu sarasas yra tuscias");
    }
    float suma = 0;
    for (int pazymys : pazymiai) {
        suma += pazymys;
    }
    return suma / pazymiai.size();
}

float Mediana(vector<int> pazymiai) {
    if (pazymiai.empty()) {
        throw invalid_argument("Pazymiu sarasas yra tuscias");
    }
    sort(pazymiai.begin(), pazymiai.end());
    int pazSk = pazymiai.size();
    if (pazSk % 2 == 0) {
        return (pazymiai[pazSk / 2 - 1] + pazymiai[pazSk / 2]) / 2.0;
    } else {
        return pazymiai[pazSk / 2];
    }
}

void GeneruotiPazymius(int pazymiuSk, vector<int>& pazymiai) {
    if (pazymiuSk <= 0) {
        throw invalid_argument("Pazymiu skaicius turi buti teigiamas");
    }
    for(int i = 0; i < pazymiuSk; i++){
        pazymiai.push_back(rand() % 10 + 1);
        cout << pazymiai[i] << " ";
    }
    cout << endl;
}

void GeneruotiStudentus(int studentuSk, vector<Studentas>& studentai) {
    if (studentuSk <= 0) {
        throw invalid_argument("Studentu skaicius turi buti teigiamas");
    }
    vector<string> vardai = {"Jonas", "Petras", "Antanas", "Tomas", "Marius"};
    vector<string> pavardes = {"Jonaitis", "Petraitis", "Antanaitis", "Tomaitis", "Maraitis"};

    for (int i = 0; i < studentuSk; i++) {
        Studentas studentas;
        studentas.vardas = vardai[rand() % vardai.size()];
        studentas.pavarde = pavardes[rand() % pavardes.size()];
        GeneruotiPazymius(rand() % 10 + 1, studentas.pazymiai);
        studentas.egzaminas = rand() % 10 + 1;
        studentai.push_back(studentas);
    }
}

void NuskaitytiStudentusIsFailo(string failas, vector<Studentas>& studentai) {
    auto start = high_resolution_clock::now();

    ifstream in(failas);
    if (!in.is_open()) {
        throw runtime_error("Nepavyko atidaryti failo");
    }

    string line;
    studentai.reserve(10000000); // rezervuoja atminti

    // praleidzia pirma eilute
    getline(in, line);

    while (getline(in, line)) {
        istringstream iss(line);
        Studentas studentas;
        iss >> studentas.vardas >> studentas.pavarde;

        int pazymys;
        studentas.pazymiai.clear();
        while (iss >> pazymys) {
            studentas.pazymiai.push_back(pazymys);
        }

        // paskutinis skaicius yra egzaminas
        if (!studentas.pazymiai.empty()) {
            studentas.egzaminas = studentas.pazymiai.back();
            studentas.pazymiai.pop_back();
        }

        studentai.push_back(move(studentas)); // naudoja move, kad nereiketu kopijuoti
           
    }
    studentai.shrink_to_fit();
    in.close();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Studentu nuskaitymas is failo uztruko: " << duration.count() << " ms" << endl;
}

void RikiuotiStudentus(vector<Studentas>& studentai, int pasirinkimas) {
    auto start = high_resolution_clock::now();
    switch (pasirinkimas) {
        case 0:
            // nerikiuoti
            break;
        case 1:
            sort(studentai.begin(), studentai.end(), [](const Studentas& a, const Studentas& b) {
                return a.vardas < b.vardas;
            });
            break;
        case 2:
            sort(studentai.begin(), studentai.end(), [](const Studentas& a, const Studentas& b) {
                return a.pavarde < b.pavarde;
            });
            break;
        case 3:
            sort(studentai.begin(), studentai.end(), [](const Studentas& a, const Studentas& b) {
                return a.galutinis < b.galutinis;
            });
            break;
        case 4:
            sort(studentai.begin(), studentai.end(), [](const Studentas& a, const Studentas& b) {
                return a.galutinis > b.galutinis;
            });
            break;
        default:
            throw invalid_argument("Neteisingas rikiavimo pasirinkimas");
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Studentu rikiavimas uztruko: " << duration.count() << " ms" << endl;
}

void FailuGeneravimas(int studentuSk, int pazymiuSk) {
    auto startVisas = high_resolution_clock::now();
    ofstream out("studentai" + to_string(studentuSk) + ".txt");
    ostringstream buffer;

    buffer << left << setw(25) << "Vardas" << setw(25) << "Pavarde";
    for (int i = 1; i <= pazymiuSk; i++) {
        buffer << setw(10) << ("ND " + to_string(i));
    }
    buffer << setw(10) << "Egz." << endl;
    out << buffer.str();
    buffer.str(""); // isvalo bufferi

    auto startGeneravimas = high_resolution_clock::now();
    auto totalIrasymasDuration = milliseconds(0);

    // generuoja studentus su pazymiais
    for (int i = 1; i <= studentuSk; i++) {
        
        buffer << left << setw(25) << ("Vardas" + to_string(i)) << setw(25) << ("Pavarde" + to_string(i));
        for (int j = 0; j < pazymiuSk; j++) {
            buffer << setw(10) << (rand() % 10 + 1);
        }
        buffer << setw(10) << (rand() % 10 + 1) << endl;

        if (i % 100000 == 0) { // iraso i faila kas 100000 irasu
            auto startIrasymas = high_resolution_clock::now();
            out << buffer.str();
            buffer.str(""); // isvalo bufferi
            auto endIrasymas = high_resolution_clock::now();
            totalIrasymasDuration += duration_cast<milliseconds>(endIrasymas - startIrasymas);
        }
    }
    auto endGeneravimas = high_resolution_clock::now();
    auto durationGeneravimas = duration_cast<milliseconds>(endGeneravimas - startGeneravimas);

    auto startIrasymas = high_resolution_clock::now();
    // likusius irasom i faila
    out << buffer.str();
    out.close();
    auto endIrasymas = high_resolution_clock::now();
    totalIrasymasDuration += duration_cast<milliseconds>(endIrasymas - startIrasymas);
    
    auto endVisas = high_resolution_clock::now();
    auto durationVisas = duration_cast<milliseconds>(endVisas - startVisas);

    cout << "Studentu generavimas uztruko: " << (durationGeneravimas.count() - totalIrasymasDuration.count()) << " ms" << endl;
    cout << "Studentu irasymas i faila uztruko: " << totalIrasymasDuration.count() << " ms" << endl;
    cout << "Is viso uztruko: " << durationVisas.count() << " ms" << endl;
}

void StudentuAtskirimas() {
    ofstream outVargsiukai("stud_b.txt");
    ofstream outKieti("stud_g.txt");

    if (!outVargsiukai.is_open() || !outKieti.is_open()) {
        throw runtime_error("Nepavyko atidaryti failo");
    }

    vector<Studentas> vargsiukai;
    vector<Studentas> kieti;

    auto start = high_resolution_clock::now();

    // Separate students into two vectors
    for (const auto& studentas : studentai) {
        if (studentas.galutinis < 5) {
            vargsiukai.push_back(studentas);
        } else {
            kieti.push_back(studentas);
        }
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Studentu suskirstymas uztruko: " << duration.count() << " ms" << endl;

    auto start2 = high_resolution_clock::now();

    // Write "vargsiukai" to file
    outVargsiukai << left << setw(15) << "Vardas" << setw(20) << "Pavarde";
    if (!vargsiukai.empty()) {
        for (size_t i = 1; i <= vargsiukai[0].pazymiai.size(); i++) {
            outVargsiukai << setw(10) << ("ND " + to_string(i));
        }
    }
    outVargsiukai << setw(10) << "Egz." << endl;

    for (const auto& studentas : vargsiukai) {
        outVargsiukai << left << setw(15) << studentas.vardas
                      << setw(20) << studentas.pavarde;
        for (const auto& pazymys : studentas.pazymiai) {
            outVargsiukai << setw(10) << pazymys;
        }
        outVargsiukai << setw(10) << studentas.egzaminas << endl;
    }

    // Write "kieti" to file
    outKieti << left << setw(15) << "Vardas" << setw(20) << "Pavarde";
    if (!kieti.empty()) {
        for (size_t i = 1; i <= kieti[0].pazymiai.size(); i++) {
            outKieti << setw(10) << ("ND " + to_string(i));
        }
    }
    outKieti << setw(10) << "Egz." << endl;

    for (const auto& studentas : kieti) {
        outKieti << left << setw(15) << studentas.vardas
                 << setw(20) << studentas.pavarde;
        for (const auto& pazymys : studentas.pazymiai) {
            outKieti << setw(10) << pazymys;
        }
        outKieti << setw(10) << studentas.egzaminas << endl;
    }

    auto end2 = high_resolution_clock::now();
    auto duration2 = duration_cast<milliseconds>(end2 - start2);
    cout << "Studentu irasymas i failus uztruko: " << duration2.count() << " ms" << endl;

    outVargsiukai.close();
    outKieti.close();
}

///////////////////////////////////////////////////////////////////////////
void Test1() {
    vector<int> studentuSkaiciai = {1000, 10000, 100000, 1000000, 10000000};
    int pazymiuSk = 10; // pazymiu skaicius
    
    for (int studentuSk : studentuSkaiciai) {
        cout << "Generuojami " << studentuSk << " studentu" << endl;
        
        FailuGeneravimas(studentuSk, pazymiuSk);
       
    }
}

void Test2() {
    vector<string> failai = {"studentai1000.txt", "studentai10000.txt", "studentai100000.txt", "studentai1000000.txt", "studentai10000000.txt"};

    
    for (string failas : failai) {
        cout << failas << endl;
        auto start = high_resolution_clock::now();
        ofstream out;
        
        out.open("rezultatai.txt", ios::trunc | ios::out | ios::binary);
          
        vector<Studentas> studentai;
        NuskaitytiStudentusIsFailo(failas, studentai);
        
        
        out << left << setw(15) << "Vardas" << setw(20) << "Pavarde" << "Galutinis" << endl;
        
        // loopina per kiekviena studenta studentu vektoriuje ir skaiciuoja galutini pazymi 
        for (auto& studentas : studentai) {
           
                studentas.galutinis = 0.4 * Vidurkis(studentas.pazymiai) + 0.6 * studentas.egzaminas;
        }

        RikiuotiStudentus(studentai, 0);

        // irasoma dalimis, kad neuzimtu per daug atminties
        const size_t chunkSize = 100000; // daliu dydis
        size_t totalStudents = studentai.size();
        for (size_t i = 0; i < totalStudents; i += chunkSize) {
            size_t end = min(i + chunkSize, totalStudents);
            for (size_t j = i; j < end; ++j) {
          
                out << left << setw(15) << studentai[j].vardas << setw(20) << studentai[j].pavarde << fixed << setprecision(2) << studentai[j].galutinis << endl;
               
            }
        }
        StudentuAtskirimas();
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        cout << "Is viso uztruko: " << duration.count() << " ms" << endl;
        cout << endl;
    }

}
