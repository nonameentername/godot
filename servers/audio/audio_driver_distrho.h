#ifndef AUDIO_DRIVER_DISTRHO_H
#define AUDIO_DRIVER_DISTRHO_H

#include "servers/audio_server.h"

#include "core/os/mutex.h"
#include "core/templates/safe_refcount.h"

class AudioDriverDistrho : public AudioDriver {
	Mutex mutex;

	int32_t *samples_in = nullptr;

	uint32_t buffer_frames = 4096;
	int32_t mix_rate = -1;
	SpeakerMode speaker_mode = SPEAKER_MODE_STEREO;

	int channels;

	SafeFlag active;

	static AudioDriverDistrho *singleton;

public:
	virtual const char *get_name() const override {
		return "Distrho";
	}

	virtual Error init() override;
	virtual void start() override;
	virtual int get_mix_rate() const override;
	virtual SpeakerMode get_speaker_mode() const override;

	virtual void lock() override;
	virtual void unlock() override;
	virtual void finish() override;

	void set_speaker_mode(SpeakerMode p_mode);
	void set_mix_rate(int p_rate);

	uint32_t get_channels() const;

	void process_external(int p_frames);

	static AudioDriverDistrho *get_singleton() { return singleton; }

	AudioDriverDistrho();
	~AudioDriverDistrho();
};

#endif // AUDIO_DRIVER_DISTRHO_H
