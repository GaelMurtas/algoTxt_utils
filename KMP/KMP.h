#ifndef _KMP
#define _KMP
#include <array>
#include <thread>
#include <chrono>
#include <algorithm>
#include "../Text/text.h"

class KMP {
    public :
        //Appel de l'instance de l'algorithme sur les valeurs voulu
        static KMP& getInstance(const Text &, const Text &);
        //Permet de définir le nombre de threads a utiliser
        static void setNbThreads(uint);
        //Permet de dénfinir la mémoire max à utiliser
        static void setMemory(size_t);
        //Execute l'algorithme
        void calcul();
        //renvoi les résultats
        static std::vector<size_t> getResult();

    private :
        static KMP* instance;
        //le nombre de threads a utiliser pour le calcul
        static uint nbt;
        //mémoire max à utiliser
        static size_t memory;
        //texte dans lequel on va chercher un mot
        Text text;
        //mot a chercher dans le text
        Text mot;
        //Tableaur contenant les overlaps du mots
        //long int car on a besoin de la valeur -1 pour faire fonctionner l'algo
        std::vector<long int> overlap;
        //les positions ou le mot apparait
        std::vector<size_t> result;
        //permet de savoir si un calcul est en cours
        //on ne veut pas laisser la possibilité de faire tourner plusieur calcul en parallèle
        //en effet l'algorithme va utiliser la mojorité des ressources de l'ordinateur
        static bool working;

        //calculs du tableau contenant les overlap du mot
        void calculOverlap();
        //recherche du mot dans le texte, avec gestion de la mémoire et multi-threading
        void recherche();
        //partie centrale de l'algorithme, celle dont on va vouloir faire tourner plusieurs instance en paralèle
        static void rechercheCore(const std::string &, const std::string &, size_t);
        //utilitaire de recherche simple pour test
        void testRecherche();

        //constructeur et destructeur
        KMP();
        KMP(const Text &, const Text &);
        KMP(const KMP &);
        ~KMP();
};

#endif
