#include "KMP.h"
#include "../Exception/exception.h"

using namespace std;

//initialisation des membres statiques
KMP* KMP::instance = NULL;
uint KMP::nbt = 200;
size_t KMP::memory = (size_t)32 _Go;
bool  KMP::working = false;

KMP& KMP::getInstance(const Text & local_text, const Text & local_mot){
    if(instance == NULL){
    instance = new KMP(local_text, local_mot);
    }
    else{
        while(working){
            this_thread::sleep_for(std::chrono::seconds(2));
            }
        instance->text = local_text;
        instance->mot = local_mot;
    }
    return *instance;
}

KMP::KMP(const Text & local_text, const Text & local_mot): text(local_text), mot(local_mot){
}

KMP::~KMP(){
    delete instance;
}

void KMP::setNbThreads(uint t){
    bool validInput = ThreadsException::nbThreadsCheck(instance, t, source_location::current());
    if(validInput) nbt = t;
}

void KMP::setMemory(size_t m){
    ProgException::memoryCheck(m, source_location::current());
    memory = m;
}

void KMP::calcul(){
     //juste pour les tests
        //testRecherche();
        //getResult();

    while(working){
        this_thread::sleep_for(std::chrono::seconds(2));
        }
    working = true;
    calculOverlap();
    recherche();
    working = false;
}

vector<size_t> KMP::getResult(){
    cout << "- - - - - - - - - -\n"
        "résultat de la recherche :\n"
        "- - - - - - - - - -\n"
        "   nombre d'itération :\n    "
        << instance->result.size()
        << "\n   Position des itérations :\n   "
        << [&](){string s;
                for(uint i = 0; i<instance->result.size(); i++){
                   s += " ";
                   s += to_string(instance->result[i]);
                   if((!(i%10))&&(i!=0))s += "\n";
                }
                s += "\n";
                return s;
        }()
        << "- - - - - - - - - -\n";
        return instance->result;
}

void KMP::calculOverlap(){
    string_view word = mot.get();
    //cout << "test in TEXT::GET "  << word.size() <<   endl;
    overlap.resize(word.size(),-1);
    long int k=-1;
    for(size_t i = 1; i<(word.size()); i++){
        while((k> -1)&&(word[k+1]!=word[i])){
            k = overlap[k];
        }
        if(word[k+1]==word[i]){
            k++;
        }
        overlap[i] = k;
    }
    //Affichage pour tests
    /*cout << "le mot est : " << word << endl;
    cout << "Overlap :\n";
    for(long int x: overlap){
         cout << x << " ";
    }
    cout << endl;*/
}

void KMP::rechercheCore(const string & txt, const string & word, size_t pos){
     size_t txtSize = txt.size();
     long wordSize = word.size();
     long q = -1;
     for(size_t i=0; i<txtSize; ++i){
          //cout << "les valeurs des variables sont " << i << " " << q << endl;
          while ((q>=0)&&(word[q+1]!=txt[i])){
               q = instance->overlap[q];
          }
          if (word[q+1]==txt[i]){
               ++q;
          }
          if (q+1 == wordSize){
               instance->result.push_back(pos+i-q);
               q = instance->overlap[q];
          }
     }
}

/*void KMP::recherche(){
     result.clear();
     string word = mot.get();
     //On veut s'assurer que le nombre de  thread convient par rapport à la taille des inputs et la mémoire disponible
     while((memory<=(2*mot.getSize()*nbt)) && (nbt>1)) --nbt;
     //calcul du nombre de divisions à faire sur le texte
     size_t nbParts = (size_t)(text.getSize()/(memory-2*nbt*mot.getSize()))+1;
     size_t partsLength = [&]()->size_t{
          if(text.getSize()%nbParts == 0){return text.getSize()/nbParts;}
          return 1+(text.getSize()/nbParts);
     }() + 2*mot.getSize() - 2;
     text.setMaxSize(partsLength);
     for( size_t pos = 0; pos < text.getSize();){
          vector<thread> threads(nbt);
          vector<string> STR(nbt);
          for(size_t t = 0; ((t<nbt)&&(pos<text.getSize())); ++t){
               STR[t] = text.get(max((long int)0,(long int)(pos-mot.getSize()+1)));
               threads[t] = thread(rechercheCore, ref(STR[t]), ref(word), pos);
               pos += partsLength;
          }
          for (thread & T: threads){
                if (T.joinable()) T.join();
          }
     }
}*/

