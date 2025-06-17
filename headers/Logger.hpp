#pragma once

#ifndef LOGGER_HPP
#define LOGGER_HPP


#include <string>
#include <fstream>



using namespace std;

class Logger {
public:
    // Métodos de log (níveis de erro)
    void error(const string& message);
    void warning(const string& message);
    void info(const string& message);

    // Construtor recebe um nome base para o arquivo (opcional)
    Logger(const string& logBaseName = "log");
    ~Logger();

private:
    ofstream logFile;
    string getCurrentTime() const;
    string generateLogFileName(const string& baseName) const;
    void writeLog(const string& level, const string& message);
};

#endif