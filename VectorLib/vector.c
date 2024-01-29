#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>
#include "vector.h"
/*******************************************分割线*******************************************/
/*
    下面是该文件下所有函数的声明，除了初始化函数
*/
// 下面是VectorGenerator中的函数
static Vector *double_vector(int size);
static Vector *float_vector(int size);
static Vector *int_vector(int size);
Vector *generate_EmptyVector(int type, int size);
Vector *generate_ZerosVector(int type, int size);
Vector *generate_OnesVector(int type, int size);
Vector *generate_RandomVector(int type, int size, double min, double max);
Vector *generate_RangeVector(int type, double start, double end, double step);
Vector *generate_NormalVector(int type, int size, double mean, double std);
static double generate_NormalRandom(double mean, double std);

// 下面是VectorCopyer中的函数
Vector *copy_DoubleArray_to_NewVector(double *array, int size);
Vector *copy_FloatArray_to_NewVector(float *array, int size);
Vector *copy_IntArray_to_NewVector(int *array, int size);
Vector *copy_DoubleArray_to_ExistingVector(double *array, int size, Vector *vector);
Vector *copy_FloatArray_to_ExistingVector(float *array, int size, Vector *vector);
Vector *copy_IntArray_to_ExistingVector(int *array, int size, Vector *vector);
Vector *copy_Vector_to_NewVector(Vector vector);
Vector *copy_Vector_to_ExistingVector(Vector *vector, Vector source_vector);

// 下面是VectorConverter中的函数
Vector *convert_DoubleArray_to_NewVector(double **array, int size);
Vector *convert_FloatArray_to_NewVector(float **array, int size);
Vector *convert_IntArray_to_NewVector(int **array, int size);
Vector *convert_DoubleArray_to_ExistingVector(double **array, int size, Vector *vector);
Vector *convert_FloatArray_to_ExistingVector(float **array, int size, Vector *vector);
Vector *convert_IntArray_to_ExistingVector(int **array, int size, Vector *vector);
Vector *toInt(Vector *vector);
Vector *toFloat(Vector *vector);
Vector *toDouble(Vector *vector);
Vector *shuffle(Vector *vector);

// 下面是VectorDeleter中的函数
void delete_vector(Vector **vector);

// 下面是VectorCalculator中的函数
Vector *add_array(Vector *vectors, int count);
Vector *add_variadic(int count, ...);
Vector *multiple_vector(Vector vector, double times);
Vector *subtract_array(Vector *vectors, int count);
Vector *subtract_variadic(int count, ...);
double dot_product(Vector v1, Vector v2);
double mean(Vector vector);
double variance(Vector vector);
double standard(Vector vector);
double covariance(Vector v1, Vector v2);
double correlation(Vector v1, Vector v2);
Vector *StandardScaler(Vector *vector);
Vector *MinMaxScaler(Vector *vector);
double lp_norm(Vector vector, int p);

// 下面是VectorResizer中的函数
Vector *slice(Vector vector, int start, int end, int step);
Vector *extend(Vector v1, Vector v2);
Vector *fancy_indexing(Vector data, Vector indices);
Vector *boolean_indexing(Vector data, Vector mask);

// 下面是VectorComparator中的函数
static void merge(Vector *vector, int left, int mid, int right);
static void merge_sort(Vector *vector, int left, int right);
Vector *sort(Vector vector);
static void mergeArgsort(Vector *vector, int *indices, int left, int mid, int right);
static void mergeSortArgsort(Vector *vector, int *indices, int left, int right);
Vector *argsort(Vector vector);
double find_max(Vector vector);
int argmax(Vector vector);
double find_min(Vector vector);
int argmin(Vector vector);
Vector *get_BooleanVector_for_condition(Vector vector, char *condition, double value);
/*
    上面是该文件下所有函数的声明，除了初始化函数
*/
/*******************************************分割线*******************************************/

/*******************************************分割线*******************************************/
/*
    以下是用来生成特定向量的函数，包括空向量、零向量、一向量、均匀分布向量、正态分布向量、随机向量
    通过VectorGenerator调用
*/
// 生成一个double类型的空向量
Vector *double_vector(int size)
{
    Vector *vector = (Vector *)malloc(sizeof(Vector));         // 分配内存空间给一个Vector结构体
    vector->d_array = (double *)malloc(size * sizeof(double)); // 分配内存空间给一个double类型的数组
    vector->f_array = NULL;                                    // float类型的数组设为NULL
    vector->i_array = NULL;                                    // int类型的数组设为NULL
    vector->size = size;                                       // 设置向量的大小
    vector->type = DOUBLE;                                     // 设置向量的类型为double
    return vector;                                             // 返回生成的向量
}

// 生成一个float类型的空向量
Vector *float_vector(int size)
{
    Vector *vector = (Vector *)malloc(sizeof(Vector));       // 分配内存空间给一个Vector结构体
    vector->f_array = (float *)malloc(size * sizeof(float)); // 分配内存空间给一个float类型的数组
    vector->d_array = NULL;                                  // double类型的数组设为NULL
    vector->i_array = NULL;                                  // int类型的数组设为NULL
    vector->size = size;                                     // 设置向量的大小
    vector->type = FLOAT;                                    // 设置向量的类型为float
    return vector;                                           // 返回生成的向量
}

// 生成一个int类型的空向量
Vector *int_vector(int size)
{
    Vector *vector = (Vector *)malloc(sizeof(Vector));   // 分配内存空间给一个Vector结构体
    vector->i_array = (int *)malloc(size * sizeof(int)); // 分配内存空间给一个int类型的数组
    vector->f_array = NULL;                              // float类型的数组设为NULL
    vector->d_array = NULL;                              // double类型的数组设为NULL
    vector->size = size;                                 // 设置向量的大小
    vector->type = INT;                                  // 设置向量的类型为int
    return vector;                                       // 返回生成的向量
}

// 生成指定数据类型和大小的空向量
Vector *generate_EmptyVector(int type, int size)
{ // 确保向量的大小大于0
    assert(size > 0 && "\nError: The size of the vector must be greater than 0.\n");
    if (type == INT)                // 如果类型是int
        return int_vector(size);    // 调用int_vector函数生成int类型的向量
    else if (type == FLOAT)         // 如果类型是float
        return float_vector(size);  // 调用float_vector函数生成float类型的向量
    else if (type == DOUBLE)        // 如果类型是double
        return double_vector(size); // 调用double_vector函数生成double类型的向量
    else
        // 如果类型不是以上三种，报错
        assert(0 && "\nError: Unsupported type. The 'type' parameter must be DOUBLE, FLOAT, or INT.\n");
    return &(Vector){0}; // 返回一个空的Vector结构体
}

// 生成一个全零向量
Vector *generate_ZerosVector(int type, int size)
{
    // 生成一个空向量
    Vector *v = generate_EmptyVector(type, size);
    // 设置向量的类型和大小
    v->type = type;
    v->size = size;
    // 如果向量的类型是整数
    if (type == INT)
        // 将向量的所有元素设置为0
        for (int i = 0; i < size; ++i)
            v->i_array[i] = 0;
    // 如果向量的类型是浮点数
    else if (type == FLOAT)
        // 将向量的所有元素设置为0
        for (int i = 0; i < size; ++i)
            v->f_array[i] = 0;
    // 如果向量的类型是双精度浮点数
    else if (type == DOUBLE)
        // 将向量的所有元素设置为0
        for (int i = 0; i < size; ++i)
            v->d_array[i] = 0;
    // 返回生成的向量
    return v;
}

// 生成一个全一向量
Vector *generate_OnesVector(int type, int size)
{
    // 生成一个空向量
    Vector *v = generate_EmptyVector(type, size);
    // 设置向量的类型和大小
    v->type = type;
    v->size = size;
    // 如果向量的类型是整数
    if (type == INT)
        // 将向量的所有元素设置为1
        for (int i = 0; i < size; ++i)
            v->i_array[i] = 1;
    // 如果向量的类型是浮点数
    else if (type == FLOAT)
        // 将向量的所有元素设置为1
        for (int i = 0; i < size; ++i)
            v->f_array[i] = 1;
    // 如果向量的类型是双精度浮点数
    else if (type == DOUBLE)
        // 将向量的所有元素设置为1
        for (int i = 0; i < size; ++i)
            v->d_array[i] = 1;
    // 返回生成的向量
    return v;
}

