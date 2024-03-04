# Лабораторная работа №3: автоконтраст

## Введение
Цель работы - реализовать автоконтрастирование по квантилям черного и белого, для серых и цветных изображений.

## Реализация

### Подсчет гистограмм
Функция `calcHistogramColor()` вычисляет гистограмму для цветного изображения
        `calcHistogramGray()`  вычисляет гистограмму для серого изображения

### Подсчет квантилей
Функция `calculateQuantile()` вычисляет черный и белый квантили

### Вычисление минимального и максимального значений пикселей
Функция `calculateMinMaxFromHistogram()` вычисляет минимальное и максимальное знаечение яркости в изображении.

### Вычисление нового значения для контрастирования
Функция `calculateAutocontrast()`

### Контрастирование по каждому из каналов
Функция `autoContrastForEachChannel()` 

### Контрастирование серого изображения
`autoContrastForGray()`


## Результат
Цветное изображение

| Исходное изображение | По каналам | Общий контраст |
|----------------------|------------|----------------|
| ![](https://github.com/zhuzzzhha/misis2024s-21-03-zhukova-a-v/blob/main/images/lab_1/test_color.png) | ![](https://github.com/zhuzzzhha/misis2024s-21-03-zhukova-a-v/blob/main/images/lab_1/color_image_each_channels.png) | ![](https://github.com/zhuzzzhha/misis2024s-21-03-zhukova-a-v/blob/main/images/lab_1/color_image_all_channels.png) |

Серое изображение
| Исходное изображение | Автоконтраст |
|----------------------|--------------|
|![](https://github.com/zhuzzzhha/misis2024s-21-03-zhukova-a-v/blob/main/images/lab_1/test_gray.png) | ![](https://github.com/zhuzzzhha/misis2024s-21-03-zhukova-a-v/blob/main/images/lab_1/gray_image.png)


## Вывод
Разработанные функции позволяют генерировать тестовые изображения с различными уровнями яркости, а также анализировать параметры шума в изображениях.
