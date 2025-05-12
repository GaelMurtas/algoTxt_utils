#ifndef _EXCEPTION
#define _EXCEPTION
#include <source_location>
#include <iostream>
#include <fstream>
#include <string>
#include "../Text/text.h"
#include "../Prog/prog.h"

class Exception {

    public:
        //addresse de l'object dans lequel l'exception est déclancher
        const void* obj;

        //localisation du déclanchement de l'exception
        std::source_location location;

        //passer a true pour ignorer complétement la gestion d'exception
        //À utiliser uniquement au cours du dévelloppement où dans certain cas précis pour accéléré un peu l'exécution
        static bool BYPASS;

        //passer a true pour renvoyer les exceptions au lieu de tenter de les gérées
        //cela donne la possibilité de façon indépendante du comportement prévu par leur classe lors de leur détections en cas de besoin
        static bool THROW;

        //chemin du fichier de LOG
        //static std::string path;
        //
    enum class ExceptionType {
        warning, //Préviens l'utilisateur d'un danger potentiel le programme tante de continuer sans changement
        error, //on tente de continuer l'exécution mais après avoir géré l'exception en moditiant un variable par exemple
        fatal_error //on arrête immédiatement l'exécution
    };


    protected :
        std::string message;

        ExceptionType type;

        //construction interdite le constructeur sera seulement appeller par les classes filles
        Exception(const void*, const std::source_location &, const ExceptionType &);
        Exception() = delete;

        //afficahge des information de l'erreur dans le terminal et dans un fichier de LOG
        void log();

        //afficahge des information de l'erreur dans un fichier de LOG - À FAIRE -
        //void toLOG();
};

/*
 * Exceptions spécifiques à la classe text
 */
namespace TextException
{
    void constructorCheck(const bool &, const std::string &, size_t &, const std::source_location &);
    void accessCheck(size_t &, const Text *, const std::source_location &);
    void setCheck(const size_t &, const Text *, const std::source_location &);

    class FileError : Exception{
        using Exception::Exception;
        std::string path;
        FileError(const void*, const std::source_location &, const ExceptionType &, const std::string &);
        friend void constructorCheck(const bool &, const std::string &, size_t &, const std::source_location &);
    };

    class InputError : Exception{
        using Exception::Exception;
        size_t input;
        InputError(const void*, const std::source_location &, const ExceptionType &, const size_t &);
        friend void constructorCheck(const bool &, const std::string &, size_t &, const std::source_location &);
    };

    class AcessError : Exception{
        using Exception::Exception;
        size_t input;
        AcessError(const Text *, const std::source_location &, const ExceptionType &, const size_t &);
        friend void accessCheck(size_t &, const Text *, const std::source_location &);
    };

    class SetWarning : Exception{
        using Exception::Exception;
        size_t input;
        SetWarning(const void*, const std::source_location &, const ExceptionType &, const size_t &);
        friend void setCheck(const size_t &, const Text *, const std::source_location &);
    };
}

/*
 * Exception spécifique à la classe Prog
 */
namespace ProgException
{
    //On vérifie que l'argument passé pour l'optionest utilisable
    void argsCheck(bool &, const std::string &, const std::string &, const std::source_location &);
    //On vérifi qu'il n'y a pas d'option non traiter
    void postLoopCheck(const std::vector<std::string> &, const std::source_location &);
    //Vérification qu'il y a pas d'argument enb trop après le passage d'une option
    void postLoadCheck(std::vector<std::string> &, const uint &, const std::source_location &);

    class OptionError: Exception {
        using Exception::Exception;
        std::string imput;
        std::string option;
        OptionError(const std::source_location &, const ExceptionType &, const std::string &, const std::string &);
        OptionError(const std::source_location &, const ExceptionType &, const std::string &);
        friend void argsCheck(bool &, const std::string &, const std::string &, const std::source_location &);
        friend void postLoopCheck(const std::vector<std::string> &, const std::source_location &);
        friend void postLoadCheck(std::vector<std::string> &, const uint &, const std::source_location &);
    };

    //On vérifie que la mémoire demander est bien disponible
    void memoryCheck(const size_t &, const std::source_location &);
    void motSizeCheck(const size_t &, const size_t &, const std::source_location &);

    class MemoryError: Exception {
        using Exception::Exception;
        MemoryError(const std::source_location &, const ExceptionType &, const size_t &);
        MemoryError(const std::source_location &, const ExceptionType &, const size_t &, const size_t &);
        friend void memoryCheck(const size_t &, const std::source_location &);
        friend void motSizeCheck(const size_t &, const size_t &, const std::source_location &);
    };
}

/*
 * exception spécifique au nombre de threads
 */
class ThreadsException: Exception {
    using Exception::Exception;
    ThreadsException(const void*, const std::source_location &, const ExceptionType &, const uint &);
    public:
        static bool nbThreadsCheck(const void*, const uint &, const std::source_location &);
};

#endif
