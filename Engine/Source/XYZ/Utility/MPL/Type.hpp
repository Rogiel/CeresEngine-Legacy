#pragma once

namespace XYZ::Utility::MPL {

	// Simple wrapper for types that can be instantiated.
	template<typename T>
	struct Type {
		using type = T;
	};

}
