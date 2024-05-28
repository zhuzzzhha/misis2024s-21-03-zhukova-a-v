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
Выполним сегментацию с помощью `MeanShiftSegmentation()` параметры: radius = 5, diff = 100
Радиус влияет на размер области поиска при вычислении среднего значения в каждой итерации алгоритма. Более крупный радиус означает более широкую область поиска, что может привести к более гладким и более обобщенным сегментам, в то время как более маленький радиус сделает сегменты более точными и детализированными.
diff определяет критерий останова для алгоритма среднего сдвига. Он представляет собой пороговое значение, которое определяет, когда алгоритм должен остановиться. Если изменение вектора сдвига (Mean Shift Vector) меньше, чем это значение, алгоритм считается сойденным. Это позволяет определить, когда алгоритм достиг равновесия и сегментация завершена.


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
|![](https://github.com/zhuzzzhha/misis2024s-21-03-zhukova-a-v/blob/main/images/lab_1/gr_truth_image2.png) | ![](https://github.com/zhuzzzhha/misis2024s-21-03-zhukova-a-v/blob/main/images/lab_1/segmented_image3.png) |
|![](https://github.com/zhuzzzhha/misis2024s-21-03-zhukova-a-v/blob/main/images/lab_1/gr_truth_image4.png) | ![](https://github.com/zhuzzzhha/misis2024s-21-03-zhukova-a-v/blob/main/images/lab_1/segmented_image4.png) |

Параметры входного изображения:\
Количество кругов: 49 \
Std шума: 20 \
Blur: 2

Вычисленные показатели \
TP: 639120 \ 93060 \ 323484
FP: 41505 \ 6165 \ 18741
FN: 0 \
TN: 0

Sensitivity (Recall, true positive rate) = 1.000 \ 1.000
Specificity (true negative rate) = 0.000 \ 0.000
Fallout (false positive rate) = 1.000 \ 1.000
Precision (Confidence) = 0.939 \ 0.938 \ 0.945
Accuracy = 0.939\ 0.938 \ 0.945


## Вывод