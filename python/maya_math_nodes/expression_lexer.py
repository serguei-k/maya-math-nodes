# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
import collections
import functools

from expression_info import *

BracketToken = 0
NumberToken = 1
OperatorToken = 2
ConditionToken = 3
TernaryToken = 4
StringToken = 6
FunctionToken = 7
CommaToken = 8

Token = collections.namedtuple('Token', ['type', 'value'])


class ParsingError(Exception):
    """Parsing exception raised from lexer or parser"""
    pass


class ExpressionStream(object):
    """Expression Stream"""
    def __init__(self, input_str):
        """Initialize expression stream

        Args:
            input_str (str): Raw input string
        """
        self._data = input_str
        self._pos = 0

    def next(self):
        """Read next character in the stream

        Returns:
            str: Returns next character in string
        """
        char = self._data[self._pos]
        self._pos += 1
        return char

    def peek(self, offset=0):
        """Look ahead at next character in the stream without taking a step forward
        
        Args:
            offset (int): Offset from current position

        Returns:
            str: Returns next character in string
        """
        if self.end(offset):
            return None

        return self._data[self._pos + offset]

    def end(self, offset=0):
        """Check for end of expression

        Args:
            offset (int): Offset from current position

        Returns:
            bool: Returns True if end of string has been reached
        """
        return self._pos + offset == len(self._data)

    def error(self, message):
        """Raise parsing error

        Args:
            message (str): Error message to include with exception
        """
        raise ParsingError('{0} at {1}'.format(message, self._pos))


class ExpressionLexer(object):
    """Expression Lexer"""
    def __init__(self, input_str):
        """Initialize the lexer

        Args:
           input_str (str): Raw input expression to parse
        """
        if input_str.count('(') != input_str.count(')'):
            raise ParsingError('Found odd number of parentheses in the expression!')

        if input_str.count('{') != input_str.count('}'):
            raise ParsingError('Found odd number of curly braces in the expression!')

        if input_str.count('[') != input_str.count(']'):
            raise ParsingError('Found odd number of square brackets in the expression!')

        self._data = ExpressionStream(input_str)
        self._current = None
        self._next = None

    def next(self):
        """Read next token from stream and save it as current

        Returns:
            Token: Returns next token
        """
        if self._next:
            self._current = self._next
            self._next = None
        else:
            self._current = self.read_next()
        return self._current

    def peek(self):
        """Look ahead at next token in stream and save it as next

        Returns:
            Token: Returns next token
        """
        if not self._next:
            self._next = self.read_next()
        return self._next

    def end(self):
        """Check for end of expression

        Returns:
            bool: Returns True if end of stream has been reached
        """
        return self.peek() is None

    def error(self, message):
        """Raise parsing error

        Args:
            message (str): Error message to include with exception
        """
        self._data.error(message)

    def read_next(self):
        """Read next token from stream

        Returns:
            Token: Returns next token
        """
        self.read_while(self.is_whitespace)
        if self._data.end():
            return None

        char = self._data.peek()
        next_char = self._data.peek(1)

        if self.is_string(char):
            return self.read_string()
        if self.is_digit(char):
            return self.read_number()
        if self.is_conditional(char, next_char):
            return self.read_conditional()
        if self.is_operator(char):
            return self.read_operator()
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
        """Read from stream while condition of the predicate is met

        Args:
            predicate (function): Predicate that will exit current read

        Returns:
            str: Return resulting read string
        """
        str = ''
        while not self._data.end() and predicate(self._data.peek()):
            str += self._data.next()

        return str

    @staticmethod
    def is_whitespace(char):
        """Check for whitespace character

        Args:
            char (str): Single character to check

        Returns:
            bool: Returns True if char is whitespace
        """
        return char == ' '

    @staticmethod
    def is_digit(char, dot_ok=False):
        """Check for digit character

        Args:
            char (str): Single character to check
            dot_ok (bool): Consider . as a digit to parse decimals

        Returns:
            bool: Returns True if char is digit
        """
        if dot_ok and char == '.':
            return True
        return char.isdigit()

    def read_number(self, negate=''):
        """Read number from stream

        Args:
            negate (str): Negation sign for numbers

        Returns:
            Token: Returns read token
        """
        return Token(NumberToken, negate + self.read_while(functools.partial(self.is_digit, dot_ok=True)))

    @staticmethod
    def is_operator(char):
        """Check for operator character

        Args:
            char (str): Single character to check

        Returns:
            bool: Returns True if char is a supported operator
        """
        return char in OPERATORS

    def read_operator(self):
        """Read operator from stream

        Returns:
            Token: Returns read token
        """
        return Token(OperatorToken, self.read_while(self.is_operator))

    @staticmethod
    def is_conditional(char, next_char=''):
        """Check for conditional character

        Args:
            char (str): Single character to check
            next_char (str): Single character that follows immediately after

        Returns:
            bool: Returns True if char is a supported conditional operator
        """
        if char in CONDITION:
            if next_char and char in CONDITION[-2:]:
                return next_char in CONDITION
            return True
        return False

    def read_conditional(self):
        """Read conditional from stream

        Returns:
            Token: Returns read token
        """
        return Token(ConditionToken, self.read_while(self.is_conditional))

    @staticmethod
    def is_ternary(char):
        """Check for ternary character

        Args:
            char (str): Single character to check

        Returns:
            bool: Returns True if char is a ternary condition operator
        """
        return char in TERNARY

    def read_ternary(self):
        """Read ternary from stream

        Returns:
            Token: Returns read token
        """
        return Token(TernaryToken, self.read_while(self.is_ternary))

    @staticmethod
    def is_bracket(char):
        """Check for bracket character

        Args:
            char (str): Single character to check

        Returns:
            bool: Returns True if char is a bracket
        """
        return char in ['(', ')', '[', ']']

    def read_bracket(self):
        """Read bracket from stream

        Returns:
            Token: Returns read token
        """
        return Token(BracketToken, self._data.next())

    @staticmethod
    def is_string(char, strict=True):
        """Check for string character

        Args:
            char (str): Single character to check
            strict (bool): Only consider alpha characters to be strings

        Returns:
            bool: Returns True if char is a string
        """
        if strict:
            return char.isalpha()

        return char.isalnum() or char in ['.', '_', ':']

    def read_string(self):
        """Read string from stream

        Returns:
            Token: Returns read token
        """
        return Token(StringToken, self.read_while(functools.partial(self.is_string, strict=False)))

    @staticmethod
    def is_open_curly(char):
        """Check for open curly brace character

        Args:
            char (str): Single character to check

        Returns:
            bool: Returns True if char is an opening curly brace
        """
        return char == '{'

    @staticmethod
    def is_not_close_curly(char):
        """Check for close curly brace character

        Args:
            char (str): Single character to check

        Returns:
            bool: Returns True if char is not a closing curly brace
        """
        return char != '}'

    def read_curly(self):
        """Read curly brace value from stream

        Returns:
            Token: Returns read token
        """
        self._data.next()  # eat open brace
        value = self.read_while(self.is_not_close_curly)

        if self._data.peek() != '}':
            self.error('Expected closing curly brace, got "{0}" instead'.format(self._data.peek()))

        value = value.replace(' ', '').split(',')
        if len(value) not in [3, 4, 16]:
            self.error('Expected a vector, quaternion or matrix, got "{0}" instead'.format(value))

        self._data.next()  # eat close brace
        return Token(NumberToken, value)
