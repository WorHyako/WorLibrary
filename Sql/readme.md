# WorLibrary::Sql

Library contains classes to work with DBs (with MySQL only yet).
It's literally just wrapper around `soci` to use only behaviour i want.

There is just connection implementation, but library in progress now...

---

## Dependencies

- WorLibrary
    - Converter [v0.1]()
- soci [v4.0.3](https://github.com/SOCI/soci/tree/v4.0.3)

---

## Connecting to MySql data base

```c++
#include "Wor/Sql/MySqlManager.hpp"

int main() {
    constexpr std::string auth_data{"dbType=mysql "
        "dbName=db_name "
        "user=user "
        "password=user "
        "host=127.0.0.1 "
        "port=3306"
    };
    Sql::MySqlManager &manager = Wrappers::Singleton<Sql::MySqlManager>::get();
    manager.configure(Mss::ConfData::authParameters);
    bool connect_res = manager.tryToConnect();
    ...
    return 0;
}
```

--- 

### by Worshiper Hyakki Yakō(worHyako)
