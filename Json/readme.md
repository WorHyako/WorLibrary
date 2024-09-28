# WorLibrary::Json

Library to load/save configs for WorLibrary classes. It's literally just wrapper around nlohmann::json 
to use only behaviour i want.

There is no API to manage logger because it's used only in WorLibrary source files.

---

## Dependencies

- WorLibrary
  - Log [v0.1]()
- nlohmann::json [v3.11.3](https://github.com/nlohmann/json/tree/v3.11.3)

---

## Config saving

```c++
#include "Wor/Json/JsonManager.hpp"

int main() {
    nlohmann::json config;
    constexpr std::string filePath{"resources/test.json"};
    bool savingResult{Wor::Json::tryToSaveFile(configPath, config.dump())};
    ...
    return 0;
}
```

## Config loading

```c++
#include "Wor/Json/JsonManager.hpp"

int main() {
    constexpr std::string filePath{"resources/test.json"};
    nlohmann::json config{Wor::Json::tryToLoadFile(filePath)};

    if (config.empty()) {
        ...
    }
    ...
    return 0;
}
```

--- 

### by Worshiper Hyakki Yakō(worHyako)
