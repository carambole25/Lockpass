#ifndef CLASS_GEST_MDP
#define CLASS_GEST_MDP

#include <string>
#include <map>

using namespace std;

class Gest_mdp{
    private:
        map<string, string> annuaire;
    public:
        Gest_mdp() {};

        Gest_mdp(const Gest_mdp &other);

        void add(string site, string username, string mdp);
        void remove(string site);
        void print();

        int read_file(string path, string mdp);
        int write(string path, string mdp);

        bool exists(string user_input);
        string get(string site);

        bool is_mdp_ok(string mdp);
        string gen_mdp();
};

#endif // CLASS_GEST_MDP