// 生成一个随机向量
Vector *generate_RandomVector(int type, int size, double min, double max)
{
    // 确保最小值小于最大值
    assert(min < max && "\nError: The min must be less than max.\n");

    // 初始化随机数生成器
    srand(time(NULL));

    // 生成一个空向量
    Vector *v = generate_EmptyVector(type, size);
    v->type = type;
    v->size = size;

    // 如果向量的类型是整数
    if (type == INT)
    {

        int int_min = (int)min;
        int int_max = (int)max;

        int is_max_in_range_of_min = (int_min < max) && (max < int_min + 1);
        int is_min_in_range_of_max = (int_max < min) && (min < int_max + 1);
        if (is_max_in_range_of_min && is_min_in_range_of_max)
            assert(0 && "Error: There must be at least one integer between the upper and lower bounds.");

        if (int_min == int_max)
        {
            for (int i = 0; i < size; ++i)
                v->i_array[i] = int_min;
        }

        int lowerBound = (int_min < min) ? int_min + 1 : int_min; // 向上取整
        int upperBound = int_max;                                 // 向下取整
        // 将向量的所有元素设置为随机整数
        for (int i = 0; i < size; ++i)
            v->i_array[i] = rand() % (upperBound - lowerBound + 1) + lowerBound;
    }

    // 如果向量的类型是浮点数
    else if (type == FLOAT)
        // 将向量的所有元素设置为随机浮点数
        for (int i = 0; i < size; ++i)
            v->f_array[i] = (float)(min + (rand() / (float)RAND_MAX) * (max - min));

    // 如果向量的类型是双精度浮点数
    else if (type == DOUBLE)
        // 将向量的所有元素设置为随机双精度浮点数
        for (int i = 0; i < size; ++i)
            v->d_array[i] = min + (rand() / (double)RAND_MAX) * (max - min);

    // 返回生成的向量
    return v;
}

// 生成一个范围向量的函数
Vector *generate_RangeVector(int type, double start, double end, double step)
{
    // 计算向量的大小
    int size = (int)((end - start) / step) + 1;
    // 生成一个空向量
    Vector *v = generate_EmptyVector(type, size);
    // 设置向量的类型和大小
    v->type = type;
    v->size = size;

    // 如果向量的类型是整数
    if (type == INT)
    {
        // 将向量的所有元素设置为在给定的开始值和结束值之间的数值，步长为给定的步长
        for (int i = 0; i < size; ++i)
        {
            v->i_array[i] = (int)(start + i * step);
        }
    }
    // 如果向量的类型是浮点数
    else if (type == FLOAT)
    {
        // 将向量的所有元素设置为在给定的开始值和结束值之间的数值，步长为给定的步长
        for (int i = 0; i < size; ++i)
        {
            v->f_array[i] = (float)(start + i * step);
        }
    }
    // 如果向量的类型是双精度浮点数
    else if (type == DOUBLE)
    {
        // 将向量的所有元素设置为在给定的开始值和结束值之间的数值，步长为给定的步长
        for (int i = 0; i < size; ++i)
        {
            v->d_array[i] = start + i * step;
        }
    }

    // 返回生成的向量
    return v;
}

// 用于生成正态分布随机数的函数
double generate_NormalRandom(double mean, double std)
{
    // 使用Box-Muller变换生成正态分布随机数
    // 首先，生成两个在[0, 1)范围内的均匀分布的随机数u和v
    double u = (double)rand() / RAND_MAX;
    double v = (double)rand() / RAND_MAX;
    // 然后，使用Box-Muller变换公式生成标准正态分布（均值为0，标准差为1）的随机数x
    double x = sqrt(-2.0 * log(u)) * cos(2.0 * M_PI * v);
    // 最后，将x转换为均值为mean，标准差为std的正态分布的随机数
    return mean + std * x;
}

// 生成一个正态分布的向量的函数
Vector *generate_NormalVector(int type, int size, double mean, double std)
{
    // 首先，生成一个空向量v
    Vector *v = generate_EmptyVector(type, size);
    // 设置向量的类型和大小
    v->type = type;
    v->size = size;

    // 如果向量的类型是整数
    if (type == INT)
    {
        // 将向量的所有元素设置为均值为mean，标准差为std的正态分布的随机整数
        for (int i = 0; i < size; ++i)
        {
            v->i_array[i] = (int)round(generate_NormalRandom(mean, std));
        }
    }
    // 如果向量的类型是浮点数
    else if (type == FLOAT)
    {
        // 将向量的所有元素设置为均值为mean，标准差为std的正态分布的随机浮点数
        for (int i = 0; i < size; ++i)
        {
            v->f_array[i] = (float)generate_NormalRandom(mean, std);
        }
    }
    // 如果向量的类型是双精度浮点数
    else if (type == DOUBLE)
    {
        // 将向量的所有元素设置为均值为mean，标准差为std的正态分布的随机双精度浮点数
        for (int i = 0; i < size; ++i)
        {
            v->d_array[i] = generate_NormalRandom(mean, std);
        }
    }

    // 返回生成的向量
    return v;
}

/*
    以上是用来生成特定向量的函数，包括空向量、零向量、一向量、均匀分布向量、正态分布向量、随机向量
    通过vector_generator调用
*/
/*******************************************分割线*******************************************/

/*******************************************分割线*******************************************/
/*
    通过vector_copyer调用
*/
// 将double类型的数组转化为向量
Vector *copy_DoubleArray_to_NewVector(double *array, int size)
{ // 确保数组的大小大于0
    assert(size > 0 && "\nError: The size of the array must be greater than 0.\n");
    Vector *vector = generate_EmptyVector(DOUBLE, size); // 生成一个double类型的空向量
    for (int i = 0; i < size; i++)                       // 遍历数组
        vector->d_array[i] = (double)array[i];           // 将数组的元素赋值给向量
    return vector;                                       // 返回生成的向量
}

// 将float类型的数组转化为向量
Vector *copy_FloatArray_to_NewVector(float *array, int size)
{ // 确保数组的大小大于0
    assert(size > 0 && "\nError: The size of the array must be greater than 0.\n");
    Vector *vector = generate_EmptyVector(FLOAT, size); // 生成一个float类型的空向量
    for (int i = 0; i < size; i++)                      // 遍历数组
        vector->f_array[i] = (float)array[i];           // 将数组的元素赋值给向量
    return vector;                                      // 返回生成的向量
}

// 将int类型的数组转化为向量
Vector *copy_IntArray_to_NewVector(int *array, int size)
{ // 确保数组的大小大于0
    assert(size > 0 && "\nError: The size of the array must be greater than 0.\n");
    Vector *vector = generate_EmptyVector(INT, size); // 生成一个int类型的空向量
    for (int i = 0; i < size; i++)                    // 遍历数组
        vector->i_array[i] = (int)array[i];           // 将数组的元素赋值给向量
    return vector;                                    // 返回生成的向量
} // 这个函数将一个double数组复制到一个已存在的Vector中
Vector *copy_DoubleArray_to_ExistingVector(double *array, int size, Vector *vector)
{
    // 如果Vector原来的类型是INT，释放对应的内存
    if (vector->type == INT)
    {
        free(vector->i_array);
        vector->i_array = NULL;
    }
    // 如果Vector原来的类型是FLOAT，释放对应的内存
    else if (vector->type == FLOAT)
    {
        free(vector->f_array);
        vector->f_array = NULL;
    }
    // 如果Vector原来的类型是DOUBLE，释放对应的内存
    else if (vector->type == DOUBLE)
    {
        free(vector->d_array);
        vector->d_array = NULL;
    }
    // 设置Vector的类型为DOUBLE
    vector->type = DOUBLE;
    // 设置Vector的大小
    vector->size = size;
    // 为Vector分配内存
    vector->d_array = (double *)malloc(sizeof(double) * size);
    // 将数组的元素复制到Vector中
    for (int i = 0; i < size; ++i)
        vector->d_array[i] = array[i];

    // 返回Vector的指针
    return vector;
}

// 这个函数将一个float数组复制到一个已存在的Vector中
Vector *copy_FloatArray_to_ExistingVector(float *array, int size, Vector *vector)
{
    // 如果Vector原来的类型是INT，释放对应的内存
    if (vector->type == INT)
    {
        free(vector->i_array);
        vector->i_array = NULL;
    }
    // 如果Vector原来的类型是FLOAT，释放对应的内存
    else if (vector->type == FLOAT)
    {
        free(vector->f_array);
        vector->f_array = NULL;
    }
    // 如果Vector原来的类型是DOUBLE，释放对应的内存
    else if (vector->type == DOUBLE)
    {
        free(vector->d_array);
        vector->d_array = NULL;
    }
    // 设置Vector的类型为FLOAT
    vector->type = FLOAT;
    // 设置Vector的大小
    vector->size = size;
    // 为Vector分配内存
    vector->f_array = (float *)malloc(sizeof(float) * size);
    // 将数组的元素复制到Vector中
    for (int i = 0; i < size; ++i)
        vector->f_array[i] = array[i];

    // 返回Vector的指针
    return vector;
}

// 这个函数将一个int数组复制到一个已存在的Vector中
Vector *copy_IntArray_to_ExistingVector(int *array, int size, Vector *vector)
{
    // 如果Vector原来的类型是INT，释放对应的内存
    if (vector->type == INT)
    {
        free(vector->i_array);
        vector->i_array = NULL;
    }
    // 如果Vector原来的类型是FLOAT，释放对应的内存
    else if (vector->type == FLOAT)
    {
        free(vector->f_array);
        vector->f_array = NULL;
    }
    // 如果Vector原来的类型是DOUBLE，释放对应的内存
    else if (vector->type == DOUBLE)
    {
        free(vector->d_array);
        vector->d_array = NULL;
    }
    // 设置Vector的类型为INT
    vector->type = INT;
    // 设置Vector的大小
    vector->size = size;
    // 为Vector分配内存
    vector->i_array = (int *)malloc(sizeof(int) * size);
    // 将数组的元素复制到Vector中
    for (int i = 0; i < size; ++i)
        vector->i_array[i] = array[i];

    // 返回Vector的指针
    return vector;
}

