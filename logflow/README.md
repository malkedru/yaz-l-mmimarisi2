# LogFlow - Pipeline Mimarisi (Artım 2: Tipli Kayıtlar ve İlk Gerçek Filtre)

## Proje Hakkında
LogFlow, bir erişim günlüğünü (Ortak Günlük Biçimi / CLF) satır satır okuyup
**yapılandırılmış `LogRecord` kayıtlarına** dönüştüren ve ekrana yazan bir
işlem hattıdır (pipeline). Artım 1'de hat yalnızca dizgi (string) taşıyordu;
Artım 2'de hat bir **alan modeli (domain model)** taşır.

## Dosya Yapısı

```
LogFlow/
├── Emitter.h            # Veriyi iletme arayüzü                (DEĞİŞMEDİ)
├── Source.h             # Veri üretimi arayüzü                 (DEĞİŞMEDİ)
├── Sink.h               # Veri tüketimi arayüzü                (DEĞİŞMEDİ)
├── Stage.h              # Veri işleme arayüzü                  (DEĞİŞMEDİ)
├── FileLineSource.h     # Dosya okuyan kaynak                  (DEĞİŞMEDİ)
├── LogRecord.h          # YENİ: değişmez alan kaydı
├── ParserStage.h        # YENİ: CLF ayrıştırıcı aşaması (string -> LogRecord)
├── ConsoleSink.h        # DEĞİŞTİ: LogRecord'u tek satırda yazar
├── Pipeline.h           # DEĞİŞTİ: Source -> Stage -> Sink (şablon)
├── main.cpp             # DEĞİŞTİ: ParserStage eklendi, sayaçlar yazdırılır
├── data/access-small.log
├── tests/
│   ├── CollectingEmitter.h     # YENİ: test dublörü (toplayıcı emitter)
│   └── ParserStageTests.cpp    # YENİ: ParserStage birim testleri
├── CHANGELOG.md
└── ARCHITECTURE.md
```

## Derleme ve Çalıştırma

**Dev-C++:** Tools → Compiler Options → "Add the following commands when calling the compiler"
kutusuna `-std=c++11` ekleyin. Ana program için `main.cpp`'yi, testler için
`tests/ParserStageTests.cpp`'yi ayrı projelerde derleyin (ikisinde de `main` var).

**Komut satırı:**
```
g++ -std=c++11 -Wall -Wextra -o logflow main.cpp
./logflow data/access-small.log

g++ -std=c++11 -Wall -Wextra -o run_tests tests/ParserStageTests.cpp
./run_tests
```

Örnek çıktı:
```
2024-01-01T10:15:30Z 192.168.1.1 GET /index.html -> 200 (1234 B)
...
Ayrıştırılan kayıt: 10
Hatalı (atlanan) satır: 0
```

## Pipeline Nasıl Çalışır?

```
FileLineSource -> [Emitter] -> ParserStage -> [Emitter] -> ConsoleSink
   (string)                    string->LogRecord             (LogRecord)
```

## Tasarım Notları

**`LogRecord` değişmezdir (immutable).** Tüm alanlar `const`, yalnızca yapıcıyla atanır.

**`attributes` neden var?** Kayıt biçimi genişletmeye açık olmalıdır: 5–7. haftalarda
eklenecek aşamalar bugünkü ayrıştırıcının hiçbir şey bilmediği verileri kayda
ekleyecektir. Her yeni bilgi için `LogRecord`'a alan eklemek yerine bu eşleme ortak
genişleme noktasıdır. Bu hafta boş bırakılır.

**Hatalı satırlar (bu hafta):** yalnızca **atlanır ve sayılır** (boş satır dahil).
Hataların doğru ele alınması (nedenleri, hata kuyruğu vb.) **5. haftada** gelecek;
mimari katman katman inşa edilmektedir.

**Zaman damgası:** `Instant` karşılığı olarak UTC epoch saniyesi (`std::time_t`);
saat dilimi kayması (`+0200` vb.) UTC'ye çevrilir. `path` sorgu dizesini içerir.

## Testler

`ParserStage` için dosya sistemine dokunmayan testler: aşamaya dizgi verilir,
çıktılar `CollectingEmitter` ile yakalanır. Zorunlu 8 durum: geçerli satır, eksik alan,
hatalı zaman damgası, hatalı durum kodu, boş satır, fazladan boşluk, boşluklu tırnaklı
user agent, sorgu dizesi. Ek 2 test grubu (bayt `-`, saat dilimi, artık yıl, protokolsüz
istek, eksik user agent, `ConsoleSink`). Toplam: **10 test, 37 doğrulama, hepsi geçiyor.**

### Satır Kapsaması (gcov, `-O0 --coverage`)

| Dosya | Kapsama |
|-------|---------|
| ParserStage.h | %98,28 (116 satır) |
| LogRecord.h | %100 (13 satır) |
| ConsoleSink.h | %92,86 (14 satır) |
| **Üretim kodu toplamı** | **%97,9 (140/143)** |

`Pipeline.h` ve `main.cpp` birim testlerin kapsamı dışındadır (uçtan uca
`logflow data/access-small.log` çalıştırmasıyla doğrulanır). Kapsanmayan satırlar:
ayrıştırıcıda ulaşılamayan savunma dalı ve `ConsoleSink::format` içindeki `gmtime`
başarısızlık dalı.

Kapsama ölçümü:
```
g++ -std=c++11 --coverage -O0 -o run_tests tests/ParserStageTests.cpp
./run_tests && gcov run_tests-ParserStageTests.gcda
```
