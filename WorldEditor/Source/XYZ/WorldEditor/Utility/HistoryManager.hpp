//
// Created by Rogiel Sulzbach on 7/25/17.
//

#pragma once

#include <vector>

namespace XYZ::WorldEditor::Utility {

	/**
	 * The history manager class stores a copy of each internal state on the editor.
	 * On each change that gets made, a new item is added to the manager.
	 *
	 * @tparam Document the document type being managed
	 */
	template<typename Document>
	class HistoryManager {
	private:
		/**
		 * A list of all known document history
		 */
		std::vector<Document> history;

	public:
		/**
		 * Creates a new empty history manager
		 */
		HistoryManager() : history(1) {}

	public:
		/**
		 * Commits the latest changes to the manager.
		 */
		void commit() {
			assert(history.size());
			history.push_back(history.back());
		}

		/**
		 * Undos the last change made
		 */
		void undo() {
			assert(history.size());
			history.pop_back();
		}

		/**
		 * @return the current revision
		 */
		Document& current() {
			assert(history.size());
			return history.back();
		}

	};

}