// 这个函数将一个Vector复制到一个新的Vector中
Vector *copy_Vector_to_NewVector(Vector vector)
{
    // 根据Vector的类型，调用相应的函数
    if (vector.type == INT)
        return copy_IntArray_to_NewVector(vector.i_array, vector.size);
    else if (vector.type == FLOAT)
        return copy_FloatArray_to_NewVector(vector.f_array, vector.size);
    else if (vector.type == DOUBLE)
        return copy_DoubleArray_to_NewVector(vector.d_array, vector.size);
}

// 这个函数将一个Vector复制到一个已存在的Vector中
Vector *copy_Vector_to_ExistingVector(Vector *vector, Vector source_vector)
{
    // 根据Vector的类型，调用相应的函数
    if (vector->type == INT)
        return copy_IntArray_to_ExistingVector(source_vector.i_array, source_vector.size, vector);
    else if (vector->type == FLOAT)
        return copy_FloatArray_to_ExistingVector(source_vector.f_array, source_vector.size, vector);
    else if (vector->type == DOUBLE)
        return copy_DoubleArray_to_ExistingVector(source_vector.d_array, source_vector.size, vector);
}

/*
    通过vector_copyer调用
*/
/*******************************************分割线*******************************************/

/*******************************************分割线*******************************************/
/*
    以下是将数组转化为向量的函数，变换向量中数据类型的函数，打乱向量顺序的函数
    通过vector_converter调用
*/
// 将向量的数据类型变为int
Vector *toInt(Vector *vector)
{
    if (vector->type == INT) // 如果向量的类型已经是int，直接返回
        return vector;
    if (vector->type == FLOAT) // 如果向量的类型是float
    {
        vector->i_array = (int *)malloc(vector->size * sizeof(int)); // 分配内存空间给一个int类型的数组
        for (int i = 0; i < vector->size; i++)                       // 遍历向量
            vector->i_array[i] = (int)vector->f_array[i];            // 将向量的元素转化为int类型并赋值给新的数组
        free(vector->f_array);                                       // 释放原来的float类型的数组
        vector->f_array = NULL;                                      // 将float类型的数组设为NULL
        vector->type = INT;                                          // 将向量的类型设为int
        return vector;
    }
    else if (vector->type == DOUBLE) // 如果向量的类型是double
    {
        vector->i_array = (int *)malloc(vector->size * sizeof(int)); // 分配内存空间给一个int类型的数组
        for (int i = 0; i < vector->size; i++)                       // 遍历向量
            vector->i_array[i] = (int)vector->d_array[i];            // 将向量的元素转化为int类型并赋值给新的数组
        free(vector->d_array);                                       // 释放原来的double类型的数组
        vector->d_array = NULL;                                      // 将double类型的数组设为NULL
        vector->type = INT;                                          // 将向量的类型设为int
        return vector;
    }
}

// 将向量的数据类型变为float
Vector *toFloat(Vector *vector)
{
    if (vector->type == FLOAT) // 如果向量的类型已经是float，直接返回
        return vector;
    if (vector->type == INT) // 如果向量的类型是int
    {
        vector->f_array = (float *)malloc(vector->size * sizeof(float)); // 分配内存空间给一个float类型的数组
        for (int i = 0; i < vector->size; i++)                           // 遍历向量
            vector->f_array[i] = (float)vector->i_array[i];              // 将向量的元素转化为float类型并赋值给新的数组
        free(vector->i_array);                                           // 释放原来的int类型的数组
        vector->i_array = NULL;                                          // 将int类型的数组设为NULL
        vector->type = FLOAT;                                            // 将向量的类型设为float
        return vector;
    }
    else if (vector->type == DOUBLE) // 如果向量的类型是double
    {
        vector->f_array = (float *)malloc(vector->size * sizeof(float)); // 分配内存空间给一个float类型的数组
        for (int i = 0; i < vector->size; i++)                           // 遍历向量
            vector->f_array[i] = (float)vector->d_array[i];              // 将向量的元素转化为float类型并赋值给新的数组
        free(vector->d_array);                                           // 释放原来的double类型的数组
        vector->d_array = NULL;                                          // 将double类型的数组设为NULL
        vector->type = FLOAT;                                            // 将向量的类型设为float
        return vector;
    }
}

// 将向量的数据类型变为double
Vector *toDouble(Vector *vector)
{
    if (vector->type == DOUBLE) // 如果向量的类型已经是double，直接返回
        return vector;
    if (vector->type == INT) // 如果向量的类型是int
    {
        vector->d_array = (double *)malloc(vector->size * sizeof(double)); // 分配内存空间给一个double类型的数组
        for (int i = 0; i < vector->size; i++)                             // 遍历向量
            vector->d_array[i] = (double)vector->i_array[i];               // 将向量的元素转化为double类型并赋值给新的数组
        free(vector->i_array);                                             // 释放原来的int类型的数组
        vector->i_array = NULL;                                            // 将int类型的数组设为NULL
        vector->type = DOUBLE;                                             // 将向量的类型设为double
        return vector;
    }
    else if (vector->type == FLOAT) // 如果向量的类型是float
    {
        vector->d_array = (double *)malloc(vector->size * sizeof(double)); // 分配内存空间给一个double类型的数组
        for (int i = 0; i < vector->size; i++)                             // 遍历向量
            vector->d_array[i] = (double)vector->f_array[i];               // 将向量的元素转化为double类型并赋值给新的数组
        free(vector->f_array);                                             // 释放原来的float类型的数组
        vector->f_array = NULL;                                            // 将float类型的数组设为NULL
        vector->type = DOUBLE;                                             // 将向量的类型设为double
        return vector;
    }
}

// 这个函数将一个double数组转换为一个新的Vector
Vector *convert_DoubleArray_to_NewVector(double **array, int size)
{
    // 使用copy_DoubleArray_to_NewVector函数将数组复制到一个新的Vector中
    Vector *result = copy_DoubleArray_to_NewVector(*array, size);
    // 释放原数组的内存
    free(*array);
    // 将数组指针设置为NULL
    *array = NULL;
    // 返回新的Vector
    return result;
}

// 这个函数将一个float数组转换为一个新的Vector
Vector *convert_FloatArray_to_NewVector(float **array, int size)
{
    // 使用copy_FloatArray_to_NewVector函数将数组复制到一个新的Vector中
    Vector *result = copy_FloatArray_to_NewVector(*array, size);
    // 释放原数组的内存
    free(*array);
    // 将数组指针设置为NULL
    *array = NULL;
    // 返回新的Vector
    return result;
}

// 这个函数将一个int数组转换为一个新的Vector
Vector *convert_IntArray_to_NewVector(int **array, int size)
{
    // 使用copy_IntArray_to_NewVector函数将数组复制到一个新的Vector中
    Vector *result = copy_IntArray_to_NewVector(*array, size);
    // 释放原数组的内存
    free(*array);
    // 将数组指针设置为NULL
    *array = NULL;
    // 返回新的Vector
    return result;
}

// 这个函数将一个double数组复制到一个已存在的Vector中
Vector *convert_DoubleArray_to_ExistingVector(double **array, int size, Vector *vector)
{
    // 使用copy_DoubleArray_to_ExistingVector函数将数组复制到已存在的Vector中
    copy_DoubleArray_to_ExistingVector(*array, size, vector);
    // 释放原数组的内存
    free(*array);
    // 将数组指针设置为NULL
    *array = NULL;
    // 返回已存在的Vector
    return vector;
}

// 这个函数将一个float数组复制到一个已存在的Vector中
Vector *convert_FloatArray_to_ExistingVector(float **array, int size, Vector *vector)
{
    // 使用copy_FloatArray_to_ExistingVector函数将数组复制到已存在的Vector中
    copy_FloatArray_to_ExistingVector(*array, size, vector);
    // 释放原数组的内存
    free(*array);
    // 将数组指针设置为NULL
    *array = NULL;
    // 返回已存在的Vector
    return vector;
}

// 这个函数将一个int数组复制到一个已存在的Vector中
Vector *convert_IntArray_to_ExistingVector(int **array, int size, Vector *vector)
{
    // 使用copy_IntArray_to_ExistingVector函数将数组复制到已存在的Vector中
    copy_IntArray_to_ExistingVector(*array, size, vector);
    // 释放原数组的内存
    free(*array);
    // 将数组指针设置为NULL
    *array = NULL;
    // 返回已存在的Vector
    return vector;
}

