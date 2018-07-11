# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
import collections
import functools

from expression_info import *

OPERATORS = ['+', '/', '%', '*', '-']
CONDITION = ['<', '>', '=', '!']
TERNARY = ['?', ':']

BracketToken = 0
NumberToken = 1
OperatorToken = 2
ConditionToken = 3
TernaryToken = 4
StringToken = 6
FunctionToken = 7
CommaToken = 8

Token = collections.namedtuple('Token', ['type', 'value'])


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
        if self.end():
            return None
        
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
        self._next = None
    
    def next(self):
        """Read next token from stream"""
        if self._next:
            self._current = self._next
            self._next = None
        else:
            self._current = self.read_next()
        return self._current

    def peek(self):
        """Look ahead at next token in stream"""
        if not self._next:
            self._next = self.read_next()
        return self._next
    
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
        if self.is_conditional(char):
            return self.read_conditional()
        if self.is_ternary(char):
            return self.read_ternary()
        if self.is_bracket(char):
            return self.read_bracket()
        if self.is_open_curly(char):
            return self.read_curly()
        if char == ',':
            return Token(CommaToken, self._data.next())
        
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
    
    def read_number(self, negate=''):
        """Read number from stream"""
        return Token(NumberToken, negate + self.read_while(functools.partial(self.is_digit, dot_ok=True)))

    def is_operator(self, char):
        """Check for operator character"""
        return char in OPERATORS

    def read_operator(self):
        """Read operator from stream"""
        return Token(OperatorToken, self.read_while(self.is_operator))

    def is_conditional(self, char):
        """Check for conditional character"""
        return char in CONDITION

    def read_conditional(self):
        """Read conditional from stream"""
        return Token(ConditionToken, self.read_while(self.is_conditional))
    
    def is_ternary(self, char):
        """Check for ternary character"""
        return char in TERNARY

    def read_ternary(self):
        """Read ternary from stream"""
        return Token(TernaryToken, self.read_while(self.is_ternary))

    def is_bracket(self, char):
        """Check for bracket character"""
        return char in ['(', ')', '[', ']']
    
    def read_bracket(self):
        """Read bracket from stream"""
        return Token(BracketToken, self._data.next())
    
    def is_string(self, char, strict=True):
        """Check for string character"""
        if strict:
            return char.isalpha()
        
        return char.isalnum() or char in ['.', '_', ':']
    
    def read_string(self):
        """Read string from stream"""
        return Token(StringToken, self.read_while(functools.partial(self.is_string, strict=False)))
    
    def is_open_curly(self, char):
        """Check for open curly brance character"""
        return char == '{'
    
    def is_not_close_curly(self, char):
        """Check for close curly brance character"""
        return char != '}'
    
    def read_curly(self):
        """Read curly brace value from stream"""
        self._data.next()  # ear open brace
        value = self.read_while(self.is_not_close_curly)
        
        if self._data.peek() != '}':
            self.error('Expected closing curly brance, got "{0}" instead'.format(self._data.peek()))

        value = value.replace(' ', '').split(',')
        if len(value) not in [3, 4, 16]:
            self.error('Expected a vector, quaternion or matrix, got "{0}" instead'.format(value))
        
        self._data.next()  # ear close brace
        return Token(NumberToken, value)
