#include <iostream>
#include <string.h>
using namespace std;

#define MAX_NUM_OF_ARGS 5
#define MAX_ARG_SIZE 256
#define DEFAULT_ARG "defaultArg"
  
int main(int argc, char **argv, char **envp) 
{ 
    char argumentArray[MAX_NUM_OF_ARGS][MAX_ARG_SIZE];
    int i;
    for (i = 0; i < MAX_NUM_OF_ARGS; ++i) {
        if (i > argc || argv[i] == NULL) {
            strcpy(argumentArray[i], DEFAULT_ARG);
        }
        else {
            strcpy(argumentArray[i], argv[i]);
        }
        cout << "argumentArray[" << i << "]: " << argumentArray[i] << "\n";
    }
    if (argc > MAX_NUM_OF_ARGS) {
        cout << "Your arguments exceeded the maximum number of arguments: " << MAX_NUM_OF_ARGS << "\n";
    }
    return 0; 
} 