// 定义一个名为 shuffle 的函数，它接收一个 Vector 类型的指针作为参数
Vector *shuffle(Vector *vector)
{
    // 使用当前时间作为随机数生成器的种子
    srand(time(NULL));
    // 如果 vector 的类型是 INT
    if (vector->type == INT)
    {
        // 使用 Fisher-Yates 算法打乱数组
        for (int i = vector->size - 1; i > 0; i--)
        {
            // 生成一个 [0, i] 范围内的随机数
            int j = rand() % (i + 1);
            // 交换 i_array 数组中的第 i 和第 j 个元素
            int temp = vector->i_array[i];
            vector->i_array[i] = vector->i_array[j];
            vector->i_array[j] = temp;
        }
        // 返回打乱后的 vector
        return vector;
    }
    // 如果 vector 的类型是 FLOAT
    else if (vector->type == FLOAT)
    {
        // 使用 Fisher-Yates 算法打乱数组
        for (int i = vector->size - 1; i > 0; i--)
        {
            // 生成一个 [0, i] 范围内的随机数
            int j = rand() % (i + 1);
            // 交换 f_array 数组中的第 i 和第 j 个元素
            float temp = vector->f_array[i];
            vector->f_array[i] = vector->f_array[j];
            vector->f_array[j] = temp;
        }
        // 返回打乱后的 vector
        return vector;
    }
    // 如果 vector 的类型是 DOUBLE
    else if (vector->type == DOUBLE)
    {
        // 使用 Fisher-Yates 算法打乱数组
        for (int i = vector->size - 1; i > 0; i--)
        {
            // 生成一个 [0, i] 范围内的随机数
            int j = rand() % (i + 1);
            // 交换 d_array 数组中的第 i 和第 j 个元素
            double temp = vector->d_array[i];
            vector->d_array[i] = vector->d_array[j];
            vector->d_array[j] = temp;
        }
        // 返回打乱后的 vector
        return vector;
    }
}

/*
    以上是将数组转化为向量的函数，变换向量中数据类型的函数，打乱向量顺序的函数
    通过vector_converter调用
*/
/*******************************************分割线*******************************************/

/*******************************************分割线*******************************************/
/*
    以下是用于删除向量的函数
    通过vector_deleter调用
*/
// 删除向量
void delete_vector(Vector **vector)
{
    if ((*vector)->i_array != NULL) // 如果向量的int类型的数组不为空
    {
        free((*vector)->i_array);  // 释放这个数组的内存空间
        (*vector)->i_array = NULL; // 将这个数组设为NULL
    }
    if ((*vector)->f_array != NULL) // 如果向量的float类型的数组不为空
    {
        free((*vector)->f_array);  // 释放这个数组的内存空间
        (*vector)->f_array = NULL; // 将这个数组设为NULL
    }
    if ((*vector)->d_array != NULL) // 如果向量的double类型的数组不为空
    {
        free((*vector)->d_array);  // 释放这个数组的内存空间
        (*vector)->d_array = NULL; // 将这个数组设为NULL
    }
    free(*vector);  // 释放向量的内存空间
    *vector = NULL; // 将向量设为NULL
}
/*
    以上是用于删除向量的函数
    通过vector_deleter调用
*/
/*******************************************分割线*******************************************/

/*******************************************分割线*******************************************/
/*
    以下是用于向量计算的函数，包括向量加法、向量数乘、向量减法、向量点乘
    通过vector_calculator调用
*/
// 对多个向量进行加法运算
Vector *add_array(Vector *vectors, int count)
{
    for (int i = 1; i < count; i++) // 遍历向量数组
    {
        // 确保所有向量的大小相同
        assert((vectors[i]).size == (vectors[0]).size && "\nError: All vectors must have the same size.\n");
    }

    if ((vectors[0]).type == INT) // 如果向量的类型是int
    {
        Vector *result = generate_EmptyVector((vectors[0]).type, (vectors[0]).size); // 生成一个int类型的空向量
        for (int i = 0; i < (vectors[0]).size; i++)                                  // 遍历向量
        {
            result->i_array[i] = (vectors[0]).i_array[i]; // 将第一个向量的元素赋值给结果向量
            for (int j = 1; j < count; j++)               // 遍历其他向量
            {
                result->i_array[i] += (vectors[j]).i_array[i]; // 将其他向量的元素加到结果向量上
            }
        }
        return result; // 返回结果向量
    }
    else if ((vectors[0]).type == FLOAT) // 如果向量的类型是float
    {
        Vector *result = generate_EmptyVector((vectors[0]).type, (vectors[0]).size); // 生成一个float类型的空向量
        for (int i = 0; i < (vectors[0]).size; i++)                                  // 遍历向量
        {
            result->f_array[i] = (vectors[0]).f_array[i]; // 将第一个向量的元素赋值给结果向量
            for (int j = 1; j < count; j++)               // 遍历其他向量
            {
                result->f_array[i] += (vectors[j]).f_array[i]; // 将其他向量的元素加到结果向量上
            }
        }
        return result; // 返回结果向量
    }
    else if ((vectors[0]).type == DOUBLE) // 如果向量的类型是double
    {
        Vector *result = generate_EmptyVector((vectors[0]).type, (vectors[0]).size); // 生成一个double类型的空向量
        for (int i = 0; i < (vectors[0]).size; i++)                                  // 遍历向量
        {
            result->d_array[i] = (vectors[0]).d_array[i]; // 将第一个向量的元素赋值给结果向量
            for (int j = 1; j < count; j++)               // 遍历其他向量
            {
                result->d_array[i] += (vectors[j]).d_array[i]; // 将其他向量的元素加到结果向量上
            }
        }
        return result; // 返回结果向量
    }
    else
        // 如果向量的类型不是以上三种，报错
        assert(0 && "\nError: Unsupported type. The 'type' parameter must be DOUBLE, FLOAT, or INT.\n");
    return &(Vector){0}; // 返回一个空的Vector结构体
}

// 对多个向量进行加法运算，参数数量可变
Vector *add_variadic(int count, ...)
{
    va_list args;          // 定义一个va_list类型的变量
    va_start(args, count); // 初始化va_list

    Vector *vectors = (Vector *)malloc(sizeof(Vector) * count); // 分配内存空间给一个Vector类型的数组
    for (int i = 0; i < count; i++)                             // 遍历数组
    {
        (vectors[i]) = va_arg(args, Vector); // 获取可变参数列表中的参数
    }

    Vector *result = add_array(vectors, count); // 调用add_array函数对向量进行加法运算

    free(vectors); // 释放Vector类型的数组
    va_end(args);  // 结束获取可变参数列表

    return result; // 返回结果向量
}

// 对向量进行乘法运算
Vector *multiple_vector(Vector vector, double times)
{
    if (vector.type == INT) // 如果向量的类型是int
    {
        Vector *result = generate_EmptyVector(vector.type, vector.size); // 生成一个int类型的空向量
        for (int i = 0; i < vector.size; i++)                            // 遍历向量
        {
            result->i_array[i] = vector.i_array[i] * times; // 将向量的元素乘以一个数并赋值给结果向量
        }
        return result; // 返回结果向量
    }
    else if (vector.type == FLOAT) // 如果向量的类型是float
    {
        Vector *result = generate_EmptyVector(vector.type, vector.size); // 生成一个float类型的空向量
        for (int i = 0; i < vector.size; i++)                            // 遍历向量
        {
            result->f_array[i] = vector.f_array[i] * times; // 将向量的元素乘以一个数并赋值给结果向量
        }
        return result; // 返回结果向量
    }
    else if (vector.type == DOUBLE) // 如果向量的类型是double
    {
        Vector *result = generate_EmptyVector(vector.type, vector.size); // 生成一个double类型的空向量
        for (int i = 0; i < vector.size; i++)                            // 遍历向量
        {
            result->d_array[i] = vector.d_array[i] * times; // 将向量的元素乘以一个数并赋值给结果向量
        }
        return result; // 返回结果向量
    }
    else // 如果向量的类型不是以上三种
    {
        assert(0 && "\nError: Unsupported type. The 'type' parameter must be DOUBLE, FLOAT, or INT.\n"); // 报错
    }
}

// 对多个向量进行减法运算
Vector *subtract_array(Vector *vectors, int count)
{
    Vector *vectors_changed = (Vector *)malloc(count * sizeof(Vector)); // 分配内存空间给一个Vector类型的数组
    vectors_changed[0] = vectors[0];                                    // 将第一个向量赋值给新的数组
    for (int i = 1; i < count; i++)                                     // 遍历其他向量
    {
        Vector *temp = multiple_vector(vectors[i], -1); // 将向量乘以-1
        vectors_changed[i] = *temp;                     // 将结果赋值给新的数组
        free(temp);                                     // 释放临时变量
    }
    Vector *result = add_array(vectors_changed, count); // 调用add_array函数对新的数组进行加法运算
    free(vectors_changed);                              // 释放Vector类型的数组
    return result;                                      // 返回结果向量
}

// 对多个向量进行减法运算，参数数量可变
Vector *subtract_variadic(int count, ...)
{
    va_list args;          // 定义一个va_list类型的变量
    va_start(args, count); // 初始化va_list

    Vector *vectors = (Vector *)malloc(sizeof(Vector) * count); // 分配内存空间给一个Vector类型的数组
    for (int i = 0; i < count; i++)                             // 遍历数组
    {
        vectors[i] = va_arg(args, Vector); // 获取可变参数列表中的参数
    }

    Vector *result = subtract_array(vectors, count); // 调用subtract_array函数对向量进行减法运算

    free(vectors); // 释放Vector类型的数组
    va_end(args);  // 结束获取可变参数列表

    return result; // 返回结果向量
}

