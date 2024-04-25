import pickle


def write_to_file(filename, min_heap, encoded_message):
    with open(filename, 'wb') as output_file:
        pickle.dump(min_heap, output_file)
        pickle.dump(encoded_message, output_file)


def read_from_file(filename):
    with open(filename, 'rb') as input_file:
        min_heap = pickle.load(input_file)
        encoded_message = pickle.load(input_file)

    return min_heap, encoded_message
