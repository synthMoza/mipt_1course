#define GRAPH_DOT_FILE_NAME "expression_tree.gv"
#define GRAPH_TREE_FILE_NAME "expression_tree.ps"

#define OPTIMIZE_COUNT 5

#define createNode(type, val, left, right)                  \
({                                                          \
  Tree_t* node = NULL;                                      \
  node = (Tree_t*) calloc(1, sizeof(Tree_t));               \
  node->valueType = (type);                                 \
  node->value = (val);                                      \
  node->leftChild = (left);                                 \
  node->rightChild = (right);                               \
  node;                                                     \
})

#define diffLeft(tree) diffExpression((tree)->leftChild)
#define diffRight(tree) diffExpression((tree)->rightChild)
#define diffPlusMinus(tree) createNode(TOKEN_OP, (tree)->value, diffLeft(tree), diffRight(tree));
#define diffMulLeft(tree) createNode(TOKEN_OP, OP_MUL, diffLeft(tree), copyTree((tree)->rightChild))
#define diffMulRight(tree) createNode(TOKEN_OP, OP_MUL, copyTree((tree)->leftChild), diffRight(tree))
#define diffMul(tree) createNode(TOKEN_OP, OP_PLUS, diffMulLeft(tree), diffMulRight(tree))
#define diffDiv(tree) createNode(TOKEN_OP, OP_DIV, diffDivLeft(tree), diffDivRight(tree))
#define diffDivLeft(tree) createNode(TOKEN_OP, OP_MINUS, diffDivLeft2(tree), diffDivRight2(tree))
#define diffDivRight(tree) createNode(TOKEN_OP, OP_POW, copyTree(tree->rightChild), createNode(TOKEN_NUM, 2, NULL, NULL))
#define diffDivLeft2(tree) createNode(TOKEN_OP, OP_MUL, diffLeft(tree), copyTree((tree)->rightChild))
#define diffDivRight2(tree) createNode(TOKEN_OP, OP_MUL, copyTree((tree)->leftChild), diffRight(tree))
#define diffPow(tree) createNode(TOKEN_OP, OP_MUL, copyTree(tree), diffPow1(tree))
#define diffPow1(tree) createNode(TOKEN_OP, OP_PLUS, diffPowLeft(tree), diffPowRight(tree))
#define diffPowLeft(tree) createNode(TOKEN_OP, OP_MUL, diffRight(tree), createNode(TOKEN_OP, OP_LN, copyTree((tree)->rightChild), NULL))
#define diffPowRight(tree) createNode(TOKEN_OP, OP_MUL, copyTree((tree)->rightChild), createNode(TOKEN_OP, OP_DIV, diffLeft(tree), copyTree((tree)->leftChild)))
#define diffSin(tree) createNode(TOKEN_OP, OP_MUL, createNode(TOKEN_OP, OP_COS, copyTree((tree)->leftChild), NULL), diffLeft(tree))
#define diffCos(tree) createNode(TOKEN_OP, OP_MUL, diffCos1(tree), diffLeft(tree))
#define diffCos1(tree) createNode(TOKEN_OP, OP_MUL, createNode(TOKEN_NUM, -1, NULL, NULL), createNode(TOKEN_OP, OP_SIN, copyTree((tree)->leftChild), NULL))
#define diffLn(tree) createNode(TOKEN_OP, OP_DIV, diffLeft(tree), copyTree((tree)->leftChild))
#define diffTan(tree) createNode(TOKEN_OP, OP_MUL, diffTan1(tree), diffLeft(tree))
#define diffTan1(tree) createNode(TOKEN_OP, OP_DIV, createNode(TOKEN_NUM, 1, NULL, NULL), diffTan2(tree))
#define diffTan2(tree) createNode(TOKEN_OP, OP_POW, createNode(TOKEN_OP, OP_COS, copyTree((tree)->leftChild), NULL), createNode(TOKEN_NUM, 2, NULL, NULL))
#define diffSqrt(tree) createNode(TOKEN_OP, OP_MUL, diffSqrt1(tree), diffLeft(tree))
#define diffSqrt1(tree) createNode(TOKEN_OP, OP_DIV, createNode(TOKEN_NUM, 1, NULL, NULL), diffSqrt2(tree))
#define diffSqrt2(tree) createNode(TOKEN_OP, OP_MUL, createNode(TOKEN_NUM, 2, NULL, NULL), createNode(TOKEN_OP, OP_SQRT, copyTree((tree)->leftChild), NULL))

