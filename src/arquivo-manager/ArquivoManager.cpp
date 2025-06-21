#include "ArquivoManager.hpp"
#include <limits>
#include <iostream>

ArquivoManager::ArquivoManager(LeitorBin *leitor, GravadorDeBlocos *gravador)
    : leitor(leitor), gravador(gravador)
{
    if (!leitor || !gravador)
    {
        throw runtime_error("Erro: Dependencias nao podem ser nulas!\n");
    }
}

void ArquivoManager::inserirElementoPosicaoEspecifica()
{
    unsigned long posicao;
    cout << "Digite a posicao para insercao: ";
    cin >> posicao;

    float chave;
    string referencia;
    cout << "Digite a chave primaria do novo registro: ";
    cin >> chave;
    cout << "Digite a series_reference: ";
    cin >> referencia;

    Registro novoRegistro(chave);
    novoRegistro.setReference(referencia);

    unsigned long blocoAlvo = posicao / REGRAS::TAMANHO_BUFFER;
    unsigned long posicaoNoBloco = posicao % REGRAS::TAMANHO_BUFFER;

    if (!leitor->posicionarParaBloco(blocoAlvo))
    {
        cout << "Erro: Posicao invalida!\n";
        return;
    }

    BlocoRegistros bloco;
    if (!leitor->lerBlocoAtual(bloco))
    {
        cout << "Erro: Falha ao ler bloco!\n";
        return;
    }

    if (!bloco.push_position(novoRegistro, posicaoNoBloco))
    {
        cout << "Erro: Nao foi possivel inserir na posicao especificada!\n";
        return;
    }

    if (!gravador->escreverBloco(bloco))
    {
        cout << "Erro: Falha ao gravar bloco modificado!\n";
        return;
    }

    cout << "Registro inserido com sucesso na posicao " << posicao << "!\n";
}

void ArquivoManager::visualizarRegistrosEntrePosicoes()
{
    unsigned long inicio, fim;
    cout << "Digite a posicao inicial: ";
    cin >> inicio;
    cout << "Digite a posicao final: ";
    cin >> fim;

    if (inicio > fim)
    {
        cout << "Erro: Posicao inicial maior que final!\n";
        return;
    }

    leitor->posicionarParaBloco(0);
    BlocoRegistros bloco;
    unsigned long contadorGlobal = 0;
    bool blocoValido = true;

    while (blocoValido && contadorGlobal <= fim)
    {
        blocoValido = leitor->lerProximoBloco(bloco);

        for (unsigned long i = 0; i < REGRAS::TAMANHO_BUFFER && contadorGlobal <= fim; i++)
        {
            if (contadorGlobal >= inicio)
            {
                Registro reg;
                if (bloco.getRegistroPorIndice(i, reg))
                {
                    cout << "[" << contadorGlobal << "] " << reg.gerarStringImpressao() << "\n";
                }
            }
            contadorGlobal++;
        }
    }
}

void ArquivoManager::alterarDadosRegistro()
{
    unsigned long posicao;
    cout << "Digite a posicao do registro a ser alterado: ";
    cin >> posicao;

    unsigned long blocoIdx = posicao / REGRAS::TAMANHO_BUFFER;
    unsigned long posNoBloco = posicao % REGRAS::TAMANHO_BUFFER;

    if (!leitor->posicionarParaBloco(blocoIdx))
    {
        cout << "Erro: Posicao invalida!\n";
        return;
    }

    BlocoRegistros bloco;
    if (!leitor->lerBlocoAtual(bloco))
    {
        cout << "Erro: Falha ao ler bloco!\n";
        return;
    }

    Registro reg;
    if (!bloco.getRegistroPorIndice(posNoBloco, reg))
    {
        cout << "Erro: Registro na posicao " << posicao << " nao encontrado!\n";
        return;
    }

    cout << "Registro atual:\n"
         << reg.gerarStringImpressao() << "\n\n";

    string novoRef;
    float novoValor;
    cout << "Novo series_reference: ";
    cin >> novoRef;
    cout << "Novo data_value: ";
    cin >> novoValor;

    reg.setReference(novoRef);
    reg.setDataValue(novoValor);

    if (!bloco.atualizarRegistro(reg, posNoBloco))
    {
        cout << "Erro: Falha ao atualizar registro!\n";
        return;
    }

    if (!gravador->escreverBloco(bloco))
    {
        cout << "Erro: Falha ao gravar bloco modificado!\n";
        return;
    }

    cout << "Registro atualizado com sucesso!\n";
}

