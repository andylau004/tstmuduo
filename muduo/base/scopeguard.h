
#ifndef ONEPIECE_UTILS_SCOPEGUARD_H_
#define ONEPIECE_UTILS_SCOPEGUARD_H_





#include <memory>
#include <stdio.h>

#include "timerutil.h"

#include "muduo/base/Logging.h"



class ScopeGuardBase {
public:
    ScopeGuardBase() : active_(true) {}
    ~ScopeGuardBase() {}

public:
    void dismiss() { active_ = false; }

    ScopeGuardBase(ScopeGuardBase&& other) : active_(other.active_) {
    }
    ScopeGuardBase& operator=(ScopeGuardBase&& other) {
        if (this != &other) {
            active_ = other.active_;
            other.dismiss();
        }
        return *this;
    }

    ScopeGuardBase(const ScopeGuardBase &other) = delete;
    ScopeGuardBase& operator=(const ScopeGuardBase&) = delete;
protected:
    bool active_;
};

template < class Fun >
class TimeScopeGuard : public ScopeGuardBase {
public:
    TimeScopeGuard(Fun f, const std::string& name) : f_(std::move(f)), realFunName_(name), ScopeGuardBase() {
        start_ = TimerUtils::get_current_nanotime();
    }

    ~TimeScopeGuard() {
        if (active_) {
            try {
                f_();
            } catch(...) {
            }
        }
        check_dump();
    }
    TimeScopeGuard& operator =(TimeScopeGuard&& rhs) {
        if (this != &rhs) {
            ScopeGuardBase::operator =(std::move(rhs));
            f_ = std::move(rhs.f_);
            start_ = rhs.start_;
            realFunName_ = std::move(rhs.realFunName_);
        }
        return *this;
    }
    TimeScopeGuard(TimeScopeGuard&& rhs) :
        ScopeGuardBase(std::move(rhs)),
        f_(std::move(rhs.f_)), start_(rhs.start_), realFunName_(std::move(rhs.realFunName_)) {
    }

    TimeScopeGuard() = delete;
    TimeScopeGuard(const TimeScopeGuard &rhs) = delete;
    TimeScopeGuard& operator =(const TimeScopeGuard&) = delete;
private:
    void check_dump() {
        int32_t diff = (int32_t)((TimerUtils::get_current_nanotime() - start_) / 1e6);
        if (realFunName_.empty()) {
            LOG_INFO << "?cost=" << diff << "ms";
        }
        else {
            LOG_INFO << realFunName_ << "?cost=" << diff << "ms";
        }
    }
private:
    Fun f_;
    int64_t start_;
    std::string realFunName_;
};


template < class F >
TimeScopeGuard<F> timeScopeGuard(F f, const std::string& name) {
    LOG_INFO << "recv name=" << name;
    return TimeScopeGuard<F>(std::move(f), name);
}




#endif