void KMP::recherche(){
     result.clear();
     string word = mot.get();
     std::cout << "[INFO] Taille du mot à chercher : " << word.size() << "\n";
     std::cout << "[INFO] Taille totale du texte : " << text.getSize() << "\n";
 
     // Ajustement du nombre de threads en fonction de la mémoire
     while ((memory <= (2 * mot.getSize() * nbt)) && (nbt > 1)) {
         std::cout << "[DEBUG] Réduction du nombre de threads : nbt = " << nbt << " -> " << (nbt - 1) << "\n";
         --nbt;
     }
     //variables globales de l'algorithme
     size_t motSize = mot.getSize();
     size_t textSize = text.getSize();
     // Calcul du nombre de parties
     //division minimale nécessaire pour éviter les dépassement mémoire
     size_t nbParts = (size_t)(textSize / (memory - 2 * nbt * motSize)) + 1;
     std::cout << "[INFO] Nombre de segments prévus : " << nbParts << "\n";
     //prise en compte du nombre de thread
     nbParts = nbParts*nbt;
     std::cout << "[INFO] Nombre de segments prévus : " << nbParts << "\n";
     //longueur de chaques parties de texte à considéré
     size_t partsLength = [&]() -> size_t {
         size_t base = textSize / nbParts;
         return (textSize % nbParts == 0 ? base : base + 1);
     }() + 2 * motSize - 2;
     std::cout << "[INFO] Longueur des segments (avec recouvrement) : " << partsLength << "\n";
 
     //à remplacer part setMaxSize(la longueur de la grosse partie) pour rendre l'algo plus rapide
     //text.setMaxSize(partsLength);
 
     //boucle principale on découpe dans un premier temps le texte en parts adaptés à la mémoire à utiliser
     //la variables pos nous donne la position début du segment à analyse dans le texte
     for (size_t pos = 0; pos < textSize;) {
         std::cout << "\n[LOOP] Position de départ dans le texte : " << pos << "\n";
         //variables locales
         vector<thread> threads(nbt);
         vector<string> STR(nbt);
         //position du premier caractère à lire dans le texte en début de boucle
         size_t partStart = (size_t)std::max((long)0, (long)(pos - motSize + 1));
         //lecture sur le disque
         string textPart = text.get(partStart);
         //boucle secondaire gérant le multi-thread
         for (size_t t = 0; (t < nbt) && (pos < textSize); ++t) {
             std::cout << "[THREAD " << t << " loop begin\n";
             //position dans le texte en début de boucle
             size_t textPosStart = std::max((long)0, (long)(pos - motSize + 1));
             //position dans textPart en début de boucle
             size_t segmentStart = (size_t)std::max((long)0, (long)(pos - motSize + 1 - partStart));
             //ségment à analyser dans ce thread
             STR[t] = textPart.substr(segmentStart, partsLength);
             std::cout << "[THREAD " << t << "] Lecture texte à partir de : " << segmentStart
                       << " (taille effective : " << STR[t].size() << ")\n";
             //lancement de l'algo KMP sur le segment voulu
             threads[t] = thread(rechercheCore, ref(STR[t]), ref(word), textPosStart);
             std::cout << "[THREAD " << t << "] Démarré avec position d'origine : " << pos << "\n";
             pos += partsLength;
         }
         for (size_t t = 0; t < threads.size(); ++t) {
             if (threads[t].joinable()) {
                 threads[t].join();
                 std::cout << "[THREAD " << t << "] Terminé.\n";
             }
         }
     }
     std::cout << "\n[INFO] Recherche terminée. Résultats trouvés : " << result.size() << "\n";
 }
 

//algorithme naïf utiliser pour tester d'autres algorithme
void KMP::testRecherche(){
     result.clear();
     string word = mot.get();
     string txt = text.get();
     //cout << "mot  : " << word << endl;
     //cout << "texte: " << txt << endl;
     //cout << "wordSize: " << word.size() << ", txtSize: " << txt.size() << endl;
     for(size_t i=0; (i+word.size())<=txt.size(); ++i){
          size_t j=0;
          while((j<word.size())&&(word[j]==txt[(i+j)])){
               ++j;
          }
          if (j==word.size()){
               //cout << "Occurence trouvé à la position " << i << endl;
               result.push_back(i);
          }
     }
}
