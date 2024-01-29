#ifndef VECTOR_H
#define VECTOR_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef TYPE_FLAG
#define TYPE_FLAG
#define DOUBLE 0
#define FLOAT 1
#define INT 2
#endif

    typedef struct Vector Vector;                     // 规定了向量的属性
    typedef struct VectorGenerator VectorGenerator;   // 生成器，用于生成特定的向量
    typedef struct VectorConverter VectorConverter;   // 转换器，将数组转换为向量，转换向量的数据类型，和打乱向量的顺序
    typedef struct VectorDeleter VectorDeleter;       // 删除器，用于删除向量
    typedef struct VectorCalculator VectorCalculator; // 计算器，用于向量的计算
    typedef struct VectorCopyer VectorCopyer;         // 复制器，用于将其他数组和向量的值进行复制
    typedef struct VectorComparator VectorComparator; // 比较器，包含和比较相关的操作
    typedef struct VectorResizer VectorResizer;       // 放缩器，用于对向量进行放缩，比如切片，扩展，索引等
    typedef struct VectorOperations VectorOperations; // 向量操作，包含了所有的向量操作

    void init_VectorGenerator(VectorGenerator *vector_generator);    // 初始化生成器
    void init_VectorConverter(VectorConverter *vector_converter);    // 初始化转换器
    void init_VectorDeleter(VectorDeleter *vector_deleter);          // 初始化删除器
    void init_VectorCalculator(VectorCalculator *vector_calculator); // 初始化计算器
    void init_VectorResizer(VectorResizer *vector_calculator);       // 初始化放缩器
    void init_VectorCopyer(VectorCopyer *vector_copyer);             // 初始化复制器
    void init_VectorComparator(VectorComparator *vector_comparator); // 初始化比较器
    VectorOperations *init_VectorOperations();                       // 初始化向量操作

    void delete_VectorOperations(VectorOperations **vector_operations); // 删除VectorOperations结构体

    // 该结构体规定了向量的属性，包括向量的数据类型，向量的大小，向量的数据
    typedef struct Vector
    {
        // 同一时间只有一个指针指向数组，其余指针为NULL
        int *i_array;    // int型数组
        float *f_array;  // float型数组
        double *d_array; // double型数组
        int size;        // 向量的大小
        int type;        // 向量的数据类型，取值为DOUBLE, FLOAT, INT
    } Vector;

    // 转换器，包含三类操作，分别是将数组转换为向量、转换向量的数据类型和打乱向量的顺序
    typedef struct VectorConverter
    {
        /// @brief 将特定数据类型的数组转换为一个新的向量，原数组将会被删除
        /// @param array 原数组，为二级指针
        /// @param size 原数组中的元素个数
        /// @return 返回一个新向量的指针
        Vector *(*convert_DoubleArray_to_NewVector)(double **array, int size);
        Vector *(*convert_FloatArray_to_NewVector)(float **array, int size);
        Vector *(*convert_IntArray_to_NewVector)(int **array, int size);

        /// @brief 将特定数据类型的数组转换到一个已存在的向量，会对目标向量本身进行操作，原数组将会被删除
        /// @param array 原数组，为二级指针
        /// @param size 原数组中的元素个数
        /// @param vector 指向目标向量的指针，不用考虑目标向量的数据类型和大小
        /// @return 返回一个指向目标向量的指针
        Vector *(*convert_DoubleArray_to_ExistingVector)(double **array, int size, Vector *vector);
        Vector *(*convert_FloatArray_to_ExistingVector)(float **array, int size, Vector *vector);
        Vector *(*convert_IntArray_to_ExistingVector)(int **array, int size, Vector *vector);

        /// @brief 将向量的数据类型转换为指定的数据类型
        /// @param vector 指向目标向量的指针
        /// @return 返回一个指向目标向量的指针
        Vector *(*toInt)(Vector *vector);
        Vector *(*toFloat)(Vector *vector);
        Vector *(*toDouble)(Vector *vector);

        /// @brief 打乱向量的顺序，直接对原向量进行操作
        /// @param vector 目标向量的指针
        /// @return 返回一个指向目标向量的指针
        Vector *(*shuffle)(Vector *vector);
    } VectorConverter;

    // 复制器，包含两类操作，分别是将数组的值复制到一个向量和将一个数组复制到另一个向量
    typedef struct VectorCopyer
    {
        /// @brief 将特定数据类型的数组的值复制到一个新的向量，原数组不会被删除
        /// @param array 原数组
        /// @param size 原数组中的元素个数
        /// @return 返回一个新向量的指针
        Vector *(*copy_DoubleArray_to_NewVector)(double *array, int size);
        Vector *(*copy_FloatArray_to_NewVector)(float *array, int size);
        Vector *(*copy_IntArray_to_NewVector)(int *array, int size);

        /// @brief 将特定数据类型的数组的值复制到一个已存在的向量，会对目标向量本身进行操作，原数组不会被删除
        /// @param array 原数组
        /// @param size 原数组中的元素个数
        /// @param vector 指向目标向量的指针，不用考虑目标向量的数据类型和大小
        /// @return 返回一个指向目标向量的指针
        Vector *(*copy_DoubleArray_to_ExistingVector)(double *array, int size, Vector *vector);
        Vector *(*copy_FloatArray_to_ExistingVector)(float *array, int size, Vector *vector);
        Vector *(*copy_IntArray_to_ExistingVector)(int *array, int size, Vector *vector);

        /// @brief 将一个向量的值复制到一个新的向量，不会对原向量进行任何操作
        /// @param vector 原向量
        /// @return 返回一个新向量的指针
        Vector *(*copy_Vector_to_NewVector)(Vector vector);

        /// @brief 将一个向量的值复制到一个已存在的向量，会对目标向量本身进行操作，原向量不会被删除
        /// @param vector 目标向量的指针，目标向量中元素的值、元素个数和数据类型都将根据原向量进行修改
        /// @param source_vector 原向量，只会被读取，不会被修改
        /// @return 返回一个指向目标向量的指针
        Vector *(*copy_Vector_to_ExistingVector)(Vector *vector, Vector source_vector);
    } VectorCopyer;

    // 生成器，包含六类操作，分别是生成空向量，生成全0向量，生成全1向量，生成随机向量，生成等差向量和生成正态分布向量
    typedef struct VectorGenerator
    {
        /// @brief 生成一个指定数据类型和大小的空向量，向量中元素的值为内存中的随机数
        /// @param type 向量的数据类型，取值为DOUBLE, FLOAT, INT
        /// @param size 向量的大小
        /// @return 返回一个新向量的指针
        Vector *(*generate_EmptyVector)(int type, int size);

        /// @brief 生成一个指定数据类型和大小的全0向量，向量中元素的值为0
        /// @param type 向量的数据类型，取值为DOUBLE, FLOAT, INT
        /// @param size 向量的大小
        /// @return 返回一个新向量的指针
        Vector *(*generate_ZerosVector)(int type, int size);

        /// @brief 生成一个指定数据类型和大小的全1向量，向量中元素的值为1
        /// @param type 向量的数据类型，取值为DOUBLE, FLOAT, INT
        /// @param size 向量的大小
        /// @return 返回一个新向量的指针
        Vector *(*generate_OnesVector)(int type, int size);

        /// @brief 生成一个指定数据类型和大小的随机向量，向量中元素的值为[min, max]之间的随机数
        /// @param type 向量的数据类型，取值为DOUBLE, FLOAT, INT
        /// @param size 向量的大小
        /// @param min 随机数的下限, 类型为double
        /// @param max 随机数的上限, 类型为double
        /// @return 返回一个新向量的指针
        Vector *(*generate_RandomVector)(int type, int size, double min, double max);

        /// @brief 生成一个指定数据类型和大小的等差向量，向量中元素的值为[start, end]之间的等差数列
        /// @param type 向量的数据类型，取值为DOUBLE, FLOAT, INT
        /// @param start 等差数列的起始值，类型为double
        /// @param end 等差数列的终止值, 类型为double
        /// @param step 等差数列的公差, 类型为double
        /// @return 返回一个新向量的指针
        Vector *(*generate_RangeVector)(int type, double start, double end, double step);

        /// @brief 生成一个指定数据类型和大小的正态分布向量，向量中元素的值为服从正态分布的随机数
        /// @param type 向量的数据类型，取值为DOUBLE, FLOAT, INT
        /// @param size 向量的大小
        /// @param mean 正态分布的期望, 类型为double
        /// @param std 正态分布的标准差, 类型为double
        /// @return 返回一个新向量的指针
        Vector *(*generate_NormalVector)(int type, int size, double mean, double std);
    } VectorGenerator;

    // 删除器，包含一个操作，删除一个指定向量
    typedef struct VectorDeleter
    {
        /// @brief 删除一个指定向量，向量中的数据将会被删除，向量本身也会被删除
        /// @param vector 目标向量的二级指针
        void (*delete_vector)(Vector **vector);
    } VectorDeleter;

    // 计算器，包含四类操作，分别是计算向量的加减乘、计算向量的统计值、向量的归一化和计算向量的范数
    typedef struct VectorCalculator
    {
        /// @brief 计算若干个向量的和，从第二个参数开始，每个参数的类型都需要是Vector，不会对原向量进行任何操作
        /// @param count 向量的个数
        /// @param ... 参与计算的向量，为Vector类型，其type和size都需要相同，否则会报错
        /// @return 返回一个新向量的指针，所指向的向量的数据类型和大小都和参与计算的向量相同
        Vector *(*add_variadic)(int count, ...);

        /// @brief 计算若干个向量的和，传入的参数是一个向量的数组，不会对原向量进行任何操作
        /// @param vectors 向量的数组，存储的数据类型是Vector类型，数组中向量的type和size都需要相同，否则会报错
        /// @param count 向量的个数
        /// @return 返回一个新向量的指针，所指向的向量的数据类型和大小都和参与计算的向量相同
        Vector *(*add_array)(Vector *vectors, int count);

        /// @brief 计算向量的数乘，即对向量中的每个元素都乘以一个常数，不会对原向量进行任何操作
        /// @param vector 目标向量
        /// @param times 常数
        /// @return 返回一个新向量的指针，所指向的向量的数据类型和大小都和目标向量相同，向量中的每个元素都乘以了常数
        Vector *(*multiple_vector)(Vector vector, double times);

        /// @brief 计算若干个向量的差，第一个向量为被减数，其余向量为减数，不会对原向量进行任何操作
        /// @param count 向量的个数
        /// @param ... 参与计算的向量，为Vector类型，其type和size都需要相同，否则会报错
        /// @return 返回一个新向量的指针，所指向的向量的数据类型和大小都和参与计算的向量相同
        Vector *(*subtract_variadic)(int count, ...);

        /// @brief 计算若干个向量的差，数组中第一个向量为被减数，其余向量为减数，不会对原向量进行任何操作
        /// @param vectors 向量的数组，存储的数据类型是Vector类型，数组中向量的type和size都需要相同，否则会报错
        /// @param count 向量的个数
        /// @return 返回一个新向量的指针，所指向的向量的数据类型和大小都和参与计算的向量相同
        Vector *(*subtract_array)(Vector *vectors, int count);

        /// @brief 计算两个向量的点乘，两个向量的type和size都需要相同，否则会报错，不会对原向量进行任何操作
        /// @param v1 第一个向量
        /// @param v2 第二个向量
        /// @return 返回两个向量的点乘结果，类型为double
        double (*dot_product)(Vector v1, Vector v2);

        /// @brief 计算向量的平均值，不会对原向量进行任何操作
        /// @param vector 目标向量
        /// @return 返回向量的平均值，类型为double
        double (*mean)(Vector vector);

        /// @brief 计算向量的标准差，不会对原向量进行任何操作
        /// @param vector 目标向量
        /// @return 返回向量的标准差，类型为double
        double (*standard)(Vector vector);

        /// @brief 计算向量的方差，不会对原向量进行任何操作
        /// @param vector 目标向量
        /// @return 返回向量的方差，类型为double
        double (*variance)(Vector vector);

        /// @brief 计算两个向量的协方差，两个向量的type和size都需要相同，否则会报错，不会对原向量进行任何操作
        /// @param v1 第一个向量
        /// @param v2 第二个向量
        /// @return 返回两个向量的协方差，类型为double
        double (*covariance)(Vector v1, Vector v2);

        /// @brief 计算两个向量的相关系数，两个向量的type和size都需要相同，否则会报错，不会对原向量进行任何操作
        /// @param v1 第一个向量
        /// @param v2 第二个向量
        /// @return 返回两个向量的相关系数，类型为double
        double (*correlation)(Vector v1, Vector v2);

        /// @brief 对向量进行0均值归一化，将向量变换为均值为0，标准差为1的向量，直接对目标向量本身进行操作
        /// @param vector 目标向量的指针
        /// @return 返回一个指向目标向量的指针
        Vector *(*StandardScaler)(Vector *vector);

        /// @brief 对向量进行最大最小值归一化，将向量变换为最大值为1，最小值为0的向量，直接对目标向量本身进行操作
        /// @param vector 目标向量的指针
        /// @return 返回一个指向目标向量的指针
        Vector *(*MinMaxScaler)(Vector *vector);

        /// @brief 计算向量的p范数，不会对原向量进行任何操作
        /// @param vector 目标向量
        /// @param p 范数的p值，类型为int
        /// @return 返回向量的p范数，类型为double
        double (*lp_norm)(Vector vector, int p);
    } VectorCalculator;

    // 放缩器，包含三类操作，分别是切片，扩展，高级索引
    typedef struct VectorResizer
    {
        /// @brief 对向量进行切片，即截取向量中连续的一部分，不会对原向量进行任何操作
        /// @param vector 目标向量
        /// @param start 切片的起始位置，类型为int，包含该位置的元素，可取负数，等同于python
        /// @param end 切片的终止位置，类型为int，不包含该位置的元素，可取负数，等同于python
        /// @param step 切片的步长，类型为int，可取负数，等同于python
        /// @return 返回一个新向量的指针，所指向的向量的数据类型和大小都和目标向量相同
        Vector *(*slice)(Vector vector, int start, int end, int step);

        /// @brief 对向量进行扩展，即将两个向量合并为一个新的向量，不会对原向量进行任何操作，两个向量的数据类型和大小都需要相同
        /// @param v1 第一个向量，其数据在新向量的前半部分
        /// @param v2 第二个向量，其数据在新向量的后半部分
        /// @return 返回一个新向量的指针，所指向的向量的数据类型和大小都和目标向量相同
        Vector *(*extend)(Vector v1, Vector v2);

        /// @brief 对向量进行神奇索引，即根据索引数组中的索引值，从目标向量中取出对应索引的元素，不会对原向量进行任何操作
        /// @param data 目标向量
        /// @param indices 索引数组，类型为Vector，数据类型为INT
        /// @return 返回一个新向量的指针，所指向的向量的数据类型和大小都和目标向量相同
        Vector *(*fancy_indexing)(Vector data, Vector indices);

        /// @brief 对向量进行布尔索引，即根据布尔数组中的布尔值，从目标向量中取出对应布尔值为true的元素，不会对原向量进行任何操作
        /// @param data 目标向量
        /// @param mask 布尔数组，类型为Vector，数据类型为INT，大小和目标向量一致，值只能取1和0，1表示布尔值true，0表示布尔值false
        /// @return 返回一个新向量的指针，所指向的向量的数据类型和目标向量相同，大小为布尔数组中布尔值为true的元素个数
        Vector *(*boolean_indexing)(Vector data, Vector mask);
    } VectorResizer;

    // 比较器，包含七类操作，分别是排序，排序后的索引，最大值，最大值的索引，最小值，最小值的索引，根据条件生成布尔值向量
    typedef struct VectorComparator
    {
        /// @brief 对向量中的数组进行升序排序，返回新向量的指针，不会对原向量进行任何操作
        /// @param vector 目标向量
        /// @return 返回一个新向量的指针，所指向的向量的数据类型和大小都和目标向量相同
        Vector *(*sort)(Vector vector);

        /// @brief 返回一个新向量的指针，新向量中数组的元素是原向量的数组升序排序后的索引，不会对原向量进行任何操作
        /// @param vector 目标向量
        /// @return 返回一个新向量的指针，数据类型为INT，大小和目标向量相同
        Vector *(*argsort)(Vector vector);

        /// @brief 找到向量中的最大值，并以double类型返回，不会对原向量进行任何操作
        /// @param vector 目标向量
        /// @return 返回向量中的最大值，类型为double
        double (*find_max)(Vector vector);

        /// @brief 找到向量中的最大值的索引，不会对原向量进行任何操作
        /// @param vector 目标向量
        /// @return 返回向量中的最大值的索引，类型为int
        int (*argmax)(Vector vector);

        /// @brief 找到向量中的最小值，并以double类型返回，不会对原向量进行任何操作
        /// @param vector 目标向量
        /// @return 返回向量中的最小值，类型为double
        double (*find_min)(Vector vector);

        /// @brief 找到向量中的最小值的索引，不会对原向量进行任何操作
        /// @param vector 目标向量
        /// @return 返回向量中的最小值的索引，类型为int
        int (*argmin)(Vector vector);

        /// @brief 根据条件从向量中取出满足条件的元素，不会对原向量进行任何操作
        /// @param vector 目标向量
        /// @param condition 条件，类型为char*，可取值为">", "<", ">=", "<=", "=="
        /// @param value 条件的值，类型为double
        /// @return 返回一个新向量的指针，所指向的向量的数据类型为INT，值只有0和1，大小同目标向量
        Vector *(*get_BooleanVector_for_condition)(Vector vector, char *condition, double value);
    } VectorComparator;

    // 向量操作，是所有涉及到向量操作的结构体的父类，包含了所有涉及到向量操作的结构体，这些结构体均为该结构体的具体模块
    typedef struct VectorOperations
    {
        VectorGenerator vector_generator;
        VectorDeleter vector_deleter;
        VectorConverter vector_converter;
        VectorCopyer vector_copyer;
        VectorCalculator vector_calculator;
        VectorResizer vector_resizer;
        VectorComparator vector_comparator;
    } VectorOperations;

#ifdef __cplusplus
}
#endif

#endif
