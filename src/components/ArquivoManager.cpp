#include <iostream>
#include <fstream>

using namespace std;

class Arquivo
{
    private:
    fstream arquivo;
    string nomeArquivo;

    public:
    Arquivo(string nome);
    ~Arquivo();
    void ordenarArquivoDecrescente();
};