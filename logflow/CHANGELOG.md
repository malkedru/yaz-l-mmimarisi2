# Değişiklik Günlüğü

## v2 — Tipli Kayıtlar ve İlk Gerçek Filtre

### Yeni dosyalar
- `LogRecord.h` — değişmez alan kaydı (`attributes` genişleme noktasıyla)
- `ParserStage.h` — CLF ayrıştırıcı aşaması; hatalı satırlar atlanır ve sayılır
- `tests/CollectingEmitter.h` — toplayıcı emitter test dublörü
- `tests/ParserStageTests.cpp` — ParserStage birim testleri (8 zorunlu durum + ek)
- `CHANGELOG.md`

### Değiştirilen dosyalar
- `Pipeline.h` — `Source -> Stage -> Sink` şablonu; `StringEmitterAdapter` yerine
  genel `SinkEmitterAdapter<T>` ve `StageEmitterAdapter<I,O>`
- `ConsoleSink.h` — `Sink<std::string>` yerine `Sink<LogRecord>`, tek satırlı biçim
- `main.cpp` — `ParserStage` eklendi; ayrıştırılan/hatalı sayıları yazdırılır
- `README.md`, `ARCHITECTURE.md` — güncellendi

### Değişmeyen dosyalar
`Emitter.h`, `Source.h`, `Sink.h`, `Stage.h`, `FileLineSource.h` — mimarinin
arayüzleri yeni bir aşama eklenirken hiç değişmedi.

### Bilinen sınırlar (sonraki haftalar)
- Hatalı satırların doğru ele alınması 5. haftada.
- `attributes` bu hafta boş; 5–7. haftalarda doldurulacak.
