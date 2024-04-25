class Node:
    def __init__(self, character, frequency, left=None, right=None):
        self.character = character
        self.frequency = frequency
        self.left = left
        self.right = right
        self.direction = ''

    def __lt__(self, other):
        return self.frequency < other.frequency
