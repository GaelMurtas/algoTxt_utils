#include "text.h"
#include "../Exception/exception.h"

using namespace std;

uint RandomText::nbt = 200;

RandomText::RandomText(Text & t): Text(t){
}

char RandomText::randChar(uniform_int_distribution<short> & uniform_dist, default_random_engine & generator){
    short r = uniform_dist(generator);
    switch(r){
        case(0): return 'a';
        case(1): return 'c';
        case(2): return 'g';
        case(3): return 't';
        }
    return ' ';
}

void RandomText::randString(const size_t & s, string & str){
    random_device seed;
    std::default_random_engine generator(seed());
    std::uniform_int_distribution<short> uniform_dist(0, 3);
    for(size_t i = 0; i<s; i++){
        str.push_back(randChar(uniform_dist, generator));
    }
}

void RandomText::randText(Text & t,const size_t & s){
    RandomText text(t);
    text.erase();
    size_t m = text.getMaxSize();
    if((s/m)!=0){
        for(size_t i = 0; i < (s/m); i++){
            text.randString(m, text.data);
            text.setEnd(text.data);
            text.data = string();
    //cout << endl << "size : " << text.getSize() << endl;
        }
    }
    text.randString(s%m, text.data);
    text.setEnd(text.data);
    //cout << endl << "size : " << text.getSize() << endl;
    //cout << endl << "string : " << text.data << endl;
}

void RandomText::randTextT(Text & t,const size_t & s){
    size_t m = t.getMaxSize();
    size_t tmp = m/nbt;
    if(tmp == 0){
        randText(t,s);
        return;
    }
    RandomText text(t);
    text.erase();
    if((s/m)!=0){
        for(size_t j = 0; j < (s/m); j++){
            vector<thread> threads(nbt);
            vector<string> STR(nbt);
            for (size_t i = 0; i < nbt; i++){
                threads[i] = thread(randString, ref(tmp), ref(STR[i]));
            }
            for (thread & T: threads){
                T.join();
            }
            for (string& str: STR){
                text.data += str;
            }
            if(m%nbt>0){
                text.randString(m%nbt,text.data);
            }
            text.setEnd(text.data);
            text.data = string();
            //cout << endl << "size : " << text.getSize() << endl;
        }
    }
    if(s%m != 0){
        text.randString(s%m, text.data);
        text.setEnd(text.data);
        text.data = string();
    }
    //cout << endl << "size : " << text.getSize() << endl;
}

void RandomText::setNbThreads(const uint & t){
    bool validImput = ThreadsException::nbThreadsCheck(NULL, t, source_location::current());
    if(validImput) nbt = t;
}
