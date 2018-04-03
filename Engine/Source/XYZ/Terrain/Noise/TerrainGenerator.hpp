//
// Created by Rogiel Sulzbach on 8/6/17.
//

#pragma once

#include <random>

namespace XYZ::Terrain::Noise {

	class TerrainGenerator {
	private:
		std::mt19937 device;

	public:
		float noise(float x, float y);

	};

}

