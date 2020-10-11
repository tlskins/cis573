#include <iostream> 
using namespace std; 
  
int main(int argc, char **argv, char **envp) 
{ 
    for (int i = 0; i < argc; ++i) 
        cout << "argv[" << i << "]: " << argv[i] << "\n"; 
  
    return 0; 
} 