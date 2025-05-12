#ifndef _TEXT
#define _TEXT
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <random>
#include <memory>
#include "../Prog/prog.h"
#ifndef _Go
#define _Go *1000*1000*1000
#endif

/*
    Classe pour lire et écrire des fichier de texte
    Nottament de fichier dont la taille peut dépasser la mémoire vive disponible
*/

class Text{
    protected:
        //Extrait du fichier actuellement en mémoire
        std::string data;

    private:
        //Chemin du fichier texte
        const std::string path;

        //le flot du fichier
        std::fstream file;

        //Nombre max de caractère a charger simultanément
        size_t maxSize;
        static const size_t maxSizeDefault = 1 _Go;

        //Longuer du fichier
        std::shared_ptr<size_t> fileSize;

        //Indice de début de l'extrait actuellement en mémoire
        size_t index;

        //retien si il y a eu du changement sur le text;
        bool change;

    public:
        //Constructeurs & destructeur
        //Constructiontion à partie d'un fichier avec les paramètres par défault
        Text(const std::string &);
        //Construction par copie, on veut éviter la recopie de "data"
        Text(const Text &);
        //On doit donc aussi redéfinir l'opérateur =
        Text operator=(const Text &);
        //constructeur paramétré avec choix de la taille max de data
        Text(const std::string &, const size_t &);

        Text() = delete;
        ~Text();

        //accesseurs
        //accède au début du texte [0, maxSize-1]
        const std::string & get();
        //accède au texte à partir de l'indice voulue
        const std::string & get(size_t);

        //écriture
        //écrit au début du texte [0, maxSize-1]
        void set(const std::string &);
        //écrit en fin de texte;
        void setEnd(const std::string &);
        //écrit dans le texte à partir de l'indice voulue
        void set(const std::string &,size_t);
        //supprime le contenu du fichier;
        void erase();

        size_t getSize() const;
        size_t getMaxSize() const;
        static size_t getDefSize();
        void setMaxSize(size_t s);
};

//Génerateur de nombre aléatoire se greffe à un texte existant en le rempli.
class RandomText : public Text{
    using Text::Text;

    public:
        //ramplissage d'un text avec un string aléatoire
        static void randText(Text &, const size_t & s);
        //version multi thread
        static void randTextT(Text &, const size_t & s);
        //Permet de définir le nombre de threads a utiliser
        static void setNbThreads(const uint &);

    private:
        //nombre de threads du programe
        static uint nbt;
        //constructeur
        RandomText() = delete;
        RandomText(const RandomText &) = delete;
        RandomText(Text &);

        //Génération d'un caractère aléatoire
        static char randChar(std::uniform_int_distribution<short> &, std::default_random_engine &);
        //Générationd d'un string aléatoire d'une longeur voulue
        static void randString(const size_t &, std::string &);

        friend class Prog;
};

#endif
