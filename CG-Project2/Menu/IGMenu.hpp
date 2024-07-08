#pragma once

class IGMenu {
private:

public:
    IGMenu() {}

    virtual void render() = 0;

    ~IGMenu() = default;
};
