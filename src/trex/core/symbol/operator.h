
#ifndef __TREX_OPERATOR_H__

#define __TREX_OPERATOR_H__

typedef float (*GetOperatorResult)(float a, float b);

typedef struct Operator {
    char *_str;
    GetOperatorResult _get_result;
} Operator;

typedef struct OperatorArray {
    Operator *_data;
    size_t _length;
} OperatorArray;

#endif  // !__TREX_OPERATOR_H__