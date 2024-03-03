**Разработка единой системы хранения и обработки датасета цифровых схем**

<font size="5">**Content**:</font>
<a name="content_rus"></a> 
- [Сборка и запуск компонентов](#build_rus)
    - [Генератор комбинационных схем](#generator_rus)
        - [Сборка программы](#generator_build_rus)
    - [Backend + frontend](#backend_frontend_rus)
        - [Установка](#install_rus)
        - [Запуск](#run_rus)
    

# Сборка и запуск компонентов
<a name="build_rus"></a> 

Сборка и запуск производятся из операционной системы Linux.

Программа запускалась на Ubuntu 22.04 с использованием WSL (Windows Subsystem for Linux).

## [Генератор комбинационных схем](Generator/README.md)
<a name="generator_rus"></a> 
### Сборка программы
<a name="generator_build_rus"></a> 

В первую очередь треубется установить все требуемые утилиты. Для этого выполните следующие команды: 
```
chmod +x installUtils.sh
sudo ./installUtils.sh
```

Для сборки программы необходимо выполнить следующую команду из начальной директории:
```
./buildGenerator
```

[&#8593; Contents](#content_rus)

## Backend + frontend
<a name="backend_frontend_rus"></a> 
### Установка
<a name="install_rus"></a> 
```
pip install -r requirements.txt

python manage.py makemigrations
python manage.py migrate
python manage.py migrate --run-syncdb
cd frontend
npm i
npm run build
cd ..
```

[&#8593; Contents](#content_rus)

### Запуск
<a name="run_rus"></a> 
```
python manage.py runserver
```

[&#8593; Contents](#content_rus)
# [Текущий датасет](https://vvzunin.me:10003/d/s/tVFkjEa5dJVgkpNCMirx37WFS3vxKPgU/tWINRKjvi7TCinaI8i5arDSuCxhKzd-o-X7RAk_qacAo)

# Планы на будущее
## Написать полностью с нуля свою архитектуру нейронной сети
## Исправить сомнительные участки в коде, которые были помечены в ходе работы
## Разработать больше тестов на тестируемые функции
## Разработать программу визуализации схем, которая будет заметно быстрее работать, чем yosys
## Реализовать кросс-платформенность

