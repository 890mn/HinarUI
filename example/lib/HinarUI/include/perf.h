#ifndef PERF_H
#define PERF_H

class PerfMonitor {
public:
    void begin();
    void loop();
};

extern PerfMonitor perf;

#endif