void ArquivoManager::trocarRegistrosPosicao()
{
    unsigned long pos1, pos2;
    cout << "Digite a primeira posicao: ";
    cin >> pos1;
    cout << "Digite a segunda posicao: ";
    cin >> pos2;

    if (pos1 == pos2)
    {
        cout << "As posicoes sao iguais. Nenhuma alteracao necessaria.\n";
        return;
    }

    if (pos1 > pos2)
    {
        unsigned long temp = pos1;
        pos1 = pos2;
        pos2 = temp;
    }

    unsigned long bloco1 = pos1 / REGRAS::TAMANHO_BUFFER;
    unsigned long bloco2 = pos2 / REGRAS::TAMANHO_BUFFER;
    unsigned long posNoBloco1 = pos1 % REGRAS::TAMANHO_BUFFER;
    unsigned long posNoBloco2 = pos2 % REGRAS::TAMANHO_BUFFER;

    if (bloco1 == bloco2)
    {
        BlocoRegistros bloco;
        if (!leitor->posicionarParaBloco(bloco1) || !leitor->lerBlocoAtual(bloco))
        {
            cout << "Erro: Falha ao acessar bloco!\n";
            return;
        }

        if (!bloco.trocarRegistros(posNoBloco1, posNoBloco2))
        {
            cout << "Erro: Falha ao trocar registros!\n";
            return;
        }

        if (!gravador->escreverBloco(bloco))
        {
            cout << "Erro: Falha ao gravar bloco modificado!\n";
            return;
        }
    }
    else
    {
        BlocoRegistros blocoA, blocoB;
        Registro regA, regB;

        if (!leitor->posicionarParaBloco(bloco1) || !leitor->lerBlocoAtual(blocoA))
        {
            cout << "Erro: Falha ao ler primeiro bloco!\n";
            return;
        }

        if (!blocoA.getRegistroPorIndice(posNoBloco1, regA))
        {
            cout << "Erro: Falha ao obter primeiro registro!\n";
            return;
        }

        if (!leitor->posicionarParaBloco(bloco2) || !leitor->lerBlocoAtual(blocoB))
        {
            cout << "Erro: Falha ao ler segundo bloco!\n";
            return;
        }

        if (!blocoB.getRegistroPorIndice(posNoBloco2, regB))
        {
            cout << "Erro: Falha ao obter segundo registro!\n";
            return;
        }

        if (!blocoA.atualizarRegistro(regB, posNoBloco1) ||
            !blocoB.atualizarRegistro(regA, posNoBloco2))
        {
            cout << "Erro: Falha ao atualizar registros!\n";
            return;
        }

        if (!gravador->escreverBloco(blocoA))
        {
            cout << "Erro: Falha ao gravar primeiro bloco!\n";
            return;
        }

        if (!gravador->escreverBloco(blocoB))
        {
            cout << "Erro: Falha ao gravar segundo bloco!\n";
            gravador->escreverBloco(blocoA);
            cout << "Operacao cancelada (rollback realizado)!\n";
            return;
        }
    }

    cout << "Registros trocados com sucesso!\n";
}

void ArquivoManager::imprimirTodosRegistros()
{
    leitor->posicionarParaBloco(0);
    BlocoRegistros bloco;
    unsigned long contador = 0;

    while (leitor->lerProximoBloco(bloco))
    {
        for (unsigned long i = 0; i < REGRAS::TAMANHO_BUFFER; i++)
        {
            Registro reg;
            if (bloco.getRegistroPorIndice(i, reg))
            {
                cout << "[" << contador << "] " << reg.gerarStringImpressao() << "\n";
                contador++;
            }
        }
    }

    cout << "\nTotal de registros: " << contador << "\n";
}
