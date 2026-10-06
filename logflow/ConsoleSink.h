#ifndef CONSOLESINK_H
#define CONSOLESINK_H

#include "Sink.h"
#include "LogRecord.h"
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

// LogRecord'u okunabilir TEK SATIRDA gösterir:
// 2024-01-01T10:15:30Z 192.168.1.1 GET /index.html -> 200 (1234 B)
class ConsoleSink : public Sink<LogRecord> {
public:
    static std::string format(const LogRecord& r) {
        char buf[32];
        std::time_t t = r.timestamp();
        std::tm* tmv = std::gmtime(&t);
        if (tmv) std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", tmv);
        else buf[0] = '\0';

        std::ostringstream os;
        os << buf << " " << r.clientIp() << " " << r.method() << " " << r.path()
           << " -> " << r.status() << " (" << r.bytes() << " B)";
        if (!r.userAgent().empty()) os << " UA=\"" << r.userAgent() << "\"";
        return os.str();
    }

    void consume(const LogRecord& item) override {
        std::cout << format(item) << std::endl;
    }
};

#endif
