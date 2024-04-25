import heapq
import Node


def get_character_codes(character_codes, node, node_value=''):
    new_node_value = node_value + str(node.direction)

    if node.left:
        get_character_codes(character_codes, node.left, new_node_value)

    if node.right:
        get_character_codes(character_codes, node.right, new_node_value)

    if node.left is None and node.right is None:
        character_codes[node.character] = new_node_value


def get_huffman_tree(message):
    distinct_characters = {}
    for character in message:
        if character not in distinct_characters:
            distinct_characters[character] = 1
        else:
            distinct_characters[character] += 1

    leaf_nodes = []
    for character in distinct_characters:
        leaf_node = Node.Node(character, distinct_characters[character])
        leaf_nodes.append(leaf_node)

    min_heap = []
    for leaf_node in leaf_nodes:
        heapq.heappush(min_heap, leaf_node)

    while len(min_heap) > 1:
        min_left = heapq.heappop(min_heap)
        min_left.direction = 0
        min_right = heapq.heappop(min_heap)
        min_right.direction = 1
        merged_node = Node.Node('Internal Node', min_left.frequency + min_right.frequency, min_left, min_right)
        heapq.heappush(min_heap, merged_node)

    return min_heap


def encode_message(message):
    min_heap = get_huffman_tree(message)
    root_node = heapq.heappop(min_heap)
    character_codes = {}
    get_character_codes(character_codes, root_node)
    encoded_message = ''
    for character in message:
        encoded_message += character_codes[character]

    return encoded_message


def decode_message(min_heap, encoded_message):
    decoded_message = ''
    root_node = heapq.heappop(min_heap)
    current_node = root_node
    for character in encoded_message:
        if character == '0':
            current_node = current_node.left
        else:
            current_node = current_node.right

        if current_node.left is None and current_node.right is None:
            decoded_message += current_node.character
            current_node = root_node

    return decoded_message
