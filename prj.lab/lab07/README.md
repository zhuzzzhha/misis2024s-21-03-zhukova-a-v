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

### Ход работы
1. Выполним сегментацию с помощью `MeanShiftSegmentation()` параметры: radius = 5, diff = 100
Радиус влияет на размер области поиска при вычислении среднего значения в каждой итерации алгоритма. Более крупный радиус означает более широкую область поиска, что может привести к более гладким и более обобщенным сегментам, в то время как более маленький радиус сделает сегменты более точными и детализированными.
diff определяет критерий останова для алгоритма среднего сдвига. Он представляет собой пороговое значение, которое определяет, когда алгоритм должен остановиться. Если изменение вектора сдвига (Mean Shift Vector) меньше, чем это значение, алгоритм считается сойденным. Это позволяет определить, когда алгоритм достиг равновесия и сегментация завершена.
2. Выполним бинаризацию сегментированного изображения.
3. Вычислим TP, FP, FN, TN, Sensitivity, Specificity, Fallout, Precision, Accuracy, ROC AUC


### Подсчет качества сегментации
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
| ![](https://github.com/zhuzzzhha/misis2024s-21-03-zhukova-a-v/blob/main/images/lab_1/gr_truth_1.png) | ![](https://github.com/zhuzzzhha/misis2024s-21-03-zhukova-a-v/blob/main/images/lab_1/segmented_image1.png) |

| Параметр | Значение |
|---|---|
| Количество кругов | 100 |
| Минимальный радиус | 2 |
| Максимальный радиус | 31 |
| Минимальный контраст | 36 |
| Максимальный контраст | 219 |
| Std шума | 22 |
| Blur | 2 |

Вычисленные показатели 
| Показатель | Значение |
|---|---|
| TP | 25919 |
| FP | 48508 |
| FN | 194896 |
| TN | 777206 |
| Sensitivity (Recall, true positive rate) | 0.117 |
| Specificity (true negative rate) | 0.941 |
| Fallout (false positive rate) | 0.059 |
| Precision (Confidence) | 0.348 |
| Accuracy | 0.767 |
| Area under ROC Curve | 0.529 |

| Сгенерированное изображение | Сегментация |
|----------------------|------------|
| ![](https://github.com/zhuzzzhha/misis2024s-21-03-zhukova-a-v/blob/main/images/lab_1/gr_truth_2.png) | ![](https://github.com/zhuzzzhha/misis2024s-21-03-zhukova-a-v/blob/main/images/lab_1/segmented_image2.png) |

| Параметр | Значение |
|---|---|
| Количество кругов | 25 |
| Минимальный радиус | 7 |
| Максимальный радиус | 43 |
| Минимальный контраст | 36 |
| Максимальный контраст | 170 |
| Std шума | 39 |
| Blur | 2 |

Вычисленные показатели 
| Показатель | Значение |
|---|---|
| TP | 29711 |
| FP | 42316 |
| FN | 176879 |
| TN | 350170 |
| Sensitivity (Recall, true positive rate) | 0.144 |
| Specificity (true negative rate) | 0.892 |
| Fallout (false positive rate) | 0.108 |
| Precision (Confidence) | 0.412 |
| Accuracy | 0.634 |
| Area under ROC Curve | 0.518 |

| Сгенерированное изображение | Сегментация |
|----------------------|------------|
| ![](https://github.com/zhuzzzhha/misis2024s-21-03-zhukova-a-v/blob/main/images/lab_1/gr_truth_image3.png) | ![](https://github.com/zhuzzzhha/misis2024s-21-03-zhukova-a-v/blob/main/images/lab_1/segmented_image3.png) |

| Параметр | Значение |
|---|---|
| Количество кругов | 49 |
| Минимальный радиус | 7 |
| Максимальный радиус | 43 |
| Минимальный контраст | 36 |
| Максимальный контраст | 203 |
| Std шума | 23 |
| Blur | 2 |

Вычисленные показатели 
| Показатель | Значение |
|---|---|
| TP | 47909 |
| FP | 63307 |
| FN | 221124 |
| TN | 732684 |
| Sensitivity (Recall, true positive rate) | 0.178 |
| Specificity (true negative rate) | 0.920 |
| Fallout (false positive rate) | 0.080 |
| Precision (Confidence) |0.431 |
| Accuracy | 0.733 |
| Area under ROC Curve | 0.549 |

| Сгенерированное изображение | Сегментация |
|----------------------|------------|
| ![](https://github.com/zhuzzzhha/misis2024s-21-03-zhukova-a-v/blob/main/images/lab_1/gr_truth_image4.png) | ![](https://github.com/zhuzzzhha/misis2024s-21-03-zhukova-a-v/blob/main/images/lab_1/segmented_image4.png) |

| Параметр | Значение |
|---|---|
| Количество кругов | 100 |
| Минимальный радиус | 3 |
| Максимальный радиус | 76 |
| Минимальный контраст | 61 |
| Максимальный контраст | 177 |
| Std шума | 11 |
| Blur | 3 |

Вычисленные показатели 
| Показатель | Значение |
|---|---|
| TP | 176080 |
| FP | 329951 |
| FN | 5423218 |
| TN | 360815 |
| Sensitivity (Recall, true positive rate) | 0.328 |
| Specificity (true negative rate) | 0.943 |
| Fallout (false positive rate) | 0.057  |
| Precision (Confidence) | 0.348 |
| Accuracy | 0.890 |
| Area under ROC Curve | 0.635 |

| Сгенерированное изображение | Сегментация |
|----------------------|------------|
| ![](https://github.com/zhuzzzhha/misis2024s-21-03-zhukova-a-v/blob/main/images/lab_1/gr_truth_image5.png) | ![](https://github.com/zhuzzzhha/misis2024s-21-03-zhukova-a-v/blob/main/images/lab_1/segmented_image5.png) |

| Параметр | Значение |
|---|---|
| Количество кругов | 81 |
| Минимальный радиус | 2 |
| Максимальный радиус | 16 |
| Минимальный контраст | 87 |
| Максимальный контраст | 119 |
| Std шума | 40 |
| Blur | 2 |

Вычисленные показатели 
| Показатель | Значение |
|---|---|
| TP | 17189 |
| FP | 25927 |
| FN | 126958 |
| TN | 60326 |
| Sensitivity (Recall, true positive rate) | 0.222 |
| Specificity (true negative rate) | 0.830 |
| Fallout (false positive rate) | 0.170  |
| Precision (Confidence) | 0.399 |
| Accuracy | 0.626 |
| Area under ROC Curve | 0.526 |

Сравним значения функций оценки качества с результатами детекции из Лабораторной работы №4:

| Функция | Детекция (Лаб. 4) | Сегментация |
|---|---|---|
| Sensitivity | 0.117 | 0.13 |
| Precision |0.348 | 0.001|
| Accuracy | 0.767 | 0.13|

## Вывод
Метод сегментации показал более высокие значения при оценке качества.
