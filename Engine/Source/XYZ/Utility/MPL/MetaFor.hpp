#pragma once

#include <tuple>
#include <initializer_list>

namespace XYZ::Utility::MPL {

	template<typename TFunction, typename TTuple,
			std::size_t... TIndices>
	constexpr decltype(auto) tupleApplyImpl(TFunction&& mFunction,
											TTuple&& mTuple, std::index_sequence<TIndices...>) {
		return std::forward<decltype(mFunction)>(mFunction)(
				std::get<TIndices>(std::forward<decltype(mTuple)>(mTuple))...);
	}

	template<typename TFunction, typename TTuple>
	constexpr decltype(auto) tupleApply(
			TFunction&& mFunction, TTuple&& mTuple) {
		using Indices = std::make_index_sequence<
				std::tuple_size<std::decay_t<TTuple>>::value>;
		return tupleApplyImpl(
				std::forward<decltype(mFunction)>(mFunction), std::forward<decltype(mTuple)>(mTuple), Indices{});
	}

	template<typename TFunction, typename... Ts>
	constexpr decltype(auto) forArgs(
			TFunction&& mFunction, Ts&& ... mArgs) {
		return (void) std::initializer_list<int>{
				(mFunction(std::forward<decltype(mArgs)>(mArgs)), 0)...};
	}

	template<typename TFunction, typename TTuple>
	constexpr decltype(auto) forTuple(
			TFunction&& mFunction, TTuple&& mTuple) {
		return tupleApply(
				[&mFunction](auto&& ... xs) {
					forArgs(mFunction, std::forward<decltype(xs)>(xs)...);
				},
				std::forward<decltype(mTuple)>(mTuple));
	}

}