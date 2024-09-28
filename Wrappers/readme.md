# WorLibrary::Wrappers

Library contains template wrappers like Singleton.

---

## Singleton wrapper

```c++
#include "Wor/Wrappers/Singleton.hpp"

int main() {
    Foo &foo_static_ref = Wor::Wrappers::Singleton<Foo>::get();
    ...
    return 0;
}
```

--- 

### by Worshiper Hyakki Yakō(worHyako)
