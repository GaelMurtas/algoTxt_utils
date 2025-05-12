#include "prog.h"
#include "../Exception/exception.h"
#include "../KMP/KMP.h"

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
    loadOption();
    //for(string a: args){cout << a << endl;}
    /*auto addOption = [](string name, string fullName, string message) -> Option{
        Option o;
        o.name = name;
        o.fullName = fullName;
        o.message = message;
        return o;
    };

    options.emplace("memory",addOption("-m","--memory", "[size_t] Quantité de mémoire vive actuellement disponible en Mo."));
    options.emplace("threads",addOption("-t","--threads", "[size_t] nombre de threads du processeur à utiliser."));
    
    //options.emplace("config",addOption("-c", "--config", "Charge les paramètres du fichier de configuration."));
    options.emplace("generate",addOption("-g", "--generate", "[size_t s] Rempli le fichier 'path' une chaine de 's' Ko."));
    options.emplace("generateCopie", addOption("-gc", "--generateCopie", "[string path] [size_t] Génére une chaine 1 Mo puis créé un chaine de 's' Mo avec des copies de la chaine créé."));*/

}

Prog::~Prog(){
    delete instance;
}

void Prog::loadOption(){
    //initialisation de chaque option disponnible, définition en fin de fichier
    initHelp();
    initThreads();
    initMemory();
    initTexte();
    initMot();
    initGenerate();
    initCopie();
    initRecherche();
    if(args.size()==0) return;
    for(uint i = args.size(); i > 0; i--){
        for(auto o: options){
            if((o.second.name==args[i-1])||(o.second.fullName==args[i-1])){
                o.second.load(i-1);
            }
        }
    }
    ProgException::postLoopCheck(args, source_location::current());
}



/*void Prog::calculs(){
    if(options["generate"].isInit){
        cout << "- - - - - - - - - -\n   Lancement de la génération aléatoire\n";
        Text rt(options["generate"].valueStr, options["memory"].valueInt/4);
        RandomText::nbt = options["threads"].valueInt;
        RandomText::randTextT(rt,options["generate"].valueInt);
    }
    if(options["recherche"].isInit){
        cout << "- - - - - - - - - -\n   Lancement de la recherche de motifs\n";
        Text texteRecherche(options["texte"].valueStr, options["memory"].valueInt/4);
        Text motRecherche(options["mot"].valueStr, options["memory"].valueInt/4);
        KMP::setNbThreads(options["threads"].valueInt);
        KMP::setMemory(options["memory"].valueInt);
        KMP::getInstance(texteRecherche, motRecherche).calcul();
    }
}*/

void Prog::execute(const int & argc, char** str){
    if(!instance) init(argc, str);
    instance->affiche();
    //les options doivent être exécuter dans un ordre précis
    instance->options["help"].execute();
    instance->options["memory"].execute();
    instance->options["threads"].execute();
    instance->options["mot"].execute();
    instance->options["texte"].execute();
    instance->options["generate"].execute();
    instance->options["copie"].execute();
    instance->options["recherche"].execute();
}

void Prog::affiche(){
    cout << "- - - - - - - - - -\nLancement du programme avec la cofigaration suivante:\n- - - - - - - - - -\n";
    for (auto o : options){
        cout <<"   "<< o.first
            << "   " << [&](){if(o.second.isInit)return "true";return "false";}()
            << " " << o.second.valueStr
            << " " << [&](){if(o.second.valueInt != 0)return to_string(o.second.valueInt);return string();}()
            << endl ;
    }
    cout <<"- - - - - - - - - -\n";
}

void Prog::initHelp(){
    Option o;
    o.name = "-h";
    o.fullName = "--help";
    o.message = "Affiche l'aide.";
    o.load = [&](const uint & i){
        args.erase(args.begin()+i);
        ProgException::postLoadCheck(args, i, source_location::current());
        options["help"].isInit = true;
    };
    o.execute = [&](){
        //cout << "\nHELP\n";
        if(!options["help"].isInit) return;
        cout << "\n- - - - - - - - - -\nOptions disponibles :\n- - - - - - - - - -\n";
        for(auto & o: options){
            cout << "   " << o.second.name << "   " << o.second.fullName << "   " << o.second.message << "\n";
        }
        cout << "- - - - - - - - - -\n";
    };
    options.emplace("help",o);
}

