#include "gravar_blocos_bin.hpp"
using namespace std;

GravadorDeBlocos::GravadorDeBlocos(const char* caminhoSaida, Logger* pLog)
    : log(pLog)
{
    arquivoSaida.open(caminhoSaida, ios::binary | ios::out | ios::trunc);
    if (!arquivoSaida.is_open()) {
        log->error("Erro ao abrir o arquivo de saída para escrita.");
        return;
    }

    // Inicializa cabeçalho com valores padrão
    cabecalhoArquivo.qtd_total_blocos_no_arquivo = 0;
    cabecalhoArquivo.qtd_total_registros_no_arquivo = 0;
    cabecalhoArquivo.proximo_id_bloco = 0;

    // Reserva espaço para o cabeçalho (será sobrescrito ao final)
    arquivoSaida.write(reinterpret_cast<const char*>(&cabecalhoArquivo), sizeof(cabecalhoParaArquivo));
    if (!arquivoSaida) {
        log->error("Erro ao escrever o cabeçalho inicial do arquivo.");
        arquivoSaida.close();
    }
}

GravadorDeBlocos::~GravadorDeBlocos() {
    finalizar();
}

bool GravadorDeBlocos::escreverBloco(BlocoRegistros& bloco) {
    if (!arquivoSaida.is_open()) {
        log->error("Tentativa de escrever em arquivo inválido.");
        return false;
    }

    // Atribui ID único ao bloco e atualiza metadado
    BlocoRegistros& blocoNaoConst = bloco;
    blocoNaoConst.setIdBloco(cabecalhoArquivo.proximo_id_bloco);

    cabecalhoParaBloco cabBloco = bloco.getCabecalho();

    // Escreve o cabeçalho do bloco
    arquivoSaida.write(reinterpret_cast<const char*>(&cabBloco), sizeof(cabecalhoParaBloco));
    if (!arquivoSaida) {
        log->error("Erro ao escrever o cabeçalho do bloco.");
        return false;
    }

    // Escreve cada registro individualmente
    Registro temp;
    const size_t tamanhoRegistro = Registro::sizeofRegistro();
    char* buffer = new char[tamanhoRegistro];
    for (uint32_t i = 0; i < cabBloco.qtd_registros_validos; ++i) {
        if (!bloco.getRegistroPorIndice(i, temp)) {
            log->error("Erro ao recuperar registro do bloco para gravação.");
            delete[] buffer;
            return false;
        }
        temp.serializar(buffer);
        arquivoSaida.write(buffer, tamanhoRegistro);
        if (!arquivoSaida) {
            log->error("Erro ao serializar registro do bloco.");
            delete[] buffer;
            return false;
        }
    }
    delete[] buffer;

    // Atualiza cabeçalho global
    cabecalhoArquivo.qtd_total_blocos_no_arquivo++;
    cabecalhoArquivo.qtd_total_registros_no_arquivo += cabBloco.qtd_registros_validos;
    cabecalhoArquivo.proximo_id_bloco++;

    return true;
}

void GravadorDeBlocos::finalizar() {
    if (!arquivoSaida.is_open()) return;

    // Reposiciona no início para regravar o cabeçalho
    arquivoSaida.seekp(0, ios::beg);
    arquivoSaida.write(reinterpret_cast<const char*>(&cabecalhoArquivo), sizeof(cabecalhoParaArquivo));
    if (!arquivoSaida) {
        log->error("Erro ao reescrever o cabeçalho final do arquivo.");
    }
    arquivoSaida.close();
}
