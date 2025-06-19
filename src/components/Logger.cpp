#include "Logger.hpp"
#include <ctime>



using namespace std;

Logger::Logger(const string& logBaseName) {
    string filename = generateLogFileName(logBaseName);
    logFile.open("../data/logs/"+filename);
    
    if (!logFile.is_open()) {
        /*cerr << "ERRO: Não foi possível criar o arquivo de log: " 
                 << filename << endl;*/
        throw runtime_error("Falha ao criar arquivo de log");
    }
    
    logFile << "[" << getCurrentTime() << "] [INFO] Log iniciado\n";
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

string Logger::getCurrentTime() const {
   time_t now = time(nullptr);
    struct tm* timeInfo = localtime(&now); 
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);
    return buffer;
}

string Logger::generateLogFileName(const string& baseName) const {
    return baseName + "_" + getCurrentTime() + ".txt"; // Ex: log_2024-06-12_14:30:00.txt
}

void Logger::writeLog(const string& level, const string& message) {
    if (logFile.is_open()) {
        logFile << "[" << getCurrentTime() << "] [" << level << "] " << message << "\n";
    }
}

void Logger::error(const string& message) {
    writeLog("ERRO", message);
}

void Logger::warning(const string& message) {
    writeLog("AVISO", message);
}

void Logger::info(const string& message) {
    writeLog("INFO", message);
}