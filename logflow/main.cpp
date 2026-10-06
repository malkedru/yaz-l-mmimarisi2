#include <iostream>
#include "Pipeline.h"
#include "FileLineSource.h"
#include "ParserStage.h"
#include "ConsoleSink.h"

int main(int argc, char* argv[]) {
    try {
        // Komut satırını kontrol et
        if (argc < 2) {
            std::cerr << "Kullanım: logflow <dosya_yolu>" << std::endl;
            std::cerr << "Örnek: logflow data/access-small.log" << std::endl;
            return 1;
        }

        std::string filePath = argv[1];

        // Bileşenleri oluştur
        FileLineSource source(filePath);
        ParserStage parser;
        ConsoleSink sink;

        // Pipeline'ı kur ve çalıştır: Source -> ParserStage -> Sink
        Pipeline<std::string, LogRecord> pipeline(&source, &parser, &sink);
        pipeline.run();

        std::cout << std::endl
                  << "Ayrıştırılan kayıt: " << parser.parsedCount() << std::endl
                  << "Hatalı (atlanan) satır: " << parser.badCount() << std::endl;
        std::cout << "✓ Pipeline başarıyla tamamlandı!" << std::endl;
        return 0;

    } catch (const StageException& e) {
        std::cerr << "Hata: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Beklenmeyen hata: " << e.what() << std::endl;
        return 1;
    }
}
