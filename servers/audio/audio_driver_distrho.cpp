#include "audio_driver_distrho.h"

#include "core/os/os.h"

AudioDriverDistrho *AudioDriverDistrho::singleton = nullptr;

Error AudioDriverDistrho::init() {
    active.clear();
    samples_in = nullptr;

    if (mix_rate == -1) {
        mix_rate = _get_configured_mix_rate();
    }

    channels = get_channels();
    samples_in = memnew_arr(int32_t, size_t(buffer_frames) * channels);

    return OK;
}

void AudioDriverDistrho::start() {
    active.set();
}

int AudioDriverDistrho::get_mix_rate() const {
    return mix_rate;
}

AudioDriver::SpeakerMode AudioDriverDistrho::get_speaker_mode() const {
    return speaker_mode;
}

void AudioDriverDistrho::lock() {
    mutex.lock();
}

void AudioDriverDistrho::unlock() {
    mutex.unlock();
}

void AudioDriverDistrho::set_speaker_mode(SpeakerMode p_mode) {
    speaker_mode = p_mode;
}

void AudioDriverDistrho::set_mix_rate(int p_rate) {
    mix_rate = p_rate;
}

uint32_t AudioDriverDistrho::get_channels() const {
    static const int channels_for_mode[4] = { 2, 4, 8, 16 };
    return channels_for_mode[speaker_mode];
}

void AudioDriverDistrho::process_external(int p_frames) {
    ERR_FAIL_COND(!active.is_set());
    lock();
    start_counting_ticks();
    audio_server_process(p_frames, samples_in);
    stop_counting_ticks();
    unlock();
}

void AudioDriverDistrho::finish() {
    if (samples_in) {
        memdelete_arr(samples_in);
    }
}

AudioDriverDistrho::AudioDriverDistrho() {
    singleton = this;
}

AudioDriverDistrho::~AudioDriverDistrho() {
    ERR_FAIL_COND(singleton != this);
    singleton = nullptr;
}
