#include <iostream>
#include <cassert>
#include <limits>
#include <algorithm>

#include "class.hpp"

using namespace std;


template <typename T>
void input(T &var)
{
    while (!(std::cin >> var))
    {
        std::cout << "Il y a eu une erreur, recommencer:" << std::endl;
        std::cin.clear();                                                   // On remet std::cin dans un état fonctionnel.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // On vide les caractères mémorisés.
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // On vide les caractères mémorisés.
}

void test(){
    Gest_mdp test;
    assert(!test.exists("random"));

    test.add("random", "yolo", "mysupersecretmdp");
    assert(test.exists("random"));
    
    test.add("a", "b", "c");
    string res = test.get("a");
    assert(res == "b:c");

    test.remove("random");
    assert(!test.exists("random"));
}

void ui(){
    bool is_exit = 0;

    string mdp_maitre;
    cout << "Mot de passe maitre : ";
    input(mdp_maitre);

    Gest_mdp bdd;

    if (mdp_maitre != "yolo"){
        is_exit = 1;
    }else{
        bdd.read_file("output.txt", mdp_maitre);
    
        cout << "----------------------------------" << endl;
        cout << "Gestionnaire de mots de passe" << endl;
        cout << "Commandes : exit / add / print / remove" << endl;
        cout << "----------------------------------" << endl;

        bdd.print();

        do {
            string user_input;
            cout << ">>> ";
            getline(cin, user_input);

            if (user_input == "exit"){
                bdd.write("output.txt", "yolo");
                is_exit = 1;
            }
            else if (user_input == "add"){
                string site_to_add;
                cout << "Quelle site voulez vous ajouter ? ";
                input(site_to_add);

                string username_to_add;
                cout << "Quelle est le username correspondant ? ";
                input(username_to_add);

                
                bool mdp_ok = false;
                string mdp_to_add;
                do{
                    
                    cout << "Quelle est le mot de passe correspondant ? (g to generate) ";
                    input(mdp_to_add);
                    if (mdp_to_add == "g"){
                        mdp_to_add = bdd.gen_mdp();
                    }
                    mdp_ok = bdd.is_mdp_ok(mdp_to_add);
                    if (mdp_ok != true){
                        cout << "Le mdp est pas assez robuste." << endl;
                    }
                }while (mdp_ok != true);

                bdd.add(site_to_add, username_to_add, mdp_to_add);
            }
            else if (user_input == "remove"){
                string site_to_remove;
                cout << "Quelle site voulez vous retirer ? ";
                input(site_to_remove);
                bdd.remove(site_to_remove);
            }
            else if (user_input == "print"){
                bdd.print();
            }
            else{
                cout << "Commande non reconnu : exit / add / print / remove" << endl;
            }

        }while (is_exit != 1);
    }

    
}

int main(){
    test();
    ui();

    return 0;
}