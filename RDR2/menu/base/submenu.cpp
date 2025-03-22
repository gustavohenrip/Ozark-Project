#include "submenu.hpp"
#include "base.hpp"
#include "submenu_handler.hpp"
#include "renderer.hpp"
#include <algorithm>

namespace menu {
	void submenu::init() {}
	void submenu::updateOnce() {}
	void submenu::update() {}
	void submenu::featureUpdate() {}

	void submenu::updateMenus() {
		update();

		getRenderer()->drawTitle(name.get().c_str());

		getSubmenuHandler()->setTotalOptions(0);
		getSubmenuHandler()->setCurrentOptionWithoutBreaks(0);
		getSubmenuHandler()->setTotalOptionsWithoutBreaks(0);

		if (options.size() > 0) {
			std::vector<std::shared_ptr<option>> tempOptions;
			tempOptions.reserve(options.size());

			int relativePosition = 0;
			int visibilityOffset = 0;

			for (std::size_t i = 0; i < options.size(); i++) {
				if (options.at(i)->isVisible()) {
					tempOptions.push_back(options.at(i));
					visibilityOffset++;

					getSubmenuHandler()->setTotalOptions(getSubmenuHandler()->getTotalOptions() + 1);
					if (!options.at(i)->isBreak()) {
						if (visibilityOffset <= getBase()->currentOption) {
							getSubmenuHandler()->setCurrentOptionWithoutBreaks(getSubmenuHandler()->getCurrentOptionWithoutBreaks() + 1);
						}

						getSubmenuHandler()->setTotalOptionsWithoutBreaks(getSubmenuHandler()->getTotalOptionsWithoutBreaks() + 1);
					}
				}
			}

			for (int i = getBase()->scrollOffset; i < getBase()->scrollOffset + getBase()->maxOptions; i++) {
				if (i >= 0 && i < tempOptions.size()) {
					if (relativePosition >= getBase()->maxOptions) break;
					tempOptions.at(i).get()->render(relativePosition);
					if (relativePosition == getBase()->currentOption - getBase()->scrollOffset) {
						getRenderer()->drawTooltip(tempOptions.at(i).get()->getTooltip().get().c_str());
						if (getBase()->open) {
							tempOptions.at(i).get()->renderSelected(relativePosition, name.getOriginal());
						}
					}

					relativePosition++;
				}
			}
		}
	}

	void submenu::clearOptionsOffset(int offset) {
		if (offset > options.size()) return;
		options.resize(offset);
	}

	void submenu::setName(std::string n, bool translation, bool searchable) {
		name.set(n.c_str());
		name.setHasTranslation(translation);
		canBeSearched = searchable;
	}
}