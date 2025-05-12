#include "exception.h"

using namespace std;

//Intialisaétion des membres statiques
bool Exception::BYPASS = false;
bool Exception::THROW = false;
//string path = "../LOG";

Exception::Exception(const void* ptr, const std::source_location & lc, const ExceptionType & t): obj(ptr), location(lc), type(t) {
}

void Exception::log(){
    cout << "\n- - - - - - - - - -" << endl;
    string s = " WARNING ";
    switch (type){
        case (ExceptionType::error) :
            s = " ERROR ";
            break;
        case (ExceptionType::warning) :
            break;
        case (ExceptionType::fatal_error) :
            s = " FATAL_ERROR ";
    }
    cout << s << endl;
    cout << "- - - - - - - - - -" << endl;
    cout << "fichier : " << location.file_name() << endl;
    cout << "fonction : " << location.function_name() << endl;
    cout << "ligne : " << location.line() << endl;
    if(obj) cout << "adresse de l’object : " << &obj << endl;
    cout << "- - - - - - - - - -" << endl;
    cout << message << endl;
    cout << "- - - - - - - - - -" << endl;
}

using type = Exception::ExceptionType ;

/*
 * Spécifique à la classe Text
 */

void TextException::constructorCheck(const bool & open,const string & p, size_t & s, const std::source_location & l){
    if(Exception::BYPASS) return;
    try{
        if(s<1){
            InputError e(NULL,l,type::error,s);
            e.log();
            throw e;
        }
    }
    catch(InputError){
        if(Exception::THROW) throw;
        s = Text::getDefSize();
    }
    try{
        if (!open) {
            FileError e(NULL,l,type::fatal_error,p);
            e.log();
            throw e;
        }
    }
    catch(FileError){
        if(Exception::THROW) throw;
        exit(-1);
    }
}

TextException::FileError::FileError(const void* ptr, const std::source_location & location, const ExceptionType & t, const std::string & s):
    Exception(ptr,location,t), path(s){
    message = "Accès au fichier ";
    message += path;
    message  += " impossible.";
}

TextException::InputError::InputError(const void* ptr, const std::source_location & location, const ExceptionType & t, const size_t & s):
    Exception(ptr,location,t), input(s){
    message = "Paramètre de constructeur invalide l'argument \"maxSize\" doit être supérieur à 1. \nImput value : ";
    message += to_string(input);
    message  += "\nmaxSize rétablie à sa valeur par défault.";
}

void TextException::accessCheck(size_t & pos, const Text * t, const std::source_location & l){
    if(Exception::BYPASS) return;
    try{
        if(pos > t->getSize()){
            AcessError e(t,l,type::error,pos);
            e.log();
            throw e;
        }
    }
    catch(AcessError){
        if(Exception::THROW) throw;
        pos = t->getSize();
    }
}

TextException::AcessError::AcessError(const Text * ptr, const std::source_location & l, const ExceptionType & t, const size_t & pos):
    Exception(ptr,l,t), input(pos){
        message = "Impossible d'accéder à la position ";
        message += to_string(input);
        message += " du texte.\nEn effet le  texte est de taille : ";
        message += to_string(ptr->getSize());
        message += ".\nCurseur positioner en fin de texte et chaine vide renvoyer";
    }

void TextException::setCheck(const size_t & s, const Text * t, const std::source_location & l){
    if(Exception::BYPASS) return;
    try{
        if((s > t->getMaxSize())||(s==0)){
            SetWarning e(t,l,type::warning,s);
            e.log();
            throw e;
        }
    }
    catch(SetWarning){
        if(Exception::THROW) throw;
    }
}

TextException::SetWarning::SetWarning(const void* ptr, const std::source_location & l, const ExceptionType & t, const size_t & s):
    Exception(ptr,l,t), input(s){
        message = "Donné d'entré de taille ";
        message += to_string(input);
        if(s>0){
            message += ".\nSeul le début de la donné sera écrite car sa taille est supérieur au maximum pou cet objet.";
        }
}

/*
 * Exception spécifique à la classe Prog
 */
void ProgException::argsCheck(bool & b, const std::string & arg, const std::string & opt, const std::source_location & l){
    // b est true si il y a assez d'argument pour l'option
    if(Exception::BYPASS) return;
    try{stoul(arg);}
    catch(invalid_argument const &){
        b = false;
    }
    if(b){b = b && (stoul(arg)!=0);}
    if(b){return;}
    try{
        OptionError e(l, type::error, arg, opt);
        e.log();
        throw e;
    }
    catch(OptionError){
        if(Exception::THROW) throw;
    }
}

