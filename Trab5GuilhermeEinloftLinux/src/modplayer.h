#define BUFFERSIZE 960
#define SAMPLERATE 44100

struct modplayer {
	float left[BUFFERSIZE];
	float right[BUFFERSIZE];
	float *buffers[2];
	openmpt_module *mod;
	PaStream *stream;
	size_t count;
};

void mod_init(struct modplayer *mp, char *path);
void mod_play(struct modplayer *mp);
