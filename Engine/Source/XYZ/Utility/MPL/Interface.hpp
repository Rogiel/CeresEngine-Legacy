#pragma once

#include <type_traits>
#include "Type.hpp"
#include "TypeList.hpp"
#include "Rename.hpp"
#include "Concat.hpp"
#include "Map.hpp"
#include "TypeListOps.hpp"
#include "Repeat.hpp"
#include "All.hpp"
#include "IndexOf.hpp"
#include "Count.hpp"
#include "ContainsAll.hpp"
#include "Filter.hpp"
#include "Unique.hpp"

namespace XYZ::Utility::MPL {

//	template<typename TTypeList>
//	constexpr auto size() noexcept {
//		return size<TTypeList>();
//	}
//
//	template<typename TTypeList, typename TFunction>
//	constexpr void forTypes(TFunction&& mFunction) noexcept {
//		forTypes<TTypeList>(std::forward(mFunction));
//	}
//
//	template<typename... Ts, typename TFunction>
//	constexpr void forTypesExp(TFunction&& mFunction) noexcept {
//		forTypes<TypeList<Ts...>>(std::forward(mFunction));
//	}
}
