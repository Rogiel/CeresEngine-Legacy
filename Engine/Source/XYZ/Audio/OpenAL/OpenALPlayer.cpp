////
//// Created by Rogiel Sulzbach on 7/16/17.
////
//
//#include "OpenALPlayer.hpp"
//
//#include <OpenAL/OpenAL.h>
//
//#include <cmath>
//#include <iostream>
//
//namespace XYZ::Audio::OpenAL {
//
//	void OpenALPlayer::play(const std::shared_ptr<AudioClip>& clip) {
//
//	}
//
//	void OpenALPlayer::al() {
//		std::cout << "initing" << std::endl;
//
//		std::cout << (size_t)&alcOpenDevice << std::endl;
//
//		// Initialization
//		ALCdevice* device = alcOpenDevice(nullptr); // select the "preferred device"
//		if(device ==nullptr) {
//			return;
//		}
//
//		if(device) {
//			ALCcontext* context = alcCreateContext(device, nullptr);
//			alcMakeContextCurrent(context);
//		}
//
//		ALenum error;
//
//		// Generate Buffers
//		alGetError(); // clear error code
//
//		ALuint buffer;
//		alGenBuffers(1, &buffer);
//		if((error = alGetError()) != AL_NO_ERROR) {
//			return;
//		}
//
//
//		/* Fill buffer with Sine-Wave */
//		float freq = 440.f;
//		int seconds = 4;
//		unsigned sample_rate = 22050;
//		size_t buf_size = seconds * sample_rate;
//
//		short *samples;
//		samples = new short[buf_size];
//		for(int i=0; i<buf_size; ++i) {
//			samples[i] = 32760 * sin( (2.f*float(M_PI)*freq)/sample_rate * i );
//		}
//
//		// Copy test.wav data into AL Buffer 0
//		alBufferData(buffer, AL_FORMAT_MONO16, samples, buf_size, sample_rate);
//		if((error = alGetError()) != AL_NO_ERROR) {
//			alDeleteBuffers(1, &buffer);
//			return;
//		}
//
//		// Generate Sources
//		ALuint source;
//		alGenSources(1, &source);
//		if((error = alGetError()) != AL_NO_ERROR) {
//			return;
//		}
//
//		ALfloat source0Pos[]={ -2.0f, 0.0, 0.0};
//		ALfloat source0Vel[]={ 0.0, 0.0, 0.0};
//
//		// Attach buffer 0 to source
//		alSourcei(source, AL_BUFFER, buffer);
//		alSourcef(source, AL_PITCH, 1.0f);
//		alSourcef(source, AL_GAIN, 1.0f);
//		alSourcefv(source, AL_POSITION, source0Pos);
//		alSourcefv(source, AL_VELOCITY, source0Vel);
//		alSourcei(source, AL_BUFFER,buffer);
//		alSourcei(source, AL_LOOPING, AL_TRUE);
//
//		ALfloat listenerPos[]={0.0,0.0,4.0};
//		ALfloat listenerVel[]={0.0,0.0,0.0};
//		ALfloat listenerOri[]={0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
//
//		alListenerfv(AL_POSITION,listenerPos);
//		alListenerfv(AL_VELOCITY,listenerVel);
//		alListenerfv(AL_ORIENTATION,listenerOri);
//
//		alSourcePlay(source);
//
////		// Exit
////		context = alcGetCurrentContext();
////		device = alcGetContextsDevice(Context);
////		alcMakeContextCurrent(NULL);
////		alcDestroyContext(Context);
////		alcCloseDevice(Device);
//	}
//
//	void OpenALPlayer::setPosition(float x, float y, float z) {
//		ALfloat listenerPos[]={x,y,z};
//		alListenerfv(AL_POSITION, listenerPos);
//	}
//}