# Statistics Basics

## Frequency table (度数分布表):

| #   | term                | description           | Ja |
|:---:|---------------------|-----------------------|----|
|     | dataset             |                       | 資料 |
|     | size                | the number of dataset | サイズ |
| CL  | class               | a.k.a. class interval | 階級 |
| MP  | midpoint            |                       | 階級値 |
| Fr  | frequency           |                       | 度数 |
| RF  | relative frequency  | frequency/size        | 相対度数 |

Example dataset:
```
  82, 67, 47, 78, 52, 84, 76, 73, 54, 91, 52, 67, 70, 68, 71, 84, 71, 61, 69, 43;
```

| CL          | MP  | Fr  | RF    |
|:-----------:|----:|----:|------:|
| [40,  50)   |  45 |   2 | 0.10  |
| [50,  60)   |  55 |   3 | 0.15  |
| [60,  70)   |  65 |   5 | 0.25  |
| [70,  80)   |  75 |   6 | 0.30  |
| [80,  90)   |  85 |   3 | 0.15  |
| [90, 100)   |  95 |   1 | 0.05  |

## Representative values (代表値)

| RP        | description                 | Ja    |
|-----------|-----------------------------|-------|
| min       | 43                          | 最小値 |
| max       | 91                          | 最大値 |
| range     | max - min = 91 -43 = 48     | 範囲 |
| mean      |                             | 平均値 |
| mode      | 75 as [70, 80) is of max Fr | 最頻値 |
| median    | (69 + 70) ÷ 2 = 69.5        | 中央値 |
| quartile  | see below                   | 四分位数 |

Quartiles are three values that split sorted data into four parts, each with an equal number of observations.
      
- The first quartile (Q1, or the lowest quartile) is the 25th percentile, meaning that 25% of the data falls below the first quartile.
- The second quartile (Q2, or the median) is the 50th percentile, meaning that 50% of the data falls below the second quartile.
- The third quartile (Q3, or the upper quartile) is the 75th percentile, meaning that 75% of the data falls below the third quartile.

To find the quartiles of a dataset or sample, follow the step-by-step guide below.

- Count the number of observations in the dataset (`n`).
- Sort the observations from smallest to largest.
- Find the first quartile:
  - Calculate `n * (1 / 4)`.
  - If `n * (1 / 4)` is an integer, then the first quartile is the mean of the numbers at positions `n * (1 / 4)` and `n * (1 / 4) + 1`.
  - If `n * (1 / 4)` is **not** an integer, then round it up. The number at this position is the first quartile.
- Find the second quartile:
  - Calculate  `n * (2 / 4)`.
  - If `n * (2 / 4)` is an integer, the second quartile is the mean of the numbers at positions `n * (2 / 4)` and `n * (2 / 4) + 1`.
  - If `n * (2 / 4)` is not an integer, then round it up. The number at this position is the second quartile.
- Find the third quartile:
  - Calculate `n * (3 / 4)`.
  - If `n * (3 / 4)` is an integer, then the third quartile is the mean of the numbers at positions `n * (3 / 4)` and `n * (3 / 4) + 1`.
  - If `n * (3 / 4)` is not an integer, then round it up. The number at this position is the third quartile.

