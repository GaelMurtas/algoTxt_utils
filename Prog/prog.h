#ifndef _PROG
#define _PROG
#include "../Text/text.h"
#include <vector>
#include <map>
#include <iostream>
#include <functional>
#define _Go *1000*1000*1000
#define _Mo *1000*1000
#define _Ko *1000

/*
 * Classe pour être directement utiliser dans le main
 */
class Prog {
    //Permet de simplifier la prise en  compte des entrer de argv
    struct Option{
        std::string name;
        std::string fullName;
        std::string message;
        //Ici on préfrère éviter l'utulisation d'un template
        std::string valueStr;
        size_t valueInt = 0;
        //On veut également savoir si l'option est passer en argument ou pas
        bool isInit = false;
        //Chaque option a une fonction spécifique pour être charger etexécuter
        std::function<void(const uint &)> load;
        std::function<void(void)> execute;
    };

    public :
        //exécution du programme en prenat en compte les options
        static void execute(const int &, char**);

    private :
        static Prog* instance;
        //Stockage des argument entrer par l'utilisateur
        std::vector<std::string> args;
        //Stockage des oprions prise en charge
        std::map<std::string,Option> options;

        Prog() = delete;
        Prog(const int &, char**);
        ~Prog();

        //Creation de l'instance
        static void init(const int &, char**);

        //chargement des valeurs enter l'utilisateur
        void loadOption();

        //Enregistrement des options
        void initHelp();
        void initThreads();
        void initMemory();
        void initTexte();
        void initMot();
        void initGenerate();
        void initCopie();
        void initRecherche();

        //Affichage de la configuration actuelle
        void affiche();

        //lance les calculs si nécessaire
        void calculs();
};

//un petit utilitaire pour affiche
std::string vecteurToString(const std::vector<size_t> &);
#endif
