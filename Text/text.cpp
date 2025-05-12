#include "text.h"
#include "../Exception/exception.h"

using namespace std;

/*
 * méthodes la classe Text
 */

Text::Text(const string & p): Text(p,getDefSize()){
}

Text::Text(const string & p, const size_t & s): path(p), maxSize(s), index(0), change(false){
    file.open(path, fstream::in|fstream::out);
    TextException::constructorCheck(file.is_open(), path, maxSize, source_location::current());
    fileSize.reset(new size_t(filesystem::file_size(path)));
    //cout <<"\n" << "Text constrction call " << maxSize << " " << fileSize << "\n";
}

Text::Text(const Text & t): 
    path(t.path), maxSize(t.maxSize),  index(0), change(false){
    file.open(path, fstream::in|fstream::out);
    //fileSize.reset(new size_t);
    fileSize = t.fileSize;
}

Text Text::operator=(const Text & t){
    if(&t != this){
        Text newText(t);
        return newText;}
    return *this;
}

Text::~Text(){
    file.close();
}

const string & Text::get(size_t pos){
    TextException::accessCheck(pos,this,source_location::current());
    if((index!=pos)||(data.size()==0)||change){
        data = string();
        index = pos;
        size_t maxPos = min(getSize(), pos+maxSize);
        file.seekg(pos);
        char c;
        for(size_t i = pos; i < maxPos;i++){
            if (!file.get(c)) break;
            data += c;
        }
    }
    // Nettoyage des caractères de fin : '\n', '\r', '\0'
    while (!data.empty() && (data.back() == '\n' || data.back() == '\r' || data.back() == '\0')) {
    data.pop_back();
    }
    return data;
}

const string & Text::get(){
    return get(0);
}

void Text::set(const std::string & s,size_t pos){
    TextException::setCheck(s.size(), this, source_location::current());
    TextException::accessCheck(pos,this,source_location::current());
    change = true;
    file.seekp(pos);
    file.write(s.data(), min(s.size(),maxSize));
    file.flush();
    size_t fin = max(getSize(), pos + min(s.size(),maxSize));
    if(getSize() < fin){*fileSize = fin;}
}

void Text::set(const std::string &s){
    set(s, 0);
}

void Text::setEnd(const std::string &s){
    set(s, getSize());
}

void Text::erase(){
    file.close();
    file.open(path, fstream::out|fstream::trunc);
    file << "";
    file.close();
    file.open(path, fstream::in|fstream::out);
    *fileSize = 0;
    index = 0;
    change = true;
}

size_t Text::getSize() const{
    return *fileSize;
}

size_t Text::getMaxSize() const{
    return maxSize;
}

size_t Text::getDefSize(){
    return maxSizeDefault;
}

void Text::setMaxSize(size_t s){
     //A faire: mettre un warning si s == 0 et ne pas modifier
     data = string();
     maxSize = s;
}


