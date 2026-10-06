# LogFlow - Sistem Mimarisi (Artım 2)

## Mimari Diyagram

```
FileLineSource ──(string)──> ParserStage ──(LogRecord)──> ConsoleSink
   (Source)                    (Stage)                      (Sink)
        └────────── Emitter (adaptörler) ──────────┘
```

## Bileşenler

| Bileşen | Arayüz | Örnek | Durum |
|---------|--------|-------|-------|
| Source | `produce(Emitter<T>&)` | `FileLineSource` | değişmedi |
| Emitter | `emit(const T&)` | `SinkEmitterAdapter`, `StageEmitterAdapter` | arayüz değişmedi |
| Stage | `process(const I&, Emitter<O>&)` | `ParserStage` (string -> LogRecord) | arayüz değişmedi |
| Sink | `consume(const T&)` | `ConsoleSink` (artık `LogRecord`) | arayüz değişmedi |
| Pipeline | `run()` | `Pipeline<I,O>`: Source -> Stage -> Sink | genelleştirildi |

## Tasarım Kararları

- **Alan modeli:** hat artık dizgi değil, değişmez `LogRecord` taşır.
- **`attributes` haritası:** sonraki aşamaların ayrıştırıcının bilmediği veriyi
  kayda ekleyebilmesi için genişleme noktası (5–7. haftalar).
- **Emitter:** 1->0 (filtre), 1->1 (dönüşüm), 1->N için tek yapı. `ParserStage`
  hatalı satırda hiçbir şey yaymaz (1->0), geçerlide bir kayıt yayar (1->1).
- **Hatalı satırlar:** bu hafta yalnızca atlanır ve sayılır; doğru ele alınması 5. haftada.
- **Test edilebilirlik:** `CollectingEmitter` ile aşama dosya okumadan test edilir.

## Aşama Eklemek Neleri Değiştirdi?

Yeni: `LogRecord.h`, `ParserStage.h`. Değişen: `Pipeline.h` (araya aşama girdi),
`ConsoleSink.h` (artık `LogRecord` tüketir), `main.cpp` (kurulum).
Değişmeyen: `Source.h`, `Sink.h`, `Emitter.h`, `Stage.h`, `FileLineSource.h`.

## Gelecek Aşamalar

```
Source → Parser → Stage2 → Stage3 → Sink
```
Filtre, dönüştürme ve hata yönetimi aşamaları aynı arayüzlerle eklenecek.
