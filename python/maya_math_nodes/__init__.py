# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
import expression_builder
import expression_parser

NodeNameGenerator = expression_builder.NodeNameGenerator


def eval_expression(expression, base_node_name='', name_generator=None):
    """Evaluate Expression

    Args:
        expression (str): Math expression to evaluate
        base_node_name (str): Base name used to initialize new node name generator
        name_generator (NodeNameGenerator): Node name generator used by the builder

    Returns:
        str: Returns resulting output attribute for generated node graph
    """
    if not name_generator:
        name_generator = NodeNameGenerator(base_node_name)

    ast = expression_parser.ExpressionParser(expression).parse()
    builder = expression_builder.ExpressionBuilder(name_generator)
    output = builder.generate(ast)
    builder.build()

    return output.value
