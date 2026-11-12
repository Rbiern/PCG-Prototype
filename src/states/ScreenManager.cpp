#include "ScreenManager.h"
#include "Screen.hpp"


ScreenManager::~ScreenManager() {
    while (!stateStack.empty()) {
        delete stateStack.back();
        stateStack.pop_back();
    }

    while (!transitions.empty()) {
        delete transitions.front().state;
        transitions.pop();
    }
}


Screen* ScreenManager::activeScreen() {
    return stateStack.empty() ? nullptr : stateStack.back();
}


const Screen* ScreenManager::activeScreen() const {
    return stateStack.empty() ? nullptr : stateStack.back();
}


void ScreenManager::stateTransition() {
    while (!transitions.empty()) {
        auto&[type, scene] = transitions.front();

        switch (type) {
            case TransitionType::Push:
                stateStack.push_back(scene);
                break;

            case TransitionType::Pop:
                if (!stateStack.empty()) {
                    const Screen* previous = stateStack.back();
                    delete previous;
                    stateStack.pop_back();
                }
                break;

            case TransitionType::ReplaceAll:
                for (Screen*& menu : stateStack) {
                    delete menu;
                }
                stateStack.clear();
                if (scene != nullptr) {
                    stateStack.push_back(scene);
                }
                break;
        }

        transitions.pop();
    }
}


void ScreenManager::pushState(Screen* state) {
    transitions.push({TransitionType::Push, state});
}


void ScreenManager::popState() {
    transitions.push({TransitionType::Pop, nullptr});
}


void ScreenManager::replaceAll(Screen* state) {
    transitions.push({TransitionType::ReplaceAll, state});
}
