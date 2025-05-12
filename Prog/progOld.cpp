#include "prog.h"
#include "../Exception/exception.h"

using namespace std;

Prog* Prog::instance = NULL;

void Prog::init(const int & argc, char** str){
    if(instance == NULL){
    instance = new Prog(argc, str);
    }
}

Prog::Prog(const int & argc, char** str){
    for(int i=1; i<argc; i++){
        args.push_back(string(str[i]));
    }
    //for(string a: args){cout << a << endl;}
    auto addOption = [](string name, string fullName, string message) -> Option{
        Option o;
        o.name = name;
        o.fullName = fullName;
        o.message = message;
        return o;
    };
    options.emplace("memory",addOption("-m","--memory", "[size_t] Quantité de mémoire vive actuellement disponible en Mo."));
    options.emplace("threads",addOption("-t","--threads", "[size_t] nombre de threads du processeur à utiliser."));
    options.emplace("help",addOption("-h","--help", "Affiche l'aide."));
    //options.emplace("config",addOption("-c", "--config", "Charge les paramètres du fichier de configuration."));
    options.emplace("generate",addOption("-g", "--generate", "[size_t s] Rempli le fichier 'path' une chaine de 's' Ko."));
    options.emplace("generateCopie", addOption("-gc", "--generateCopie", "[string path] [size_t] Génére une chaine 1 Mo puis créé un chaine de 's' Mo avec des copies de la chaine créé."));

}

Prog::~Prog(){
    delete instance;
}

void Prog::loadOption(){
    if(args.size()==0) return;
    for(uint i = args.size(); i > 0; i--){
        for(auto o: options){
            if((o.second.name==args[i-1])||(o.second.fullName==args[i-1])){
                if(o.first == "help")help(i-1);
                if(o.first == "memory") memory(i-1);
                if(o.first == "threads") threads(i-1);
                if(o.first == "generate") generate(i-1);
            }
        }
    }
    ProgException::postLoopCheck(args, source_location::current());
}

void Prog::loadHelp(const uint & i){
    args.erase(args.begin()+i);
    options["help"].isInit = true;
}

void Prog::executeHelp(){
    cout << "\n- - - - - - - - - -\nOptions disponibles :\n- - - - - - - - - -\n";
    for(auto & o: options){
            cout << "   " << o.second.name << "   " << o.second.fullName << "   " << o.second.message << "\n";
    }
    cout << "- - - - - - - - - -\n";
}

void Prog::loadThreads(const uint & i){
        bool b = (i+1)<args.size();
        auto arg = [&](vector<string> args)->string {if(b){return args[i+1];}else{return string();}};
        ProgException::argsCheck(b,arg(args), "threads", source_location::current());
        if(b){
            options["threads"].valueInt = stoul(args[i+1]);
            args.erase(args.begin()+i+1);
            options["threads"].isInit = true};
        else{
            options["threads"].valueInt = 200;}
        args.erase(args.begin()+i+1);
        }
    }
}
void Prog::threads(const uint & i){
    for(size_t i=0; i<args.size(); i++){
        if((args[i]==options["threads"].name) || (args[i]==options["threads"].fullName)){
            bool b = (i+1)<args.size();
            auto arg = [&](vector<string> args)->string {if(b){return args[i+1];}else{return string();}};
            ProgException::argsCheck(b,arg(args), "threads", source_location::current());
            if(b){
                options["threads"].valueInt = stoul(args[i+1]);
                cout << "GOOD \n";
            }
            else{
                options["threads"].valueInt = 200;}
        }
    }
}

void Prog::memory(const uint & i){
    for(size_t i=0; i<args.size(); i++){
        if((args[i]==options["memory"].name) || (args[i]==options["memory"].fullName)){
            bool b = (i+1)<args.size();
            auto arg = [&](vector<string> args)->string {if(b){return args[i+1];}else{return string();}};
            ProgException::argsCheck(b,arg(args), "memory", source_location::current());
            if(b){
                options["memory"].valueInt = stoul(args[i+1]) _Mo;
                cout << "GOOD \n";
            }
            else{
                options["memory"].valueInt = 2 _Go;}
        }
    }
}

void Prog::generate(const uint & i){
    for(size_t i=0; i<args.size(); i++){
        if((args[i]==options["generate"].name) || (args[i]==options["generate"].fullName)){
            // On vérifi qu'il y a assez d'arguments
            bool b = (i+2)<args.size();
            auto arg = [&](vector<string> args)->string {if(b){return args[i+2];}else{return string();}};
            ProgException::argsCheck(b,arg(args), "generate", source_location::current());
            if(b){
                options["generate"].valueInt = stoul(args[i+2]) _Ko;
                cout << "GOOD \n";
            }
            else{
                options["generate"].valueInt = 1 _Mo;}
            options["generate"].valueStr = args[i+1];
        }
    }
}

void Prog::calculs(){
    if(options["generate"].isInit){
        cout << "- - - - - - - - - -\n   Lancement de la génération aléatoire\n";
        Text rt(options["generate"].valueStr, options["memory"].valueInt/4);
        RandomText::nbt = options["threads"].valueInt;
        RandomText::randTextT(rt,options["generate"].valueInt);
    }
}

void Prog::execute(const int & argc, char** str){
    if(!instance) init(argc, str);
    instance->loadOption();
    instance->affiche();
    instance->calculs();
}

void Prog::affiche(){
    cout << "- - - - - - - - - -\nLancement du programme avec la cofigaration suivante:\n\n";
    for (auto o : options){
        cout << o.first
            << "   " << [&](){if(o.second.isInit)return "true";return "false";}()
            << " " << o.second.valueStr
            << " " << [&](){if(o.second.valueInt != 0)return to_string(o.second.valueInt);return string();}()
            << endl ;
    }
    cout <<"- - - - - - - - - -\n";
}
