#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime> 

#include "class.hpp"
#include "AES.hpp"

EasyAES aes;

using namespace std;

Gest_mdp::Gest_mdp(const Gest_mdp &other) {
    annuaire = other.annuaire;
}

void Gest_mdp::add(string site, string username, string mdp){
    string content = username + ":" + mdp;
    this->annuaire[site] = content;
}

void Gest_mdp::remove(string site){
    this->annuaire.erase(site);
}

void Gest_mdp::print(){
    for (pair<string, string> const & paire : this->annuaire){
        cout << paire.first;
        cout << " " << paire.second << endl;
    }
}

bool Gest_mdp::exists(string user_input){
    if (this->annuaire.count(user_input)>0){
        return true;
    }
    else{
        return false;
    }
}

string Gest_mdp::get(string site){
    return this->annuaire[site];
}

int Gest_mdp::read_file(string path, string mdp){
    /**/
    ifstream file(path, ios::binary);
    if (!file.is_open()) {
        cout << "Erreur lors de l'ouverture du fichier." << endl;
        return 1;
    }

    string line;
    string delimiter = ":";

    string data;
    while (getline(file, line)) {
        data = data + line;
    }

    string data_dechiffre = aes.decrypt(data, mdp);

    stringstream ss(data_dechiffre);
    string ligne;
    
    while (getline(ss, ligne)) {
        stringstream lineStream(ligne);
        string site, user, mdp;

        if (getline(lineStream, site, ':') && getline(lineStream, user, ':') && getline(lineStream, mdp, ':')) {
            add(site, user, mdp);
        }
    }

    return 0;
}

int Gest_mdp::write(string path, string mdp){

    ofstream file(path, ios::binary);
    if (!file.is_open()){
        cout << "Impossible d'ouvrir le fichier" << endl;
        return 1;
    }

    stringstream ss;

    for (pair<string, string> const & paire : this->annuaire){
        ss << paire.first << ':' << paire.second << "\n";
    }
    string concat = ss.str();
    string data_chiffre = aes.encrypt(concat, mdp);
    cout << data_chiffre << endl;

    file << data_chiffre;

    file.close();

    return 0;
}

bool Gest_mdp::is_mdp_ok(string mdp){
    if (mdp.length() > 4){
        bool majuscule;
        bool nb;
        bool special;
        string specialChars = "!@#$%^&*()-_+=<>?";

        for (char c : mdp) {
            if (isupper(c)) {
                majuscule = true;
            } 
            if (isdigit(c)) {
                nb = true;
            } 
            if (specialChars.find(c) != string::npos) {
                special = true;
            }
        }

        ifstream file("liste.txt");
        string line;
        while (getline(file, line)) {
            if (mdp == line){
                return false;
            }
        }

        if (majuscule && nb && special){
            return true;
        }
        else{
            return false;
        }

    }

    return false;
}

string Gest_mdp::gen_mdp(){
    string car = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-_+=<>?";
    string mdp_gen;
    int nombreAleatoire;

    srand(std::time(0));
    for (int i = 0; i < 50; i++){
        nombreAleatoire = 1 + rand() % 79;
        mdp_gen = mdp_gen + car[nombreAleatoire];
    }

    return mdp_gen;
}