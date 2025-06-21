#include "LeitorCSV.hpp"
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include "type_block.hpp"
#include "type_dado.hpp"

LeitorCSV::LeitorCSV(const char *caminhoCSV) 
{
    if (!caminhoCSV) {
        throw runtime_error("Caminho do CSV é nullptr");
    }

    arquivo.open(caminhoCSV);
    if (!arquivo.is_open()) {
        throw runtime_error(string("Falha ao abrir o arquivo CSV: ") + caminhoCSV);
    }

    bufferLinha.clear();
    getline(arquivo, bufferLinha);
}

LeitorCSV::~LeitorCSV()
{
    fechar();
}

bool LeitorCSV::estaAberto() const
{
    return arquivo.is_open();
}

void LeitorCSV::fechar()
{
    if (arquivo.is_open()) {
        arquivo.close();
    }
}

bool LeitorCSV::lerProximo(Registro &registroOut)
{
    if (!estaAberto()) {
        throw runtime_error("Tentativa de leitura com arquivo fechado");
        return false;
    }

    bufferLinha.clear();
    if (!getline(arquivo, bufferLinha)) {
        //throw runtime_error("Não foi possível capturar a linha no arquivo CSV");
        return false;
    }

    istringstream ss(bufferLinha);
    string campo;

    try {
        string reference;
        float chavePrimaria = 0.0f;
        float dataValue = 0.0f;
        string status;
        string units;
        int magnitude = 0;
        string subject;
        string group;
        string titulos[4];

        for (int i = 0; i < 13; ++i) {
            if (!getline(ss, campo, ',')) {
                throw runtime_error("Linha incompleta no CSV");
            }

            // Remove aspas se existirem
            if (campo.front() == '"' && campo.back() == '"') {
                campo = campo.substr(1, campo.size()-2);
            }

            switch (i) {
                case 0: reference = campo; break;
                case 1: chavePrimaria = stof(campo); break;
                case 2: dataValue = stof(campo); break;
                case 3: status = campo; break;
                case 4: units = campo; break;
                case 5: magnitude = stoi(campo); break;
                case 6: subject = campo; break;
                case 7: group = campo; break;
                case 8: titulos[0] = campo; break;
                case 9: titulos[1] = campo; break;
                case 10: titulos[2] = campo; break;
                case 11: titulos[3] = campo; break;
                default: break;
            }
        }

        registroOut = Registro(chavePrimaria);
        registroOut.setStatus(FLAGS::ATIVO);
        registroOut.setReference(reference);
        registroOut.setDataValue(dataValue);
        registroOut.setStatusReference(status);
        registroOut.setUnits(units);
        registroOut.setMagnitude(magnitude);
        registroOut.setSubject(subject);
        registroOut.setGroup(group);
        registroOut.setTitle(titulos);

        return true;
    } 
    catch (const exception &e) {
        throw runtime_error(string("Erro ao processar linha do CSV: ") + e.what());
        return false;
    }
}

bool LeitorCSV::gerarBloco(BlocoRegistros &saida)
{
    if (!estaAberto()) {
        throw runtime_error("Tentativa de gerar bloco com arquivo fechado");
        return false;
    }

    BlocoRegistros bAux;
    Registro aux;
    bool blocoCompleto = false;
    while (!blocoCompleto && !chegouAoFim()) {
        if (!lerProximo(aux)) {
            //throw runtime_error("Erro ao ler registro do CSV");
           blocoCompleto = true;
        }
        else if (!bAux.push_back(aux)) {
            blocoCompleto = true;
        }
    }

    saida = bAux;
    return bAux.getContagemRegistros() > 0;
}

bool LeitorCSV::chegouAoFim() const
{
    if (!estaAberto()) return true;
    return arquivo.eof();
}