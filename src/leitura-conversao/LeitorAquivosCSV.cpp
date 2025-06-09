#include <iostream>
#include <fstream>

using namespace std;

class LeitorAquivosCSV
{
    private:

    public:
    LeitorAquivosCSV(string nome);
    ifstream getArq();
};