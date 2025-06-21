#include "LeitorBin.hpp"
#include <cstring>

using namespace std;

LeitorBin::LeitorBin(const string caminho)
{
    arquivoEntrada.open(caminho, ios::binary);
    if (!arquivoEntrada.is_open()) {
        throw runtime_error("Erro ao abrir arquivo de entrada para leitura.");
        return;
    }
    // Lê o cabeçalho global do arquivo
    arquivoEntrada.read(reinterpret_cast<char*>(&cabecalhoArquivo), sizeof(cabecalhoParaArquivo));
    if (!arquivoEntrada) {
        throw runtime_error("Erro ao ler o cabeçalho global do arquivo.");
        arquivoEntrada.close();
    }
}

LeitorBin::~LeitorBin() {
    if (arquivoEntrada.is_open()) {
        arquivoEntrada.close();
    }
}

cabecalhoParaArquivo LeitorBin::getCabecalho() const {
    return cabecalhoArquivo;
}

bool LeitorBin::lerProximoBloco(BlocoRegistros& blocoSaida) {
    if (!arquivoEntrada.is_open() || arquivoEntrada.eof()) return false;

    // Lê o cabeçalho do próximo bloco
    cabecalhoParaBloco cabecalhoBloco;
    arquivoEntrada.read(reinterpret_cast<char*>(&cabecalhoBloco), sizeof(cabecalhoParaBloco));
    if (!arquivoEntrada) return false;

    // Sempre lê o bloco inteiro, independente da quantidade de registros válidos
    size_t tamanhoRegistro = Registro::sizeofRegistro();
    size_t tamanhoBuffer = sizeof(cabecalhoParaBloco) + REGRAS::TAMANHO_BUFFER * tamanhoRegistro;
    char* buffer = new char[tamanhoBuffer];

    // Copia o cabeçalho para o início do buffer
    memcpy(buffer, &cabecalhoBloco, sizeof(cabecalhoParaBloco));

    // Lê todos os registros do bloco
    arquivoEntrada.read(buffer + sizeof(cabecalhoParaBloco), tamanhoRegistro * REGRAS::TAMANHO_BUFFER);
    if (!arquivoEntrada) {
        delete[] buffer;
        throw runtime_error("Erro ao ler registros do bloco.");
        return false;
    }

    // Constrói o bloco a partir do buffer (agora com cabeçalho + registros)
    blocoSaida = BlocoRegistros(buffer, tamanhoBuffer);
    blocoSaida.setIdBloco(cabecalhoBloco.id_bloco);
    delete[] buffer;
    return true;
}

bool LeitorBin::posicionarParaBloco(int indice) {
    if (!arquivoEntrada.is_open()) return false;
    if (indice < 0 || static_cast<uint64_t>(indice) >= cabecalhoArquivo.qtd_total_blocos_no_arquivo) return false;

    size_t tamanhoCabecalhoArquivo = sizeof(cabecalhoParaArquivo);
    size_t offset = tamanhoCabecalhoArquivo;
    size_t tamanhoRegistro = Registro::sizeofRegistro();
    for (int i = 0; i < indice; ++i) {
        cabecalhoParaBloco cabTemp;
        arquivoEntrada.seekg(offset, ios::beg);
        arquivoEntrada.read(reinterpret_cast<char*>(&cabTemp), sizeof(cabecalhoParaBloco));
        if (!arquivoEntrada) return false;
        offset += sizeof(cabecalhoParaBloco) + (tamanhoRegistro * cabTemp.qtd_registros_validos);
    }
    arquivoEntrada.seekg(offset, ios::beg);
    return arquivoEntrada.good();
}

bool LeitorBin::lerBlocoAtual(BlocoRegistros& bloco) {
    if (this->chegouAoFim()) return false;
    // Salva a posição atual
    size_t pos = arquivoEntrada.tellg();
    // Lê o cabeçalho do bloco
    cabecalhoParaBloco cabecalhoBloco;
    arquivoEntrada.read(reinterpret_cast<char*>(&cabecalhoBloco), sizeof(cabecalhoParaBloco));
    if (!arquivoEntrada) {
        arquivoEntrada.clear();
        arquivoEntrada.seekg(pos);
        return false;
    }
    // Sempre lê o bloco inteiro
    size_t tamanhoRegistro = Registro::sizeofRegistro();
    size_t tamanhoBuffer = sizeof(cabecalhoParaBloco) + REGRAS::TAMANHO_BUFFER * tamanhoRegistro;
    char* buffer = new char[tamanhoBuffer];
    memcpy(buffer, &cabecalhoBloco, sizeof(cabecalhoParaBloco));
    arquivoEntrada.read(buffer + sizeof(cabecalhoParaBloco), tamanhoRegistro * REGRAS::TAMANHO_BUFFER);
    if (!arquivoEntrada) {
        delete[] buffer;
        arquivoEntrada.clear();
        arquivoEntrada.seekg(pos);
        return false;
    }
    bloco = BlocoRegistros(buffer, tamanhoBuffer);
    bloco.setIdBloco(cabecalhoBloco.id_bloco);
    delete[] buffer;
    // Retorna o ponteiro para a posição original
    arquivoEntrada.clear();
    arquivoEntrada.seekg(pos);
    return true;
}

bool LeitorBin::chegouAoFim() const {
    if (!arquivoEntrada.is_open()) return true;
    return arquivoEntrada.eof();
}