void Prog::initThreads(){
Option o;
    o.name = "-t";
    o.fullName = "--threads";
    o.message = "[size_t] nombre de threads du processeur à utiliser.";
    o.valueInt = 8;
    o.load = [&](const uint & i){
        bool b = (i+1)<args.size();
        auto arg = [&](vector<string> args)->string {if(b){return args[i+1];}else{return string();}};
        ProgException::argsCheck(b,arg(args), "threads", source_location::current());
        if(b){
            options["threads"].valueInt = stoul(args[i+1]);
            args.erase(args.begin()+i+1);
            options["threads"].isInit = true;}
        args.erase(args.begin()+i);
        ProgException::postLoadCheck(args, i, source_location::current());
    };
    o.execute = [&](){
        //cout <<"\nTHREADS\n";
        RandomText::setNbThreads((uint)options["threads"].valueInt);
        KMP::setNbThreads((uint)options["threads"].valueInt);
    };
    options.emplace("threads",o);
}

void Prog::initMemory(){
    Option o;
    o.name = "-m";
    o.fullName = "--memory";
    o.message = "[size_t] Quantité de mémoire vive actuellement disponible en Mo.";
    o.valueInt = 2000;
    o.load = [&](uint i){
        bool b = (i+1)<args.size();
        auto arg = [&](vector<string> args)->string {if(b){return args[i+1];}else{return string();}};
        ProgException::argsCheck(b,arg(args), "memory", source_location::current());
        if(b){
            options["memory"].valueInt = stoul(args[i+1]);
            args.erase(args.begin()+i+1);
            options["memory"].isInit = true;}
        args.erase(args.begin()+i);
        ProgException::postLoadCheck(args, i, source_location::current());
    };
    o.execute = [&](){
        //cout << "\nMEMORY\n";
        ProgException::memoryCheck(options["memory"].valueInt _Mo, source_location::current());
        KMP::setMemory(options["memory"].valueInt _Mo);
    };
    options.emplace("memory",o);
}

void Prog::initGenerate(){
Option o;
    o.name = "-g";
    o.fullName = "--generate";
    o.message = "[string path] [size_t s] Rempli le fichier 'path' une chaine de 's' Ko.";
    o.valueStr = "DATA/randText_KMPprog";
    o.valueInt = 100;
    o.load = [&](const uint & i){
        if((args[i]==options["generate"].name) || (args[i]==options["generate"].fullName)){
            // On vérifi qu'il y a assez d'arguments
            bool b = (i+2)<args.size();
            auto arg = [&](vector<string> args)->string {if(b){return args[i+2];}else{return string();}};
            ProgException::argsCheck(b,arg(args), "generate", source_location::current());
            if(b){//prise en compte de l'argument numérique
                options["generate"].valueInt = stoul(args[i+2]) _Ko;
                args.erase(args.begin()+i+2);}
            if((i+1)<args.size()){
                options["generate"].valueStr = args[i+1];
                args.erase(args.begin()+i+1);}
            args.erase(args.begin()+i);
            ProgException::postLoadCheck(args, i, source_location::current());
            options["generate"].isInit = true;}
    };
    o.execute = [&](){
        //cout << "\nGENERATE\n";
        if(!options["generate"].isInit) return;
        cout << "- - - - - - - - - -\nLancement de la génération du texte aléatoire\n- - - - - - - - - -\n";
        auto t1 = chrono::high_resolution_clock::now();
        Text t(options["generate"].valueStr, options["memory"].valueInt _Mo);
        RandomText::randTextT(t, options["generate"].valueInt);
        auto t2 = chrono::high_resolution_clock::now();
        chrono::duration<double, std::ratio<1>> s_double = t2 - t1;
        cout << "   génération terminé en " << s_double.count() << " secondes.\n- - - - - - - - - -\n";
    };
    options.emplace("generate",o);
}

