#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include "readwave.h"
#include "portaudio.h"
#include "audioCapture.h"
using namespace std;

//energy
const double forgetfactor = 1;
double background = 0;
double level = 0;
const double adjustment = 0.05;
const double threshold = 5;
//when detect the endpointing, wait a few seconds to decide if there is no audio 
int wait1 = 0;
int wait2 = 0;
//flag the first ten frames
int frameCount = 0;
//sum up energy of first ten frames
double totalBackground = 0;

double EnergyPerSampleInDecibel(const SAMPLE* ptr, long framesToCalc)
{
	double energy = 0;
	for (int i = 0; i < framesToCalc; i++)
	{
		energy += pow(*(ptr + i), 2);
	}
	energy = 10 * log10(energy);
	return energy;
}

bool classifyFrame(const SAMPLE* ptr, long framesToCalc)
{
	//background equals to average energy of first ten frames
	background = totalBackground / 10;
	bool isSpeech = false;
	double current = EnergyPerSampleInDecibel(ptr, framesToCalc);
	level = ((level * forgetfactor) + current) / (forgetfactor + 1);
	if (current < background)
		background = current;
	else
		background += (current - background) * adjustment;
	if (level < background)
		level = background;
	if (level - background > threshold)
		isSpeech = true;
	cout << "background: " << background << endl;
	cout << "level: " << endl;
	cout << "current: " << current << endl;
	return isSpeech;
}

/* This routine will be called by the PortAudio engine when audio is needed.
** It may be called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int recordCallback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	paTestData *data = (paTestData*)userData;
	const SAMPLE *rptr = (const SAMPLE*)inputBuffer;
	SAMPLE *wptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
	long framesToCalc;
	long i;
	int finished;
	unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;

	(void)outputBuffer; /* Prevent unused variable warnings. */
	(void)timeInfo;
	(void)statusFlags;
	(void)userData;

	if (framesLeft < framesPerBuffer)
	{
		framesToCalc = framesLeft;
		finished = paComplete;
	}
	else
	{
		framesToCalc = framesPerBuffer;
		finished = paContinue;
	}

	if (inputBuffer == NULL)
	{
		for (i = 0; i < framesToCalc; i++)
		{
			*wptr++ = SAMPLE_SILENCE;  /* left */
			if (NUM_CHANNELS == 2) *wptr++ = SAMPLE_SILENCE;  /* right */
		}
	}
	else
	{
		for (i = 0; i < framesToCalc; i++)
		{
			*wptr++ = *rptr++;  /* left */
			if (NUM_CHANNELS == 2) *wptr++ = *rptr++;  /* right */
		}
	}
	data->frameIndex += framesToCalc;

	if (frameCount < 10)
	{
		int tempEnergy = EnergyPerSampleInDecibel(rptr - framesToCalc, framesToCalc);
		frameCount++;
		if (frameCount == 1)
		{
			level = tempEnergy;
		}
		else
		{
			double current = tempEnergy;
			level = ((level * forgetfactor) + current) / (forgetfactor + 1);
		}
		totalBackground += tempEnergy;
	}
	else
	{
		if (classifyFrame(rptr - framesToCalc, framesToCalc))
			wait1 = 0;
		else
			wait1++;
	}
	//cout << "wait1: " << wait1 << endl;

	//endpointing
	/*
	if (classifyFrame(rptr-framesToCalc, framesToCalc))
	{
	if (wait1 > 0)
	{
	if (wait2 > 0)
	{
	wait1 = 0;
	wait2 = 0;
	}
	wait2++;
	}
	}
	else
	{
	if (wait2 > 0)
	wait2++;
	wait1++;
	}
	*/
	/*wait 1 seconds to decide whether there is audio
	**we should reduce the effect of noise, so I use a
	**wait2 to cout the length of noise, if in 500ms
	**there is another noise, I think it is audio
	*/
	/*
	if (wait1 > 60 && wait2 < 20)
	{
	finished = paComplete;
	data->maxFrameIndex = data->frameIndex;
	}
	*/
	if (wait1 > 33)
	{
		finished = paComplete;
		data->maxFrameIndex = data->frameIndex;
	}
	return finished;
}

/* This routine will be called by the PortAudio engine when audio is needed.
** It may be called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int playCallback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	paTestData *data = (paTestData*)userData;
	SAMPLE *rptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
	SAMPLE *wptr = (SAMPLE*)outputBuffer;
	unsigned int i;
	int finished;
	unsigned int framesLeft = data->maxFrameIndex - data->frameIndex;

	(void)inputBuffer; /* Prevent unused variable warnings. */
	(void)timeInfo;
	(void)statusFlags;
	(void)userData;

	if (framesLeft < framesPerBuffer)
	{
		/* final buffer... */
		for (i = 0; i < framesLeft; i++)
		{
			*wptr++ = *rptr++;  /* left */
			if (NUM_CHANNELS == 2) *wptr++ = *rptr++;  /* right */
		}
		for (; i < framesPerBuffer; i++)
		{
			*wptr++ = 0;  /* left */
			if (NUM_CHANNELS == 2) *wptr++ = 0;  /* right */
		}
		data->frameIndex += framesLeft;
		finished = paComplete;
	}
	else
	{
		for (i = 0; i < framesPerBuffer; i++)
		{
			*wptr++ = *rptr++;  /* left */
			if (NUM_CHANNELS == 2) *wptr++ = *rptr++;  /* right */
		}
		data->frameIndex += framesPerBuffer;
		finished = paContinue;
	}
	return finished;
}

