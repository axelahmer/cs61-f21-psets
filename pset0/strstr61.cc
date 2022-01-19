#include <cstring>
#include <cassert>
#include <cstdio>

char* mystrstr(const char* s1, const char* s2) {

    // Array version:
    //
    // if (s2[0]=='\0'){
    //     return (char*)s1;
    // }
    // for(int i=0; s1[i]!='\0'; i++){
    //     int j = 0;
    //     int k = i;
    //     while (s1[k]==s2[j])
    //     {
    //         if (s1[k]=='\0'){
    //             return (char*)&s1[i];
    //         }
    //         k++;
    //         j++;
    //     }
    // }
    // return nullptr;

    // Pointer version:
    if (!*s2){
        return (char*)s1;
    }

    // i iterates over s1
    for (size_t i = 0; *(s1+i); i++)
    {
        // j iterates over s2
        size_t j = 0;
        while(*(s1+i+j) == *(s2+j)){
            if(!*(s2+j)){
                return (char*)(s1+i);
            }
            j++;
        }
    }
    return nullptr;
    
}

int main(int argc, char* argv[]) {
    assert(argc == 3);
    printf("strstr(\"%s\", \"%s\") = %p\n",
           argv[1], argv[2], strstr(argv[1], argv[2]));
    printf("mystrstr(\"%s\", \"%s\") = %p\n",
           argv[1], argv[2], mystrstr(argv[1], argv[2]));
    assert(strstr(argv[1], argv[2]) == mystrstr(argv[1], argv[2]));
}
