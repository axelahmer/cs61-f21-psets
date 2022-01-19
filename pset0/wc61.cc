#include <cstdio>
#include <cctype>

int main(){
    unsigned long nWords=0, nLines=0, nBytes = 0;
    bool prevWasWord = false;
    int c = fgetc(stdin);

    while(c != EOF){
        if(isspace(c)){
            if(prevWasWord){
                nWords++;
                prevWasWord=false;
            }
            if(c=='\n'){
                nLines++;
            }
        }else{
            prevWasWord=true;
        }
        nBytes++;
        c = fgetc(stdin);
    }
    if(prevWasWord){
        nWords++;
    }
    fprintf(stdout,"%8lu %7lu %7lu\n",nLines,nWords,nBytes);

    return 0;
}