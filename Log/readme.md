# WorLibrary::Log

Library to log all Wor's processes. It's literally just wrapper around spdlog to use only behaviour i want.

There is no API to manage logger because it's used only in WorLibrary source files. 

The option to disable it will be added in the future.

---

## Dependencies

- spdlog [v1.14.1_1](https://github.com/gabime/spdlog)

---

## Usage sample

```c++
#include "Wor/Log/Log.hpp"

int main() {
	Wor::Log::configureLogger();

	...	
	return 0;
}
```

---

## Options

- WOR_DEV_LOG - use deep logging with file line and function name info.

---

### by Worshiper Hyakki Yakō(worHyako)
