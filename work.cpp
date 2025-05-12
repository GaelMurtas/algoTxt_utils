#include "Exception/exception.h"
#include "Text/text.h"
#include "Prog/prog.h"
#include "KMP/KMP.h"
#include <cstdint>


int main(int argc, char ** argv){
    // Exception::BYPASS = true;
    //using namespace std;
    //auto simplePrint = [](string s){
    //cout << s << endl;};
    //string textPath = "/home/gael/Prog/KMP_project/DATA/texte.txt";
    //Text text(textPath,100 _Mo);
    /*text.set("! première entrer !");
    simplePrint(text.get());
    text.set("! seconde entrer !",95);
    simplePrint(text.get(90));
    text.setEnd("! troisième entrer !");
    simplePrint(text.get(150));
    simplePrint(to_string(text.getMaxSize()));*/

    //RandomText::randTextT(text,1 _Go);
    //simplePrint(to_string(text.getSize()));

    Prog::execute(argc, argv);

    //test overlap
    /*Text simpleText("/home/gael/Prog/KMP_project/DATA/simpleText.txt", 100 _Mo);
    Text simpleMot("/home/gael/Prog/KMP_project/DATA/simpleMot.txt", 100 _Mo);
    KMP::getInstance(simpleText, simpleMot).calcul();
    KMP::getResult();*/

    return 0;
}
