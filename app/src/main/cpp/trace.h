//
// Created by Sticks on 22-May-19.
//

#ifndef OSCOBOE_TRACE_H
#define OSCOBOE_TRACE_H

class Trace {

public:
    static void beginSection(const char *format, ...);
    static void endSection();
    static bool isEnabled() { return is_enabled_; }
    static void initialize();

private:
    static bool is_enabled_;
    static bool has_error_been_shown_;
};

#endif //OSCOBOE_TRACE_H
