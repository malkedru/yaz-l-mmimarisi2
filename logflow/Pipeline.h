#ifndef PIPELINE_H
#define PIPELINE_H

#include "Source.h"
#include "Stage.h"
#include "Sink.h"
#include "Emitter.h"
#include <string>

// Emitter: Sink'e ileten adaptör (artık herhangi bir T için).
template<typename T>
class SinkEmitterAdapter : public Emitter<T> {
private:
    Sink<T>* sink;
public:
    SinkEmitterAdapter(Sink<T>* s) : sink(s) {}
    void emit(const T& item) override { sink->consume(item); }
};

// Emitter: gelen öğeyi bir Stage'e verir, Stage'in çıktısını bir sonrakine iletir.
template<typename I, typename O>
class StageEmitterAdapter : public Emitter<I> {
private:
    Stage<I, O>* stage;
    Emitter<O>& next;
public:
    StageEmitterAdapter(Stage<I, O>* st, Emitter<O>& n) : stage(st), next(n) {}
    void emit(const I& item) override { stage->process(item, next); }
};

// Source<I> -> Stage<I,O> -> Sink<O>
template<typename I, typename O>
class Pipeline {
private:
    Source<I>* source;
    Stage<I, O>* stage;
    Sink<O>* sink;

public:
    Pipeline(Source<I>* s, Stage<I, O>* st, Sink<O>* si)
        : source(s), stage(st), sink(si) {}

    void run() {
        if (!source || !stage || !sink) {
            throw StageException("Pipeline: Source, Stage ve Sink tanımlanmamış!");
        }

        SinkEmitterAdapter<O> toSink(sink);
        StageEmitterAdapter<I, O> toStage(stage, toSink);

        stage->open();
        source->produce(toStage);
        stage->close();
    }
};

#endif