void Prog::initCopie(){
Option o;
    o.name = "-c";
    o.fullName = "--copie";
    o.message = "[size_t s] On copie s fois le mot dans le texte a des positions aléatoires.";
    o.valueInt = 1;
    o.load = [&](const uint & i){
        bool b = (i+1)<args.size();
        auto arg = [&](vector<string> args)->string {if(b){return args[i+1];}else{return string();}};
        ProgException::argsCheck(b,arg(args), "copie", source_location::current());
        if(b){
            options["copie"].valueInt = stoul(args[i+1]);
            args.erase(args.begin()+i+1);
            options["copie"].isInit = true;}
        args.erase(args.begin()+i);
        ProgException::postLoadCheck(args, i, source_location::current());
    };
    o.execute = [&](){
        //cout << "\nCOPIE\n";
        if(!options["copie"].isInit) return;
        Text text(options["texte"].valueStr, options["memory"].valueInt _Mo);
        Text mot(options["mot"].valueStr, options["memory"].valueInt _Mo);
        vector<size_t> positions;
        random_device seed;
        default_random_engine generator(seed());
        uniform_int_distribution<size_t> uniform_dist(0, text.getSize());
        for(uint i=0; i<options["copie"].valueInt; i++){
            size_t r = uniform_dist(generator);
            string sTest = mot.get();
            text.set(sTest,r);
            //text.set(mot.get(),r);
            positions.push_back(r);
        }
        cout << "- - - - - - - - - -\nCopie de de : " << options["mot"].valueStr
            << " inséré dans " << options["texte"].valueStr
            << " aux positions suivantes :\n"
            << vecteurToString(positions)
            << "- - - - - - - - - -\n";
    };
    options.emplace("copie",o);
}

void Prog::initTexte(){
Option o;
    o.name = "-txt";
    o.fullName = "--texte";
    o.message = "[string] Chemin du fichier contenant le texte.";
    o.valueStr = "DATA/texte.txt";
    o.load = [&](const uint & i){
        bool b = (i+1)<args.size();
        //auto arg = [&](vector<string> args)->string {if(b){return args[i+1];}else{return string();}};
        if(b){
            options["texte"].valueStr = args[i+1];
            args.erase(args.begin()+i+1);
            options["texte"].isInit = true;}
        args.erase(args.begin()+i);
        ProgException::postLoadCheck(args, i, source_location::current());
    };
    o.execute = [&](){
        //cout << "\nTEXT\n";
        Text text(options["texte"].valueStr);
    };
    options.emplace("texte",o);
}

void Prog::initMot(){
Option o;
    o.name = "-w";
    o.fullName = "--mot";
    o.message = "[string] Chemin du fichier contenant le mot à chercher dans le texte.";
    o.valueStr = "DATA/mot.txt";
    o.load = [&](const uint & i){
        bool b = (i+1)<args.size();
        //auto arg = [&](vector<string> args)->string {if(b){return args[i+1];}else{return string();}};
        if(b){
            options["mot"].valueStr = args[i+1];
            args.erase(args.begin()+i+1);
            options["mot"].isInit = true;}
        args.erase(args.begin()+i);
        ProgException::postLoadCheck(args, i, source_location::current());
    };
    o.execute = [&](){
        //cout << "\nMOT\n";
        Text motTest(options["mot"].valueStr);
        ProgException::motSizeCheck(motTest.getSize(), options["memory"].valueInt _Mo, source_location::current());
    };
    options.emplace("mot",o);
}

void Prog::initRecherche(){
    Option o;
    o.name = "-r";
    o.fullName = "--recherche";
    o.message = "Recherche le mot dans le texte.";
    o.load = [&](const uint & i){
        args.erase(args.begin()+i);
        ProgException::postLoadCheck(args, i, source_location::current());
        options["recherche"].isInit = true;
    };
    o.execute = [&](){
        //cout << "\nRECHERCHE\n";
        if(!options["recherche"].isInit) return;
        Text texteRecherche(options["texte"].valueStr, options["memory"].valueInt _Mo);
        Text motRecherche(options["mot"].valueStr, options["memory"].valueInt _Mo);
        KMP::getInstance(texteRecherche, motRecherche).calcul();
        KMP::getResult();
    };
    options.emplace("recherche",o);
}

string vecteurToString(const vector<size_t> & pos){
    string s;
    for(uint i = 0; i<pos.size();){
        for(uint j = 0; j<10; j++){
            if( i==pos.size()) break;
            s += to_string(pos[i]);
            s += " ";
            i++;
        }
        s += "\n";
    }
    return s;
}
