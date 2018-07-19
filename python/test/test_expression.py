# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
from expression_test_case import ExpressionTestCase, ParsingError, BuildingError


class TestExpression(ExpressionTestCase):
    def test_operators(self):
        self.eval_expression('2 + 2', 4, 'math_AddInt')
        self.eval_expression('2 + 2.0', 4, 'math_AddInt')
        self.eval_expression('2.0 + 2', 4.0, 'math_Add')
        self.eval_expression('2.0 + 2.0', 4.0, 'math_Add')
        self.eval_expression('dummy.tx + dummy.ty', 4.0, 'math_Add')
        self.eval_expression('dummy.rx + 2', 4.0, 'math_AddAngle')
        self.eval_expression('2 + dummy.rx', 0.0, exception=BuildingError)
        
        self.eval_expression('4 - 2', 2, 'math_SubtractInt')
        self.eval_expression('4 - 2.0', 2, 'math_SubtractInt')
        self.eval_expression('4.0 - 2', 2.0, 'math_Subtract')
        self.eval_expression('4.0 - 2.0', 2.0, 'math_Subtract')
        self.eval_expression('dummy.rx - 2', 0.0, 'math_SubtractAngle')
        self.eval_expression('2 - dummy.tx', 0.0, exception=BuildingError)
        
        self.eval_expression('2 / 2', 1.0, 'math_DivideByInt')
        self.eval_expression('2.0 / 2', 1.0, 'math_DivideByInt')
        self.eval_expression('2 / 2.0', 1.0, 'math_Divide')
        self.eval_expression('2.0 / 2.0', 1.0, 'math_Divide')
        self.eval_expression('dummy.rx / 2.0', 1.0, 'math_DivideAngle')
        self.eval_expression('dummy.rx / 2', 1.0, 'math_DivideAngleByInt')
        self.eval_expression('dummy.rx / dummy.tx', 0.0, exception=BuildingError)
        
        self.eval_expression('2 % 2', 0, 'math_ModulusInt')
        self.eval_expression('2.0 % 2', 0, 'math_ModulusInt')
        self.eval_expression('2 % 2.0', 0, 'math_ModulusInt')
        self.eval_expression('2.0 % 2.0', 0, 'math_ModulusInt')
        self.eval_expression('dummy.tx % 2', 0, exception=BuildingError)
        self.eval_expression('2 % dummy.tx', 0, exception=BuildingError)
        
        self.eval_expression('2 * 2', 4, 'math_MultiplyInt')
        self.eval_expression('2 * 2.0', 4, 'math_MultiplyInt')
        self.eval_expression('2.0 * 2', 4.0, 'math_MultiplyByInt')
        self.eval_expression('2.0 * 2.0', 4.0, 'math_Multiply')
        self.eval_expression('dummy.tx * 2', 4.0, 'math_MultiplyByInt')
        self.eval_expression('dummy.rx * 2', 4.0, 'math_MultiplyAngleByInt')
        self.eval_expression('dummy.rx * 2.0', 4.0, 'math_MultiplyAngle')
        self.eval_expression('dummy.rx * dummy.ry', 4.0, 'math_MultiplyAngle')
        self.eval_expression('2.0 * dummy.rx', 0.0, exception=BuildingError)
        self.eval_expression('dummy.rx * dummy.tx', 0.0, exception=BuildingError)
        
        self.eval_expression('{1, 0, 0} * 2', [2.0, 0.0, 0.0], 'math_MultiplyVector')
        self.eval_expression('{1, 0, 0} * 2.0', [2.0, 0.0, 0.0], 'math_MultiplyVector')
        self.eval_expression('{1, 0, 0} * dummy.tx', [0.0, 0.0, 0.0], exception=BuildingError)
        self.eval_expression('{1, 0, 0} * {1, 0, 0}', [0.0, 0.0, 0.0], exception=RuntimeError)  # needs fixing
        self.eval_expression('2 * {1, 0, 0}', [0.0, 0.0, 0.0], exception=BuildingError)
        self.eval_expression('1, 0, 0} * 2', 0.0, exception=ParsingError)
        self.eval_expression('{1, 0, 0}} * 2', 0.0, exception=ParsingError)
        
        self.eval_expression('{0.7071, 0.0, 0.0, 0.7071} * {0.7071, 0.0, 0.0, 0.7071}',
                             [1.0, 0.0, 0.0, 0.0], 'math_MultiplyQuaternion')
        self.eval_expression('{0.7071, 0.0, 0.0, 0.7071} * 2',
                             [1.0, 0.0, 0.0, 0.0],
                             exception=BuildingError)
        
        matrix1 = '{0.0, 0.0, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}'
        matrix2 = '{0.0, 0.0, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}'
        result = [-1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0]
        self.eval_expression('{0} * {1}'.format(matrix1, matrix2), result, 'math_MultiplyMatrix')

    def test_precedence(self):
        self.eval_expression('2.0 + 2.0 * 2.0 / 2.0 - 1.0', 3.0)
        self.eval_expression('(2.0 + 2.0) * (2.0 / (2.0 - 1.0))', 8.0)
        self.eval_expression('(2.0 + 2.0) * (2.0 / 2.0 - 1.0))', 0.0, exception=ParsingError)
        self.eval_expression('(2.0 + 2.0) * (2.0 / (2.0 - 1.0)', 0.0, exception=ParsingError)

    def test_conditional(self):
        self.eval_expression('2 == 2 ? 1 : 0', 1)
        self.eval_expression('2.0 == 2.0 ? 1 : 0', 1)
        self.eval_expression('2.0 == 2 ? 1 : 0', 1)
        self.eval_expression('2 == 2.0 ? 1 : 0', 1)
        self.eval_expression('dummy.tx == dummy.ty ? 1 : 0', 1)
        self.eval_expression('dummy.rx == dummy.ty ? 1 : 0', 1, exception=BuildingError)
        
        self.eval_expression('dummy.tx > 1.0 ? 2.0 + 2.0 : 2.0 - 2.0', 4.0, 'math_Select')
        self.eval_expression('dummy.tx < 1.0 ? 2.0 + 2.0 : 2', 2.0, 'math_Select')
        self.eval_expression('dummy.tx > 1.0 ? 2 + 2 : 2 - 2', 4, 'math_SelectInt')
        self.eval_expression('dummy.tx > 1.0 ? 2 + 2 : 2.0', 4, 'math_SelectInt')
        self.eval_expression('dummy.tx > 1.0 ? 2.0 + 2.0 : 2 - 2', 0.0, exception=BuildingError)
