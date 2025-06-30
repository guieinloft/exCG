#include <libopenmpt/libopenmpt.h>
#include <libopenmpt/libopenmpt_stream_callbacks_file.h>
#include "portaudio.h"
#include "modplayer.h"

void mod_init(struct modplayer *mp, char *path)
{
	mp->buffers[0] = mp->left;
	mp->buffers[1] = mp->right;
	FILE *file = fopen(path, "rb");
	if (file == NULL)
		return;
	mp->mod = openmpt_module_create2(
			openmpt_stream_get_file_callbacks2(), file, NULL, NULL,
			NULL, NULL, NULL, NULL, NULL);
	fclose(file);
	if (mp->mod == NULL)
		return;

	if (Pa_Initialize() != paNoError)
		return;
	if (Pa_OpenDefaultStream(&mp->stream, 0, 2,
			paFloat32 | paNonInterleaved, SAMPLERATE,
			paFramesPerBufferUnspecified, NULL, NULL) != paNoError
			|| mp->stream == NULL)
		return;
	if (Pa_StartStream(mp->stream) != paNoError)
		return;
	mp->count = 1;
}

void mod_play(struct modplayer *mp)
{
	if (mp->count == 0)
		return;
	mp->count = openmpt_module_read_float_stereo(mp->mod, SAMPLERATE,
			BUFFERSIZE, mp->left, mp->right);
	if (mp->count == 0)
		return;
	Pa_WriteStream(mp->stream, mp->buffers, (unsigned long)mp->count);
}
