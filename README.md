# Engine
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![CMake](https://img.shields.io/badge/CMake-3.27+-064e8b.svg)](https://cmake.org)
[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org)
[![OpenGL](https://img.shields.io/badge/OpenGL-3.3+-5586a4.svg)](https://opengl.org)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)]()

Кроссплатформенная c++ библиотека, реализующая функционал игрового движка.
## Возможности
- **Кроссплатформенный рендеринг** через OpenGL
- **Поддержка TrueType шрифтов**
- **Загрузка популярных форматов изображений**
- **Обработка ввода** (клавиатура, мышь)
- **Интеграция через CMake**
## Лицензия
Проект распространяется под лицензией MIT. [LICENSE](LICENSE)
____
# Зависимости
| Название                                     | Предназначение |
|:---------------------------------------------|:---------------|
| [GLFW](https://www.glfw.org/)                | Обработка окна и событий ввода |
| [FreeType](https://freetype.org/)            | Загрузка шрифтов |
| [GLAD](https://glad.dav1d.de/)               | Линковка функций OpenGL |
| [stb_image](https://github.com/nothings/stb) | Загрузка изображений из файла |
____
# Сборка
## Требования
- CMake 3.27+
- C++17-совместимый компилятор
- OpenGL 3.3+
### Windows
```bash
# Вариант 1: Использовать готовый скрипт
GenerateRelease.bat
# Вариант 2: Вручную
mkdir build && cd build
cmake ..
cmake --build . --config Release
```
____
# Использование
Пример подключения к CMake-проекту
```cmake
list(APPEND CMAKE_PREFIX_PATH "полный путь до корневой директории")

find_package(Engine REQUIRED)

add_executable(...)

target_link_libraries(... PRIVATE Engine::Engine)
```
