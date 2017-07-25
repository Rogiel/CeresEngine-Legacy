//
// Created by Rogiel Sulzbach on 7/24/17.
//

#include "OggVorbisClipLoader.hpp"

#include <vorbis/vorbisfile.h>

#include <iostream>

namespace XYZ::Audio::Loader::OggVorbis {

	bool OggVorbisClipLoader::supports(const std::unique_ptr<Resource::Locator::ResourceStream>& resourceStream) {
		return false;
	}

	AudioClip::Ptr OggVorbisClipLoader::load(std::unique_ptr<Resource::Locator::ResourceStream> stream) {
		ov_callbacks cb = {};
		cb.read_func = [](void* ptr, size_t size, size_t nmemb, void* datasource) -> size_t {
			auto resourceStream = reinterpret_cast<Resource::Locator::ResourceStream*>(datasource);
			return (size_t) resourceStream->read((uint8_t*) ptr, size * nmemb);
		};
		cb.seek_func = [](void* datasource, ogg_int64_t offset, int whence) -> int {
			if(datasource == nullptr) return -1;

			auto resourceStream = reinterpret_cast<Resource::Locator::ResourceStream*>(datasource);
			switch(whence) {
				case SEEK_SET:
					resourceStream->seek(offset);
					return 0;

				case SEEK_CUR:
					resourceStream->seek(offset, Resource::Locator::ResourceStreamSeekType::CURRENT_POSITON);
					return 0;

				default:
					return -1;
			}
		};
		cb.close_func = [](void* datasource) -> int {
			if(datasource == nullptr) return -1;

			auto resourceStream = reinterpret_cast<Resource::Locator::ResourceStream*>(datasource);
			delete resourceStream;

			return 0;
		};
		cb.tell_func = [](void* datasource) -> long {
			if(datasource == nullptr) return -1;

			auto resourceStream = reinterpret_cast<Resource::Locator::ResourceStream*>(datasource);
			return resourceStream->tell();
		};

		OggVorbis_File vf = {};
		if(ov_open_callbacks(stream.release(), &vf, nullptr, 0, cb) != 0) {
			throw std::runtime_error("Could not open vorbis file");
		}

		vorbis_info vi = *ov_info(&vf, -1);
		AudioClip::Samples samples;

		char pcmout[4096];
		int current_section;

		while(true) {
			long ret = ov_read(&vf, pcmout, 4096, 0, 2, 1, &current_section);
			if(ret == 0) {
				break;
			} else if(ret < 0) {
				throw std::runtime_error("Could not read vorbis sample");
			} else {
				samples.resize(samples.size() + ret / 2);
				memcpy(samples.data() + samples.size() - ret / 2, pcmout, size_t(ret));
			}
		}

		ov_clear(&vf);

		return std::make_shared<SimpleAudioClip>(
				AudioMetadata(
						AudioMetadata::Channels(vi.channels),
						AudioMetadata::SampleRate(vi.rate),
						AudioMetadata::SampleCount(samples.size())
				),
				std::move(samples)
		);
	}

}