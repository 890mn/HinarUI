#ifndef HINARUI_FILTERED_VALUE_H
#define HINARUI_FILTERED_VALUE_H

class FilteredValue {
public:
    FilteredValue(float threshold, float hysteresis)
        : threshold_(threshold), hysteresis_(hysteresis) {}

    float update(float raw) {
        if (!hasValue_) {
            hasValue_ = true;
            value_ = raw;
            return value_;
        }
        float diff = raw - value_;
        if (diff < 0) diff = -diff;

        if (diff >= threshold_) {
            value_ = raw;
            return value_;
        }

        if (diff >= (threshold_ - hysteresis_)) {
            pending_ = raw;
            pendingActive_ = true;
        } else if (pendingActive_) {
            float pendingDiff = pending_ - raw;
            if (pendingDiff < 0) pendingDiff = -pendingDiff;
            if (pendingDiff >= hysteresis_) {
                pendingActive_ = false;
                value_ = raw;
            }
        }
        return value_;
    }

    float value() const { return value_; }

private:
    float threshold_;
    float hysteresis_;
    float value_ = 0.0f;
    float pending_ = 0.0f;
    bool hasValue_ = false;
    bool pendingActive_ = false;
};

#endif