// 计算两个向量的点乘
double dot_product(Vector v1, Vector v2)
{
    assert(v1.size == v2.size && "\nError: Vectors must have the same size.\n"); // 确保两个向量的大小相同
    assert(v1.type == v2.type && "\nError: Vectors must have the same type.\n"); // 确保两个向量的类型相同

    double result = 0;  // 初始化结果为0
    if (v1.type == INT) // 如果向量的类型是int
    {
        for (int i = 0; i < v1.size; i++) // 遍历向量
        {
            result += v1.i_array[i] * v2.i_array[i]; // 计算两个向量的点乘
        }
    }
    else if (v1.type == FLOAT) // 如果向量的类型是float
    {
        for (int i = 0; i < v1.size; i++) // 遍历向量
        {
            result += v1.f_array[i] * v2.f_array[i]; // 计算两个向量的点乘
        }
    }
    else if (v1.type == DOUBLE) // 如果向量的类型是double
    {
        for (int i = 0; i < v1.size; i++) // 遍历向量
        {
            result += v1.d_array[i] * v2.d_array[i]; // 计算两个向量的点乘
        }
    }
    else // 如果向量的类型不是以上三种
    {
        assert(0 && "\nError: Unsupported type. The 'type' parameter must be DOUBLE, FLOAT, or INT.\n"); // 报错
    }

    return result; // 返回结果
}
// 计算向量的均值
double mean(Vector vector)
{
    double sum = 0.0;
    for (int i = 0; i < vector.size; ++i)
    {
        if (vector.type == INT)
            sum += vector.i_array[i];
        else if (vector.type == FLOAT)
            sum += vector.f_array[i];
        else if (vector.type == DOUBLE)
            sum += vector.d_array[i];
    }
    return sum / vector.size;
}

// 计算向量的方差
double variance(Vector vector)
{
    // 返回一个向量的方差，即平方的期望减去期望的平方
    return dot_product(vector, vector) / vector.size - pow(mean(vector), 2);
}

// 计算向量的标准差
double standard(Vector vector)
{
    return sqrt(variance(vector));
}
// 计算两个变量的协方差
double covariance(Vector v1, Vector v2)
{
    // 使用断言确保两个向量的大小相同
    assert(v1.size == v2.size && "\nError: Vectors must have the same size.\n");
    // 使用断言确保两个向量的类型相同
    assert(v1.type == v2.type && "\nError: Vectors must have the same type.\n");

    // 返回两个事件的协方差，即两个事件的联合概率的期望减去两个事件边缘概率的期望的乘积
    return dot_product(v1, v2) / v1.size - mean(v1) * mean(v2);
}
// 计算两个变量的相关系数
double correlation(Vector v1, Vector v2)
{
    // 使用断言确保两个向量的大小相同
    assert(v1.size == v2.size && "\nError: Vectors must have the same size.\n");
    // 使用断言确保两个向量的类型相同
    assert(v1.type == v2.type && "\nError: Vectors must have the same type.\n");
    // 返回两个变量的相关系数，即协方差除以两个变量的标准差的乘积
    return covariance(v1, v2) / (standard(v1) * standard(v2));
} // 这个函数对一个Vector进行标准化处理
Vector *StandardScaler(Vector *vector)
{
    // 计算Vector的均值
    double mean_value = mean(*vector);
    // 计算Vector的标准差
    double std_value = standard(*vector);
    // 根据Vector的类型，对Vector的每个元素进行标准化处理
    if (vector->type == INT)
        for (int i = 0; i < vector->size; i++)
            vector->i_array[i] = (vector->i_array[i] - mean_value) / std_value;
    else if (vector->type == FLOAT)
        for (int i = 0; i < vector->size; i++)
            vector->f_array[i] = (vector->f_array[i] - mean_value) / std_value;
    else if (vector->type == DOUBLE)
        for (int i = 0; i < vector->size; i++)
            vector->d_array[i] = (vector->d_array[i] - mean_value) / std_value;
    // 返回处理后的Vector
    return vector;
}

// 这个函数对一个Vector进行归一化处理
Vector *MinMaxScaler(Vector *vector)
{
    // 找出Vector的最大值
    double max_value = find_max(*vector);
    // 找出Vector的最小值
    double min_value = find_min(*vector);
    // 根据Vector的类型，对Vector的每个元素进行归一化处理
    if (vector->type == INT)
        for (int i = 0; i < vector->size; i++)
            vector->i_array[i] = (vector->i_array[i] - min_value) / (max_value - min_value);
    else if (vector->type == FLOAT)
        for (int i = 0; i < vector->size; i++)
            vector->f_array[i] = (vector->f_array[i] - min_value) / (max_value - min_value);
    else if (vector->type == DOUBLE)
        for (int i = 0; i < vector->size; i++)
            vector->d_array[i] = (vector->d_array[i] - min_value) / (max_value - min_value);
    // 返回处理后的Vector
    return vector;
}

// 这个函数计算一个Vector的LP范数
double lp_norm(Vector vector, int p)
{
    // 初始化范数为0
    double norm = 0;

    // 根据Vector的类型，计算Vector的LP范数
    if (vector.type == DOUBLE)
    {
        for (int i = 0; i < vector.size; i++)
            norm += pow(fabs(vector.d_array[i]), p);
    }
    else if (vector.type == FLOAT)
    {
        for (int i = 0; i < vector.size; i++)
            norm += pow(fabs(vector.f_array[i]), p);
    }
    else if (vector.type == INT)
    {
        for (int i = 0; i < vector.size; i++)
            norm += pow(fabs(vector.i_array[i]), p);
    }

    // 返回Vector的LP范数
    return pow(norm, 1.0 / p);
}

/*
    以上是用于向量计算的函数，包括向量加法、向量数乘、向量减法、向量点乘
    通过vector_calculator调用
*/
/*******************************************分割线*******************************************/

/*******************************************分割线*******************************************/
/*
    以下是用于向量缩放的函数，包括向量切片、向量扩展、高级索引
    通过vector_resizer调用
*/
// 对向量进行切片操作
Vector *slice(Vector vector, int start, int end, int step)
{
    if (end < 0)                                                                         // 如果结束索引是负数
        end += vector.size;                                                              // 将其转化为正数
    if (start < 0)                                                                       // 如果开始索引是负数
        start += vector.size;                                                            // 将其转化为正数
    assert(start >= 0 && start < vector.size && "\nError: Start index out of range.\n"); // 确保开始索引在范围内
    assert(end >= 0 && end <= vector.size && "\nError: End index out of range.\n");      // 确保结束索引在范围内
    assert(end != start && "\nError: 'end' and 'start' cannot be the same.\n");          // 确保开始索引和结束索引不相同
    assert(step != 0 && "\nError: Step cannot be zero.\n");                              // 确保步长不为0
    if (start < end)                                                                     // 如果开始索引小于结束索引
    {
        // 确保步长为正数
        assert(step > 0 && "\nError: When 'start' is less than 'end', 'step' must be positive.\n");
        if (vector.type == INT) // 如果向量的类型是int
        {
            // 生成一个int类型的空向量
            Vector *result = generate_EmptyVector(vector.type, (int)((end - start - 1) / step) + 1);
            for (int i = start, j = 0; i < end; i += step, j++) // 遍历向量
                result->i_array[j] = vector.i_array[i];         // 将向量的元素赋值给结果向量
            return result;                                      // 返回结果向量
        }
        else if (vector.type == FLOAT) // 如果向量的类型是float
        {
            // 生成一个float类型的空向量
            Vector *result = generate_EmptyVector(vector.type, (int)((end - start - 1) / step) + 1);
            for (int i = start, j = 0; i < end; i += step, j++) // 遍历向量
                result->f_array[j] = vector.f_array[i];         // 将向量的元素赋值给结果向量
            return result;                                      // 返回结果向量
        }
        else if (vector.type == DOUBLE) // 如果向量的类型是double
        {
            // 生成一个double类型的空向量
            Vector *result = generate_EmptyVector(vector.type, (int)((end - start - 1) / step) + 1);
            for (int i = start, j = 0; i < end; i += step, j++) // 遍历向量
                result->d_array[j] = vector.d_array[i];         // 将向量的元素赋值给结果向量
            return result;                                      // 返回结果向量
        }
        else
            // 如果向量的类型不是以上三种，报错
            assert(0 && "\nError: Unsupported type. The 'type' parameter must be DOUBLE, FLOAT, or INT.\n");
    }
    else // 如果开始索引大于结束索引
    {
        // 确保步长为负数
        assert(step < 0 && "\nError: When 'start' is greater than 'end', 'step' must be negative.\n");
        if (vector.type == INT) // 如果向量的类型是int
        {
            // 生成一个int类型的空向量
            Vector *result = generate_EmptyVector(vector.type, (int)((start - end - 1) / (-step)) + 1);
            for (int i = start, j = 0; i > end; i += step, j++) // 遍历向量
                result->i_array[j] = vector.i_array[i];         // 将向量的元素赋值给结果向量
            return result;                                      // 返回结果向量
        }
        else if (vector.type == FLOAT) // 如果向量的类型是float
        {
            // 生成一个float类型的空向量
            Vector *result = generate_EmptyVector(vector.type, (int)((start - end - 1) / (-step)) + 1);
            for (int i = start, j = 0; i > end; i += step, j++) // 遍历向量
                result->f_array[j] = vector.f_array[i];         // 将向量的元素赋值给结果向量
            return result;                                      // 返回结果向量
        }
        else if (vector.type == DOUBLE) // 如果向量的类型是double
        {
            // 生成一个double类型的空向量
            Vector *result = generate_EmptyVector(vector.type, (int)((start - end - 1) / (-step)) + 1);
            for (int i = start, j = 0; i > end; i += step, j++) // 遍历向量
                result->d_array[j] = vector.d_array[i];         // 将向量的元素赋值给结果向量
            return result;                                      // 返回结果向量
        }
        else
            // 如果向量的类型不是以上三种，报错
            assert(0 && "\nError: Unsupported type. The 'type' parameter must be DOUBLE, FLOAT, or INT.\n");
    }
}

