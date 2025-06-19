#include "Logger.hpp"
#include <ctime>
#include <fstream>

using namespace std;

Logger::Logger()
{
    this->infoName = generateLogFileName("info");
    this->warningName = generateLogFileName("warning");
    this->errorName = generateLogFileName("error");
    
    // Abre os arquivos em modo append e garante que são criados
    infoFile.open("./data/logs/" + infoName, ios::out | ios::app);
    warningFile.open("./data/logs/" + warningName, ios::out | ios::app);
    errorFile.open("./data/logs/" + errorName, ios::out | ios::app);
    
    if (!infoFile.is_open() || !warningFile.is_open() || !errorFile.is_open())
    {
        throw runtime_error("Falha ao criar arquivos de log");
    }
    
    // Escreve e verifica o estado
    infoFile << "[" << getCurrentTime() << "] [INFO] Log iniciado\n";
    if (infoFile.fail()) {
        throw runtime_error("Falha ao escrever no arquivo de info");
    }
    infoFile.flush();
    
    warningFile << "[" << getCurrentTime() << "] [WARNING] Log iniciado\n";
    if (warningFile.fail()) {
        throw runtime_error("Falha ao escrever no arquivo de warning");
    }
    warningFile.flush();
    
    errorFile << "[" << getCurrentTime() << "] [ERROR] Log iniciado\n";
    if (errorFile.fail()) {
        throw runtime_error("Falha ao escrever no arquivo de error");
    }
    errorFile.flush();
}

void Logger::writeLog(ofstream &file, const string &level, const string &message)
{
    if (!file.is_open())
    {
        // Tenta reabrir se estiver fechado
        string filename;
        if (&file == &infoFile) filename = infoName;
        else if (&file == &warningFile) filename = warningName;
        else if (&file == &errorFile) filename = errorName;
        
        file.open("./data/logs/" + filename, ios::out | ios::app);
        if (!file.is_open()) {
            return;
        }
    }
    
    file << "[" << getCurrentTime() << "] [" << level << "] " << message << "\n";
    if (file.fail()) {
        // Tentar recuperar o erro
        file.clear();
        return;
    }
    file.flush();
    if (file.fail()) {
        file.clear();
    }
}

void Logger::error(const string &message)
{
    writeLog(errorFile, "ERRO", message);
    if (errorFile.fail()) {
        errorFile.clear();
        // Tentar reabrir o arquivo se falhar
        errorFile.open("./data/logs/" + errorName, ios::out | ios::app);
    }
}

void Logger::warning(const string &message)
{
    writeLog(warningFile, "AVISO", message);
    if (warningFile.fail()) {
        warningFile.clear();
        warningFile.open("./data/logs/" + warningName, ios::out | ios::app);
    }
}

void Logger::info(const string &message)
{
    writeLog(infoFile, "INFO", message);
    if (infoFile.fail()) {
        infoFile.clear();
        infoFile.open("./data/logs/" + infoName, ios::out | ios::app);
    }
}

string Logger::getCurrentTime() const
{
    time_t now = time(nullptr);
    struct tm *timeInfo = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%d-%m-%Y_%H:%M:%S", timeInfo);
    return buffer;
}

string Logger::generateLogFileName(const string &level) const
{
    return level + "_" + getCurrentTime() + ".txt";
}

Logger::~Logger()
{
    if (infoFile.is_open()) {
        infoFile.close();
    }
    if (warningFile.is_open()) {
        warningFile.close();
    }
    if (errorFile.is_open()) {
        errorFile.close();
    }
}