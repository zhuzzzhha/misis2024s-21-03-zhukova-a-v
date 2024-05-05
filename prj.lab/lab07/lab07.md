# Лабораторная работа №7: сегментация

## Введение
Цель работы - произвести сегментацию сгенерированных изображений и выполнить оценку качества сегментации с помощью различных функций сегментации.


## Реализация

### Генерация тестовых изображений
Функция `generateImageWithCircles()  
генерирует изображение с следующими параметрами:
int count - количество кругов в строке;
int min_radius - минимальный радиус круга;
int max_radius - максимальный радиус круга;
int min_contrast - минимальная яркость круга;
int max_contrast - максимальная яркость круга;
double std - стандартное отклонение нормального аддитивного гауссовского шума;
int blur - параметр размытия;
 bool bSave - сохранять/не сохранять в файл итоговое изображение;

### Сегментация
Выполним сегментацию с помощью `MeanShiftSegmentation()`


### Подсчет качества детекции
Для сформированных бинарных масок подсчитываем матрицу путаницы с помощью `ContingencyTable()` и вычисляем значений функций \
`Sensitivity (Recall, true positive rate)` Sensitivity = TP / (TP + FN) \
`Specificity (true negative rate)` Specificity = TN / (TN + FP) \
`Fallout (false positive rate)` Fallout = FP / (TN + FP) \
`Precision (Confidence)` Precision = TP / (TP + FP) \
`Accuracy` Accuracy = (TP + TN) / (TP + TN + FP + FN) \
`Area under ROC Curve` AUC = ∑ [(x_{i+1} - x_i) * (y_i + y_{i+1}) / 2] (от i=1 до n-1)

## Результат

| Сгенерированное изображение | Сегментация |
|----------------------|------------|
| ![](https://github.com/zhuzzzhha/misis2024s-21-03-zhukova-a-v/blob/main/images/lab_1/gr_truth_image0.png) | ![](https://github.com/zhuzzzhha/misis2024s-21-03-zhukova-a-v/blob/main/images/lab_1/segmented_image1.png) |

Параметры входного изображения:\
Количество кругов: 49 \
Std шума: 20 \
Blur: 2 \

Вычисленные показатели \
TP: 284410 \
FP: 18090 \
FN: 0 \
TN: 0 \

Sensitivity (Recall, true positive rate) = 1.000 \
Specificity (true negative rate) = 0.000 \
Fallout (false positive rate) = 1.000 \
Precision (Confidence) = 0.940 \
Accuracy = 0.940 \ 
Area under ROC Curve = 0.500 \


## Вывод
