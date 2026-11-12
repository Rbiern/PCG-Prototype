#pragma once

#include <vector>
#include <queue>


class Screen;

enum class TransitionType {Push, Pop, ReplaceAll, Close};
struct Transition {TransitionType type; Screen* state;};

class ScreenManager {
public:
    explicit ScreenManager() = default;
    ~ScreenManager();
    ScreenManager(const ScreenManager&) = delete;
    ScreenManager& operator=(const ScreenManager&) = delete;

    [[nodiscard]] Screen* activeScreen();
    [[nodiscard]] const Screen* activeScreen() const;
    void stateTransition();
    void pushState(Screen* state);
    void popState();
    void replaceAll(Screen* state);
    void close();

private:
    std::vector<Screen*> stateStack;
    std::queue<Transition> transitions;
};
