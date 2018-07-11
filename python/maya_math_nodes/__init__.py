# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
import expression_builder
import expression_parser

def eval_expression(expression, base_node_name, name_generator=expression_builder.NodeNameGenerator):
    """Evaluate Expression
    
    Args:
        expression (str): Math expression to evaluate
        base_node_name (str): Base name used by the node name generator
        name_generator (NodeNameGenerator): Optional custom node name generator class

    Returns:
        str: Returns resulting output attribute for generated node graph
    """
    ast = expression_parser.ExpressionParser(expression).parse()
    builder = expression_builder.ExpresionBuilder(name_generator(base_node_name))
    output = builder.generate(ast)
    builder.build()

    return output.value
