# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
"""Maya Math Expression Lexer

Expression Language Rules:
  - single line only
  - 
"""
import collections
import copy
import functools
import re

OPERATORS = ['+', '/', '%', '*', '-', '<', '>', '<=', '>=', '==', '!=', '?', ':']

PRECEDENCE = {
    "<": 10, ">": 10, "<=": 10, ">=": 10, "==": 10, "!=": 10,
    "+": 20, "-": 20,
    "*": 30, "/": 30, "%": 30,
}

Token = collections.namedtuple('Token', ['type', 'value'])

BraketToken = 0
NumberToken = 1
OperatorToken = 2
StringToken = 3

class AST(list):
    pass

class Number(object):
    def __init__(self, value):
        self.value = value

class MayaReference(object):
    def __init__(self, value):
        self.value = value

class Bool(object):
    def __init__(self, value):
        self.value = value

class NodeFunction(object):
    def __init__(self, value, args):
        self.value = value
        self.args = args

class Binary(object):
    def __init__(self, value, left, right):
        self.value = value
        self.left = left
        self.right = right

class ExpressionStream(object):
    """Expression Stream"""
    def __init__(self, input_str):
        self._data = input_str
        self._pos = 0

    def next(self):
        """Read next character in the stream"""
        char = self._data[self._pos]
        self._pos += 1
        return char
    
    def peek(self):
        """Look ahead at next character in the stream"""
        return self._data[self._pos]
    
    def end(self):
        """Check for end of expression"""
        return self._pos == len(self._data)
    
    def error(self, message):
        raise RuntimeError('{0} at {1}'.format(message, self._pos))

class ExpressionLexer(object):
    """Expression Lexer"""
    def __init__(self, input_stream):
        self._data = ExpressionStream(input_stream)
        self._current = None
    
    def next(self):
        """Read next token from stream"""
        token = self._current
        self._current = None
        return token or self.read_next()

    def peek(self):
        """Look ahead at next token in stream"""
        if not self._current:
            self._current = self.read_next()
        return self._current
    
    def end(self):
        """Check for end of expression"""
        return self.peek() == None
    
    def error(self, message):
        self._data.error(message)

    def read_next(self):
        """Read next token from stream"""
        self.read_while(self.is_whitespace)
        if self._data.end():
            return None

        char = self._data.peek()
        if self.is_string(char):
            return self.read_string()
        if self.is_digit(char):
            return self.read_number()
        if self.is_operator(char):
            return self.read_operator()
        
        self._data.error('Failed to handle character: {0}'.format(char))

    def read_while(self, predicate):
        """Read from stream while condition of the predicate is met"""
        str = ''
        while not self._data.end() and predicate(self._data.peek()):
            str += self._data.next()

        return str
    
    def is_whitespace(self, char):
        """Check for whitespace character"""
        return char == ' '

    def is_digit(self, char, dot_ok=False):
        """Check for digit character"""
        if dot_ok and char == '.':
            return True
        return char.isdigit()
    
    def read_number(self):
        """Read number from stream"""
        return Token(NumberToken, self.read_while(functools.partial(self.is_digit, dot_ok=True)))

    def is_operator(self, char):
        """Check for operator character"""
        return char in OPERATORS

    def read_operator(self):
        """Read operator from stream"""
        return Token(OperatorToken, self.read_while(self.is_operator))

    def is_bracket(self, char):
        """Check for bracket character"""
        return char in ['(', ')', '[', ']']
    
    def is_string(self, char):
        """Check for string character"""
        return char.isalpha()
    
    def is_not_string(self, char):
        """Check for non-string character"""
        return not self.is_string(char)
    
    def read_string(self):
        """Read string from stream"""
        return Token(StringToken, self.read_while(self.is_not_string))

class ExpressionParser(object):
    def __init__(self, token_stream):
        self._data = ExpressionLexer(token_stream)

    def error(self, token):
        self._data.error(token)
    
    def parse(self):
        """Parse the entire expression"""
        return self.parse_expression()

    def get_precedence(self, token):
        """Get operator precedence"""
        if not token or token.type is not OperatorToken:
            return -1
        
        return PRECEDENCE[token.value]

    def parse_number(self, token):
        """Parse number token"""
        return Number(token.value)

    def parse_element(self):
        """Parse token element"""
        token = self._data.next()
        if not token:
            self.error(token)
        
        if token.type == NumberToken:
            self.parse_number(token)
        elif token.type == StringToken:
            self.parse_string()
        
        return token
    
    def parse_expression(self):
        """Parse expression"""
        left = self.parse_element()
        return self.parse_binary_right(0, left)
    
    def parse_binary_right(self, prec, left):
        """Parse binary expression with precendence"""
        while True:
            op_token = self._data.next()
            left_prec = self.get_precedence(op_token)
            if left_prec < prec:
                return left
            
            right = self.parse_element()
            next_op_token = self._data.peek()
            if left_prec < self.get_precedence(next_op_token):
                right = self.parse_binary_right(left_prec + 1, right)
            
            left = Binary(op_token.value, left, right)