// 将两个向量进行扩展操作
Vector *extend(Vector v1, Vector v2)
{
    assert(v1.type == v2.type && "\nThe type of two vectors must be the same.\n"); // 确保两个向量的类型相同
    Vector *result = (Vector *)malloc(sizeof(Vector));                             // 分配内存空间给一个Vector结构体
    result->size = v1.size + v2.size;                                              // 设置结果向量的大小
    result->type = v1.type;                                                        // 设置结果向量的类型
    if (v1.type == INT)                                                            // 如果向量的类型是int
    {
        result->i_array = (int *)malloc(sizeof(int) * result->size); // 分配内存空间给一个int类型的数组
        for (int i = 0; i < v1.size; ++i)                            // 遍历第一个向量
            result->i_array[i] = v1.i_array[i];                      // 将第一个向量的元素赋值给结果向量
        for (int i = 0; i < v2.size; ++i)                            // 遍历第二个向量
            result->i_array[i + v1.size] = v2.i_array[i];            // 将第二个向量的元素赋值给结果向量
    }
    else if (v1.type == FLOAT) // 如果向量的类型是float
    {
        result->f_array = (float *)malloc(sizeof(float) * result->size); // 分配内存空间给一个float类型的数组
        for (int i = 0; i < v1.size; ++i)                                // 遍历第一个向量
            result->f_array[i] = v1.f_array[i];                          // 将第一个向量的元素赋值给结果向量
        for (int i = 0; i < v2.size; ++i)                                // 遍历第二个向量
            result->f_array[i + v1.size] = v2.f_array[i];                // 将第二个向量的元素赋值给结果向量
    }
    else if (v1.type == DOUBLE) // 如果向量的类型是double
    {
        result->d_array = (double *)malloc(sizeof(double) * result->size); // 分配内存空间给一个double类型的数组
        for (int i = 0; i < v1.size; ++i)                                  // 遍历第一个向量
            result->d_array[i] = v1.d_array[i];                            // 将第一个向量的元素赋值给结果向量
        for (int i = 0; i < v2.size; ++i)                                  // 遍历第二个向量
            result->d_array[i + v1.size] = v2.d_array[i];                  // 将第二个向量的元素赋值给结果向量
    }
    return result; // 返回结果向量
}
// fancy_indexing函数：根据给定的索引从数据向量中取出元素
Vector *fancy_indexing(Vector data, Vector indices)
{
    // 检查索引是否为整数类型
    if (indices.type != INT)
    {
        // 如果索引不是整数类型，那么报错并返回NULL
        assert(0 && "Error: indices must be of type INT.\n");
        return NULL;
    }

    // 创建一个新的Vector来存储结果，其类型与数据向量相同，大小与索引向量相同
    Vector *result = generate_EmptyVector(data.type, indices.size);

    // 根据索引从data中取出元素
    for (int i = 0; i < indices.size; i++)
    {
        int index = indices.i_array[i];
        // 检查索引是否在数据向量的范围内
        if (index < 0 || index >= data.size)
        {
            // 如果索引超出范围，那么释放已经分配的内存，并报错返回NULL
            if (result->type == DOUBLE)
                free(result->d_array);
            else if (result->type == FLOAT)
                free(result->f_array);
            else if (result->type == INT)
                free(result->i_array);
            free(result);
            assert(0 && "Error: index out of range.\n");
            return NULL;
        }
        // 根据数据向量的类型，从数据向量中取出元素，存入结果向量
        if (data.type == DOUBLE)
            result->d_array[i] = data.d_array[index];
        else if (data.type == FLOAT)
            result->f_array[i] = data.f_array[index];
        else if (data.type == INT)
            result->i_array[i] = data.i_array[index];
    }

    // 返回结果向量
    return result;
}
// boolean_indexing函数：根据给定的掩码从数据向量中取出元素
Vector *boolean_indexing(Vector data, Vector mask)
{
    // 检查mask是否为整数类型
    if (mask.type != INT)
    {
        // 如果mask不是整数类型，那么报错并返回NULL
        assert(0 && "Error: mask must be of type INT.\n");
        return NULL;
    }

    // 计算mask中True的数量
    int count = 0;
    for (int i = 0; i < mask.size; i++)
        if (mask.i_array[i])
            count++;

    // 创建一个新的Vector来存储结果，其类型与数据向量相同，大小为mask中True的数量
    Vector *result = generate_EmptyVector(data.type, count);

    // 根据mask从data中取出元素
    int j = 0;
    for (int i = 0; i < mask.size; i++)
    {
        // 如果mask的当前元素为True，那么从数据向量中取出对应的元素，存入结果向量
        if (mask.i_array[i])
        {
            if (data.type == DOUBLE)
                result->d_array[j] = data.d_array[i];
            else if (data.type == FLOAT)
                result->f_array[j] = data.f_array[i];
            else if (data.type == INT)
                result->i_array[j] = data.i_array[i];
            j++;
        }
    }

    // 返回结果向量
    return result;
}

/*
    以上是用于向量缩放的函数，包括向量切片、向量扩展、高级索引
    通过vector_resizer调用
*/
/*******************************************分割线*******************************************/

/*******************************************分割线*******************************************/
/*
    以下是涉及到比较的函数
    通过vector_comparator调用
*/
void merge(Vector *vector, int left, int mid, int right)
{
    int i, j, k;
    int sizeLeft = mid - left + 1;
    int sizeRight = right - mid;

    if (vector->type == DOUBLE)
    {
        double Left[sizeLeft], Right[sizeRight];
        for (i = 0; i < sizeLeft; i++)
            Left[i] = vector->d_array[left + i];
        for (j = 0; j < sizeRight; j++)
            Right[j] = vector->d_array[mid + 1 + j];

        i = 0;
        j = 0;
        k = left;
        while (i < sizeLeft && j < sizeRight)
        {
            if (Left[i] <= Right[j])
            {
                vector->d_array[k] = Left[i];
                i++;
            }
            else
            {
                vector->d_array[k] = Right[j];
                j++;
            }
            k++;
        }

        while (i < sizeLeft)
        {
            vector->d_array[k] = Left[i];
            i++;
            k++;
        }

        while (j < sizeRight)
        {
            vector->d_array[k] = Right[j];
            j++;
            k++;
        }
    }
    else if (vector->type == FLOAT)
    {
        double Left[sizeLeft], Right[sizeRight];
        for (i = 0; i < sizeLeft; i++)
            Left[i] = vector->f_array[left + i];
        for (j = 0; j < sizeRight; j++)
            Right[j] = vector->f_array[mid + 1 + j];

        i = 0;
        j = 0;
        k = left;
        while (i < sizeLeft && j < sizeRight)
        {
            if (Left[i] <= Right[j])
            {
                vector->f_array[k] = Left[i];
                i++;
            }
            else
            {
                vector->f_array[k] = Right[j];
                j++;
            }
            k++;
        }

        while (i < sizeLeft)
        {
            vector->f_array[k] = Left[i];
            i++;
            k++;
        }

        while (j < sizeRight)
        {
            vector->f_array[k] = Right[j];
            j++;
            k++;
        }
    }
    else if (vector->type == INT)
    {
        double Left[sizeLeft], Right[sizeRight];
        for (i = 0; i < sizeLeft; i++)
            Left[i] = vector->i_array[left + i];
        for (j = 0; j < sizeRight; j++)
            Right[j] = vector->i_array[mid + 1 + j];

        i = 0;
        j = 0;
        k = left;
        while (i < sizeLeft && j < sizeRight)
        {
            if (Left[i] <= Right[j])
            {
                vector->i_array[k] = Left[i];
                i++;
            }
            else
            {
                vector->i_array[k] = Right[j];
                j++;
            }
            k++;
        }

        while (i < sizeLeft)
        {
            vector->i_array[k] = Left[i];
            i++;
            k++;
        }

        while (j < sizeRight)
        {
            vector->i_array[k] = Right[j];
            j++;
            k++;
        }
    }
}

void mergeSort(Vector *vector, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        mergeSort(vector, left, mid);
        mergeSort(vector, mid + 1, right);

        merge(vector, left, mid, right);
    }
}