void audioProcess()
{
	char a;
	cout << "Please enter \"s\" to start record!" << endl;
	while ((a = getch()) != 115)
	{
		cout << "Wrong key! Please enter \"s\" to start record!" << endl;
	};

	PaStreamParameters  inputParameters,
		outputParameters;
	PaStream*           stream;
	PaError             err = paNoError;
	paTestData          data;
	int                 i;
	int                 totalFrames;
	int                 numSamples;
	int                 numBytes;
	SAMPLE              max, val;
	double              average;

	printf("patest_record.c\n"); fflush(stdout);

	data.maxFrameIndex = totalFrames = NUM_SECONDS * SAMPLE_RATE; /* Record for a few seconds. */
	data.frameIndex = 0;
	numSamples = totalFrames * NUM_CHANNELS;
	numBytes = numSamples * sizeof(SAMPLE);
	data.recordedSamples = (SAMPLE *)malloc(numBytes); /* From now on, recordedSamples is initialised. */
	if (data.recordedSamples == NULL)
	{
		printf("Could not allocate record array.\n");
		goto done;
	}
	for (i = 0; i < numSamples; i++) data.recordedSamples[i] = 0;

	err = Pa_Initialize();
	if (err != paNoError) goto done;

	inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
	if (inputParameters.device == paNoDevice) {
		fprintf(stderr, "Error: No default input device.\n");
		goto done;
	}
	inputParameters.channelCount = 2;                    /* stereo input */
	inputParameters.sampleFormat = PA_SAMPLE_TYPE;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	/* Record some audio. -------------------------------------------- */
	err = Pa_OpenStream(
		&stream,
		&inputParameters,
		NULL,                  /* &outputParameters, */
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		paClipOff,      /* we won't output out of range samples so don't bother clipping them */
		recordCallback,
		&data);
	if (err != paNoError) goto done;

	err = Pa_StartStream(stream);
	if (err != paNoError) goto done;
	printf("\n=== Now recording!! Please speak into the microphone. ===\n"); fflush(stdout);

	while ((err = Pa_IsStreamActive(stream)) == 1)
	{
		Pa_Sleep(1000);
		printf("index = %d\n", data.frameIndex); fflush(stdout);
	}
	if (err < 0) goto done;

	err = Pa_CloseStream(stream);
	if (err != paNoError) goto done;

	/* Measure maximum peak amplitude. */
	//max = 0;
	//average = 0.0;
	/*calculate background to classify*/
	/*
	double totalBackground = 0;
	int numBackground = 0;
	for (i = 0; i < numSamples; i++)
	{
	val = data.recordedSamples[i];
	if (val < 0) val = -val;
	if (val > max)
	{
	max = val;
	}
	const SAMPLE &refVal = val;
	average += EnergyPerSampleInDecibel(&refVal);
	if (i < 10)
	{
	totalBackground += EnergyPerSampleInDecibel(&refVal);
	numBackground = i + 1;
	}
	if (i == 0)
	{
	level = EnergyPerSampleInDecibel(&refVal);
	}
	}

	background = totalBackground / numBackground;
	average = average / (double)numSamples;

	cout << "level: " << level << endl;
	cout << "background: " << background << endl;
	printf("sample max amplitude = \"PRINTF_S_FORMAT\"\n", max);
	printf("sample average = %lf\n", average);
	*/

	/* Write recorded data to a file. */
#if WRITE_TO_FILE
	{
		/*
		FILE  *fid;
		fid = fopen("recorded.raw", "wb");
		if (fid == NULL)
		{
		printf("Could not open file.");
		}
		else
		{
		fwrite(data.recordedSamples, NUM_CHANNELS * sizeof(SAMPLE), totalFrames, fid);
		fclose(fid);
		printf("Wrote data to 'recorded.raw'\n");
		}
		*/
		char *path = "recorded.wav";
		WriteWave(path, data.recordedSamples, totalFrames, SAMPLE_RATE);
	}
#endif

	/* Playback recorded data.  -------------------------------------------- */
	data.frameIndex = 0;

	outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
	if (outputParameters.device == paNoDevice) {
		fprintf(stderr, "Error: No default output device.\n");
		goto done;
	}
	outputParameters.channelCount = 2;                     /* stereo output */
	outputParameters.sampleFormat = PA_SAMPLE_TYPE;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	printf("\n=== Now playing back. ===\n"); fflush(stdout);
	err = Pa_OpenStream(
		&stream,
		NULL, /* no input */
		&outputParameters,
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		paClipOff,      /* we won't output out of range samples so don't bother clipping them */
		playCallback,
		&data);
	if (err != paNoError) goto done;

	if (stream)
	{
		err = Pa_StartStream(stream);
		if (err != paNoError) goto done;

		printf("Waiting for playback to finish.\n"); fflush(stdout);

		while ((err = Pa_IsStreamActive(stream)) == 1) Pa_Sleep(100);
		if (err < 0) goto done;

		err = Pa_CloseStream(stream);
		if (err != paNoError) goto done;

		printf("Done.\n"); fflush(stdout);
	}

done:
	Pa_Terminate();
	if (data.recordedSamples)       /* Sure it is NULL or valid. */
		free(data.recordedSamples);
	if (err != paNoError)
	{
		fprintf(stderr, "An error occured while using the portaudio stream\n");
		fprintf(stderr, "Error number: %d\n", err);
		fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
		err = 1;          /* Always return 0 or 1, but no other return codes. */
	}
}