#define latex_PLUS(tree) {                                                      \
    latexPrint((tree)->leftChild, output_file);                                 \
    fprintf(output_file, " + ");                                                \
    latexPrint((tree)->rightChild, output_file);                                \
}

#define latex_MINUS(tree) {                         \
    latexPrint((tree)->leftChild, output_file);                                 \
    fprintf(output_file, " - ");                                                \
    latexPrint((tree)->rightChild, output_file);                                \
}

#define latex_MUL(tree)  {                              \
    if ((tree)->leftChild->valueType == TOKEN_OP && ((tree)->leftChild->value == OP_PLUS || (tree)->leftChild->value == OP_MINUS))  \
    {                                                   \
        fprintf(output_file, "(");                      \
        latexPrint((tree)->leftChild, output_file);     \
        fprintf(output_file, ")");                      \
    }                                                   \
    else                                                \
    {                                                   \
        latexPrint((tree)->leftChild, output_file);     \
    }                                                   \
    fprintf(output_file, "*");                          \
    if ((tree)->rightChild->valueType == TOKEN_OP && ((tree)->rightChild->value == OP_PLUS || (tree)->rightChild->value == OP_MINUS))  \
    {                                                   \
        fprintf(output_file, "(");                      \
        latexPrint((tree)->rightChild, output_file);    \
        fprintf(output_file, ")");                      \
    }                                                   \
    else                                                \
    {                                                   \
        latexPrint((tree)->rightChild, output_file);    \
    }                                                   \
}

#define latex_DIV(tree) {                           \
    fprintf(output_file, "\\frac{");                \
    latexPrint((tree)->leftChild, output_file);     \
    fprintf(output_file, "}{");                     \
    latexPrint((tree)->rightChild, output_file);    \
    fprintf(output_file, "}");                      \
}

#define latex_POW(tree) {                           \
    if ((tree)->leftChild->valueType == TOKEN_NUM || (tree)->leftChild->valueType == TOKEN_VAR)     \
        latexPrint((tree)->leftChild, output_file); \
    else                                            \
    {                                               \
        fprintf(output_file, "(");                  \
        latexPrint((tree)->leftChild, output_file); \
        fprintf(output_file, ")");                  \
    }                                               \
    fprintf(output_file, "^");                      \
    if ((tree)->rightChild->valueType == TOKEN_NUM || (tree)->rightChild->valueType == TOKEN_VAR)     \
        latexPrint((tree)->rightChild, output_file);    \
    else                                                \
    {                                                   \
        fprintf(output_file, "{");                      \
        latexPrint((tree)->rightChild, output_file);    \
        fprintf(output_file, "}");                      \
    }                                                   \
}

#define latex_LN(tree) {                            \
    fprintf(output_file, "ln(");                    \
    latexPrint((tree)->leftChild, output_file);     \
    fprintf(output_file, ")");                      \
}

#define latex_SIN(tree) {                           \
    fprintf(output_file, "sin(");                   \
    latexPrint((tree)->leftChild, output_file);     \
    fprintf(output_file, ")");                      \
}

#define latex_COS(tree) {                           \
    fprintf(output_file, "cos(");                   \
    latexPrint((tree)->leftChild, output_file);     \
    fprintf(output_file, ")");                      \
}

#define latex_TAN(tree) {                           \
    fprintf(output_file, "tan(");                   \
    latexPrint((tree)->leftChild, output_file);     \
    fprintf(output_file, ")");                      \
}

#define latex_SQRT(tree) {                          \
    fprintf(output_file, "\\sqrt{");                \
    latexPrint((tree)->leftChild, output_file);     \
    fprintf(output_file, "}");                      \
}

#define latex_PI(tree)  {                           \
    fprintf(output_file, "\\pi");                   \
}

#define latex_EXP(tree) {                           \
    fprintf(output_file, "e");                      \
}