Vector *sort(Vector vector)
{
    Vector *new_vector = generate_EmptyVector(vector.type, vector.size);

    if (vector.type == DOUBLE)
    {
        for (int i = 0; i < vector.size; i++)
            new_vector->d_array[i] = vector.d_array[i];
        mergeSort(new_vector, 0, vector.size - 1);
    }
    else if (vector.type == FLOAT)
    {
        for (int i = 0; i < vector.size; i++)
            new_vector->f_array[i] = (double)vector.f_array[i];
        mergeSort(new_vector, 0, vector.size - 1);
    }
    else if (vector.type == INT)
    {
        for (int i = 0; i < vector.size; i++)
            new_vector->i_array[i] = (double)vector.i_array[i];
        mergeSort(new_vector, 0, vector.size - 1);
    }
    return new_vector;
}
void mergeArgsort(Vector *vector, int *indices, int left, int mid, int right)
{
    int i, j, k;
    int sizeLeft = mid - left + 1;
    int sizeRight = right - mid;

    double Left[sizeLeft], Right[sizeRight];
    int indicesLeft[sizeLeft], indicesRight[sizeRight];

    if (vector->type == DOUBLE)
    {
        for (i = 0; i < sizeLeft; i++)
        {
            Left[i] = vector->d_array[left + i];
            indicesLeft[i] = indices[left + i];
        }
        for (j = 0; j < sizeRight; j++)
        {
            Right[j] = vector->d_array[mid + 1 + j];
            indicesRight[j] = indices[mid + 1 + j];
        }
    }
    else if (vector->type == FLOAT)
    {
        for (i = 0; i < sizeLeft; i++)
        {
            Left[i] = vector->f_array[left + i];
            indicesLeft[i] = indices[left + i];
        }
        for (j = 0; j < sizeRight; j++)
        {
            Right[j] = vector->f_array[mid + 1 + j];
            indicesRight[j] = indices[mid + 1 + j];
        }
    }
    else if (vector->type == INT)
    {
        for (i = 0; i < sizeLeft; i++)
        {
            Left[i] = vector->i_array[left + i];
            indicesLeft[i] = indices[left + i];
        }
        for (j = 0; j < sizeRight; j++)
        {
            Right[j] = vector->i_array[mid + 1 + j];
            indicesRight[j] = indices[mid + 1 + j];
        }
    }

    i = 0;
    j = 0;
    k = left;
    while (i < sizeLeft && j < sizeRight)
    {
        if (Left[i] <= Right[j])
        {
            if (vector->type == DOUBLE)
                vector->d_array[k] = Left[i];
            else if (vector->type == FLOAT)
                vector->f_array[k] = Left[i];
            else if (vector->type == INT)
                vector->i_array[k] = Left[i];
            indices[k] = indicesLeft[i];
            i++;
        }
        else
        {
            if (vector->type == DOUBLE)
                vector->d_array[k] = Right[j];
            else if (vector->type == FLOAT)
                vector->f_array[k] = Right[j];
            else if (vector->type == INT)
                vector->i_array[k] = Right[j];
            indices[k] = indicesRight[j];
            j++;
        }
        k++;
    }

    while (i < sizeLeft)
    {
        if (vector->type == DOUBLE)
            vector->d_array[k] = Left[i];
        else if (vector->type == FLOAT)
            vector->f_array[k] = Left[i];
        else if (vector->type == INT)
            vector->i_array[k] = Left[i];
        indices[k] = indicesLeft[i];
        i++;
        k++;
    }

    while (j < sizeRight)
    {
        if (vector->type == DOUBLE)
            vector->d_array[k] = Right[j];
        else if (vector->type == FLOAT)
            vector->f_array[k] = Right[j];
        else if (vector->type == INT)
            vector->i_array[k] = Right[j];
        indices[k] = indicesRight[j];
        j++;
        k++;
    }
}

void mergeSortArgsort(Vector *vector, int *indices, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        mergeSortArgsort(vector, indices, left, mid);
        mergeSortArgsort(vector, indices, mid + 1, right);

        mergeArgsort(vector, indices, left, mid, right);
    }
}

Vector *argsort(Vector vector)
{
    Vector *new_vector = generate_EmptyVector(vector.type, vector.size);
    int *indices = (int *)malloc(vector.size * sizeof(int));

    if (vector.type == DOUBLE)
    {
        for (int i = 0; i < vector.size; i++)
        {
            new_vector->d_array[i] = vector.d_array[i];
            indices[i] = i;
        }
    }
    else if (vector.type == FLOAT)
    {
        for (int i = 0; i < vector.size; i++)
        {
            new_vector->f_array[i] = vector.f_array[i];
            indices[i] = i;
        }
    }
    else if (vector.type == INT)
    {
        for (int i = 0; i < vector.size; i++)
        {
            new_vector->i_array[i] = vector.i_array[i];
            indices[i] = i;
        }
    }

    mergeSortArgsort(new_vector, indices, 0, vector.size - 1);

    free(new_vector);
    Vector *result = copy_IntArray_to_NewVector(indices, vector.size);
    free(indices);
    return result;
}
double find_max(Vector vector)
{
    double max_value;

    if (vector.type == DOUBLE)
    {
        max_value = vector.d_array[0];
        for (int i = 1; i < vector.size; i++)
            if (vector.d_array[i] > max_value)
                max_value = vector.d_array[i];
    }
    else if (vector.type == FLOAT)
    {
        max_value = vector.f_array[0];
        for (int i = 1; i < vector.size; i++)
            if (vector.f_array[i] > max_value)
                max_value = vector.f_array[i];
    }
    else if (vector.type == INT)
    {
        max_value = vector.i_array[0];
        for (int i = 1; i < vector.size; i++)
            if (vector.i_array[i] > max_value)
                max_value = vector.i_array[i];
    }

    return max_value;
}

int argmax(Vector vector)
{
    double max_value;
    int max_index;

    if (vector.type == DOUBLE)
    {
        max_value = vector.d_array[0];
        max_index = 0;
        for (int i = 1; i < vector.size; i++)
            if (vector.d_array[i] > max_value)
            {
                max_value = vector.d_array[i];
                max_index = i;
            }
    }
    else if (vector.type == FLOAT)
    {
        max_value = vector.f_array[0];
        max_index = 0;
        for (int i = 1; i < vector.size; i++)
            if (vector.f_array[i] > max_value)
            {
                max_value = vector.f_array[i];
                max_index = i;
            }
    }
    else if (vector.type == INT)
    {
        max_value = vector.i_array[0];
        max_index = 0;
        for (int i = 1; i < vector.size; i++)
            if (vector.i_array[i] > max_value)
            {
                max_value = vector.i_array[i];
                max_index = i;
            }
    }

    return max_index;
}
double find_min(Vector vector)
{
    double min_value;

    if (vector.type == DOUBLE)
    {
        min_value = vector.d_array[0];
        for (int i = 1; i < vector.size; i++)
            if (vector.d_array[i] < min_value)
                min_value = vector.d_array[i];
    }
    else if (vector.type == FLOAT)
    {
        min_value = vector.f_array[0];
        for (int i = 1; i < vector.size; i++)
            if (vector.f_array[i] < min_value)
                min_value = vector.f_array[i];
    }
    else if (vector.type == INT)
    {
        min_value = vector.i_array[0];
        for (int i = 1; i < vector.size; i++)
            if (vector.i_array[i] < min_value)
                min_value = vector.i_array[i];
    }

    return min_value;
}

