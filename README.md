# Aerofootball
- Две команды, по три игрока в каждой команде, играют в футбол. (Настольный футбол)
- Пользователь может управлять одним игроком. Остальные управляются своим алгоритмом интелектом
- Вид сверху.
- Игроки имеют возможность дриблинга.
- Матч ограничем по времени и/или по количеству очков

## Build
```bash
    docker build -t build-env .
    docker run --rm -v ${PWD}:/project build-env bash -c "cd /project && cmake -B build/linux -G Ninja && cmake --build build/linux"
    docker run --rm -v ${PWD}:/project build-env bash -c "cd /project && cmake -B build/win -G Ninja -DCMAKE_TOOLCHAIN_FILE=mingw.cmake && cmake --build build/win"
```