ProgException::OptionError::OptionError( const std::source_location & l, const ExceptionType & t, const std::string & arg, const std::string & opt) :
    Exception(NULL, l, t){
        message = "Argument '";
        message += arg;
        message += "' invalide pour l'option '";
        message += opt;
        message += "'.\nUne valeur par défault sera utiliser. Enter l'option '-h' pour afficher l'aide. ";
}

void ProgException::postLoopCheck(const std::vector<std::string> & args, const std::source_location & l){
    if(Exception::BYPASS) return;
    if(args.size() == 0){return;}
    try{
        OptionError e(l, type::warning, args[0]);
        e.log();
        throw e;
    }
    catch(OptionError){
        if(Exception::THROW) throw;
    }
}

void ProgException::postLoadCheck(std::vector<std::string> & args, const uint & theoric_size, const std::source_location & l){
    if(Exception::BYPASS) return;
    if(args.size() == theoric_size) return;
    try{
        OptionError e(l, type::warning, args[args.size()-1]);
        e.log();
        throw e;
    }
    catch(OptionError){
        args.erase(args.begin()+theoric_size,args.end());//on efface les arguments non reconu
        if(Exception::THROW) throw;
    }
}

ProgException::OptionError::OptionError( const std::source_location & l, const ExceptionType & t, const std::string & arg) :
    Exception(NULL, l, t){
        message = "Un ou prusieurs argument n'ont pas été reconu et seront ignoré. \n";
        message += "Argument '";
        message += arg;
        message += "' non reconnu, il sera ignorer.";
        message += "\nEnter l'option '-h' pour afficher l'aide. ";
}

void ProgException::memoryCheck(const size_t & askedMemory, const std::source_location & l){
    if(Exception::BYPASS) return;
    try{
        char * allocTest = new char[askedMemory];
        delete[] allocTest;
    }
    catch(bad_alloc const &){
        MemoryError e(l, type::fatal_error, askedMemory);
        e.log();
        if(Exception::THROW) throw e;
        exit(-1);
    }
    if (askedMemory < 1 _Mo){
        MemoryError e(l, type::warning, askedMemory);
        e.message = "La quantité de memoire demander semble un peu juste pour le bon déroulement de l'algorithme. ";
        e.log();
        if(Exception::THROW) throw e;
    }
}

ProgException::MemoryError::MemoryError(const std::source_location & l, const ExceptionType & t, const size_t & arg) :
    Exception(NULL, l, t){
        message = "La mémoire demander n'est pas disponnible\n";
        message += "Mémoire de demander en Mo : ";
        message += to_string(arg/1000000);
        message += ".";
        message += "\nEnter l'option '-m' pour entrer une valeur plus petite. ";
}

void ProgException::motSizeCheck(const size_t & wLength, const size_t & memory, const std::source_location & l){
    if(Exception::BYPASS) return;
    if((wLength*3)>memory){
        MemoryError e(l, type::warning, wLength, memory);
        e.log();
        if(Exception::THROW) throw e;
    }
}

ProgException::MemoryError::MemoryError(const std::source_location & l, const ExceptionType & t, const size_t & len, const size_t & mem) :
    Exception(NULL, l, t){
        message = "La mémoire maximale utilisable par le programme doit être au moins de 3 fois la taille du mot\n";
        message += "Mémoire utilisale en Ko : ";
        message += to_string(mem/1000);
        message += ".\ntaille du mot en Ko : ";
        message += to_string(len/1000);
        message += ".";
        message += "\nSeul le début du mot sera pris en compte. Utilisez l'option '-m' pour entrer une valeur plus grande. ";
}

/*
 * Exception spécifique au nombre de threads
 */

bool ThreadsException::nbThreadsCheck(const void* ptr, const uint & nbt, const std::source_location & l)
{
    if(Exception::BYPASS) return true;
    if((nbt>0)&&(nbt<100000)) return true;
    try{
        ThreadsException e(ptr, l, type::error, nbt);
        e.log();
        throw e;
    }
    catch(ThreadsException){
        if(Exception::THROW) throw;
        return false;
    }
}

ThreadsException::ThreadsException(const void* ptr, const source_location & l, const ExceptionType & t, const uint & nbt):
    Exception(ptr,l,t){
        message = "   Nombre de threads en entrer :\n   ";
        message += nbt;
        message += "\n   Cette valmeur est étrange, une valeur par défault sera utilisé à la place.";
}