int argmin(Vector vector)
{
    double min_value;
    int min_index;

    if (vector.type == DOUBLE)
    {
        min_value = vector.d_array[0];
        min_index = 0;
        for (int i = 1; i < vector.size; i++)
            if (vector.d_array[i] < min_value)
            {
                min_value = vector.d_array[i];
                min_index = i;
            }
    }
    else if (vector.type == FLOAT)
    {
        min_value = vector.f_array[0];
        min_index = 0;
        for (int i = 1; i < vector.size; i++)
            if (vector.f_array[i] < min_value)
            {
                min_value = vector.f_array[i];
                min_index = i;
            }
    }
    else if (vector.type == INT)
    {
        min_value = vector.i_array[0];
        min_index = 0;
        for (int i = 1; i < vector.size; i++)
            if (vector.i_array[i] < min_value)
            {
                min_value = vector.i_array[i];
                min_index = i;
            }
    }

    return min_index;
}
// 定义一个函数，接受一个Vector类型的参数，一个字符指针，和一个double类型的值
Vector *get_BooleanVector_for_condition(Vector vector, char *condition, double value)
{
    // 生成一个空的Vector，类型为INT，大小和输入的vector一样
    Vector *result = generate_EmptyVector(INT, vector.size);

    // 如果输入的vector的类型是INT
    if (vector.type == INT)
    {
        // 如果条件是小于
        if (condition[0] == '<')
        {
            // 如果条件是小于等于
            if (condition[1] == '=')
                // 遍历vector中的每个元素
                for (int i = 0; i < vector.size; i++)
                    // 如果元素小于等于给定的值，结果数组对应位置为1，否则为0
                    result->i_array[i] = (vector.i_array[i] <= value) ? 1 : 0;
            else
                // 遍历vector中的每个元素
                for (int i = 0; i < vector.size; i++)
                    // 如果元素小于给定的值，结果数组对应位置为1，否则为0
                    result->i_array[i] = (vector.i_array[i] < value) ? 1 : 0;
        }
        // 如果条件是大于
        else if (condition[0] == '>')
        {
            // 如果条件是大于等于
            if (condition[1] == '=')
                // 遍历vector中的每个元素
                for (int i = 0; i < vector.size; i++)
                    // 如果元素大于等于给定的值，结果数组对应位置为1，否则为0
                    result->i_array[i] = (vector.i_array[i] >= value) ? 1 : 0;
            else
                // 遍历vector中的每个元素
                for (int i = 0; i < vector.size; i++)
                    // 如果元素大于给定的值，结果数组对应位置为1，否则为0
                    result->i_array[i] = (vector.i_array[i] > value) ? 1 : 0;
        }
        // 如果条件是等于
        else if (condition[0] == '=')
            // 遍历vector中的每个元素
            for (int i = 0; i < vector.size; i++)
                // 如果元素等于给定的值，结果数组对应位置为1，否则为0
                result->i_array[i] = (vector.i_array[i] == value) ? 1 : 0;
    }
    // 如果输入的vector的类型是FLOAT
    else if (vector.type == FLOAT)
    {
        // 如果条件是小于
        if (condition[0] == '<')
        {
            // 如果条件是小于等于
            if (condition[1] == '=')
                // 遍历vector中的每个元素
                for (int i = 0; i < vector.size; i++)
                    // 如果元素小于等于给定的值，结果数组对应位置为1，否则为0
                    result->i_array[i] = (vector.f_array[i] <= value) ? 1 : 0;
            else
                // 遍历vector中的每个元素
                for (int i = 0; i < vector.size; i++)
                    // 如果元素小于给定的值，结果数组对应位置为1，否则为0
                    result->i_array[i] = (vector.f_array[i] < value) ? 1 : 0;
        }
        // 如果条件是大于
        else if (condition[0] == '>')
        {
            // 如果条件是大于等于
            if (condition[1] == '=')
                // 遍历vector中的每个元素
                for (int i = 0; i < vector.size; i++)
                    // 如果元素大于等于给定的值，结果数组对应位置为1，否则为0
                    result->i_array[i] = (vector.f_array[i] >= value) ? 1 : 0;
            else
                // 遍历vector中的每个元素
                for (int i = 0; i < vector.size; i++)
                    // 如果元素大于给定的值，结果数组对应位置为1，否则为0
                    result->i_array[i] = (vector.f_array[i] > value) ? 1 : 0;
        }
        // 如果条件是等于
        else if (condition[0] == '=')
            // 遍历vector中的每个元素
            for (int i = 0; i < vector.size; i++)
                // 如果元素等于给定的值，结果数组对应位置为1，否则为0
                result->i_array[i] = (vector.f_array[i] == value) ? 1 : 0;
    }
    // 如果输入的vector的类型是DOUBLE
    else if (vector.type == DOUBLE)
    {
        // 如果条件是小于
        if (condition[0] == '<')
        {
            // 如果条件是小于等于
            if (condition[1] == '=')
                // 遍历vector中的每个元素
                for (int i = 0; i < vector.size; i++)
                    // 如果元素小于等于给定的值，结果数组对应位置为1，否则为0
                    result->i_array[i] = (vector.d_array[i] <= value) ? 1 : 0;
            else
                // 遍历vector中的每个元素
                for (int i = 0; i < vector.size; i++)
                    // 如果元素小于给定的值，结果数组对应位置为1，否则为0
                    result->i_array[i] = (vector.d_array[i] < value) ? 1 : 0;
        }
        // 如果条件是大于
        else if (condition[0] == '>')
        {
            // 如果条件是大于等于
            if (condition[1] == '=')
                // 遍历vector中的每个元素
                for (int i = 0; i < vector.size; i++)
                    // 如果元素大于等于给定的值，结果数组对应位置为1，否则为0
                    result->i_array[i] = (vector.d_array[i] >= value) ? 1 : 0;
            else
                // 遍历vector中的每个元素
                for (int i = 0; i < vector.size; i++)
                    // 如果元素大于给定的值，结果数组对应位置为1，否则为0
                    result->i_array[i] = (vector.d_array[i] > value) ? 1 : 0;
        }
        // 如果条件是等于
        else if (condition[0] == '=')
            // 遍历vector中的每个元素
            for (int i = 0; i < vector.size; i++)
                // 如果元素等于给定的值，结果数组对应位置为1，否则为0
                result->i_array[i] = (vector.d_array[i] == value) ? 1 : 0;
    }
    // 返回结果数组
    return result;
}
/*
    以上是涉及到比较的函数
    通过vector_comparator调用
*/
/*******************************************分割线*******************************************/

/*******************************************分割线*******************************************/
/*
    以下是用于初始化VectorOperations结构体的函数
    只需要调用init_VectorOperations函数即可
    该函数会初始化VectorOperations结构体中的所有函数指针
*/
void init_VectorGenerator(VectorGenerator *vector_generator)
{
    vector_generator->generate_EmptyVector = generate_EmptyVector;
    vector_generator->generate_ZerosVector = generate_ZerosVector;
    vector_generator->generate_OnesVector = generate_OnesVector;
    vector_generator->generate_RandomVector = generate_RandomVector;
    vector_generator->generate_RangeVector = generate_RangeVector;
    vector_generator->generate_NormalVector = generate_NormalVector;
}
void init_VectorDeleter(VectorDeleter *vector_deleter)
{
    vector_deleter->delete_vector = delete_vector;
}
void init_VectorConverter(VectorConverter *vector_converter)
{
    vector_converter->convert_DoubleArray_to_NewVector = convert_DoubleArray_to_NewVector;
    vector_converter->convert_FloatArray_to_NewVector = convert_FloatArray_to_NewVector;
    vector_converter->convert_IntArray_to_NewVector = convert_IntArray_to_NewVector;
    vector_converter->convert_DoubleArray_to_ExistingVector = convert_DoubleArray_to_ExistingVector;
    vector_converter->convert_FloatArray_to_ExistingVector = convert_FloatArray_to_ExistingVector;
    vector_converter->convert_IntArray_to_ExistingVector = convert_IntArray_to_ExistingVector;
    vector_converter->toInt = toInt;
    vector_converter->toFloat = toFloat;
    vector_converter->toDouble = toDouble;
    vector_converter->shuffle = shuffle;
};
void init_VectorCopyer(VectorCopyer *vector_copyer)
{
    vector_copyer->copy_DoubleArray_to_NewVector = copy_DoubleArray_to_NewVector;
    vector_copyer->copy_FloatArray_to_NewVector = copy_FloatArray_to_NewVector;
    vector_copyer->copy_IntArray_to_NewVector = copy_IntArray_to_NewVector;
    vector_copyer->copy_DoubleArray_to_ExistingVector = copy_DoubleArray_to_ExistingVector;
    vector_copyer->copy_FloatArray_to_ExistingVector = copy_FloatArray_to_ExistingVector;
    vector_copyer->copy_IntArray_to_ExistingVector = copy_IntArray_to_ExistingVector;
    vector_copyer->copy_Vector_to_NewVector = copy_Vector_to_NewVector;
    vector_copyer->copy_Vector_to_ExistingVector = copy_Vector_to_ExistingVector;
}
void init_VectorCalculator(VectorCalculator *vector_calculator)
{
    vector_calculator->add_array = add_array;
    vector_calculator->add_variadic = add_variadic;
    vector_calculator->multiple_vector = multiple_vector;
    vector_calculator->subtract_array = subtract_array;
    vector_calculator->subtract_variadic = subtract_variadic;
    vector_calculator->dot_product = dot_product;
    vector_calculator->mean = mean;
    vector_calculator->variance = variance;
    vector_calculator->standard = standard;
    vector_calculator->covariance = covariance;
    vector_calculator->correlation = correlation;
    vector_calculator->StandardScaler = StandardScaler;
    vector_calculator->MinMaxScaler = MinMaxScaler;
    vector_calculator->lp_norm = lp_norm;
}

void init_VectorResizer(VectorResizer *vector_resizer)
{
    vector_resizer->slice = slice;
    vector_resizer->extend = extend;
    vector_resizer->fancy_indexing = fancy_indexing;
    vector_resizer->boolean_indexing = boolean_indexing;
}

void init_VectorComparator(VectorComparator *vector_comparator)
{
    vector_comparator->sort = sort;
    vector_comparator->argsort = argsort;
    vector_comparator->find_max = find_max;
    vector_comparator->argmax = argmax;
    vector_comparator->find_min = find_min;
    vector_comparator->argmin = argmin;
    vector_comparator->get_BooleanVector_for_condition = get_BooleanVector_for_condition;
}

VectorOperations *init_VectorOperations()
{
    VectorOperations *vector_operations = (VectorOperations *)malloc(sizeof(VectorOperations));
    init_VectorGenerator(&vector_operations->vector_generator);
    init_VectorDeleter(&vector_operations->vector_deleter);
    init_VectorConverter(&vector_operations->vector_converter);
    init_VectorCopyer(&vector_operations->vector_copyer);
    init_VectorCalculator(&vector_operations->vector_calculator);
    init_VectorResizer(&vector_operations->vector_resizer);
    init_VectorComparator(&vector_operations->vector_comparator);
    return vector_operations;
}
/*******************************************分割线*******************************************/

/*******************************************分割线*******************************************/
// 删除VectorOperations结构体
void delete_VectorOperations(VectorOperations **vector_operations)
{
    free(*vector_operations);
    *vector_operations = NULL;
}