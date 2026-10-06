#ifndef SINK_H
#define SINK_H

template<typename I>
class Sink {
public:
    virtual ~Sink() {}
    virtual void consume(const I& item) = 0;
};

#endif
