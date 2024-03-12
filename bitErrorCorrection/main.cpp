#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

#define wordLength 8
#define codeWordLength 16

const bool H[wordLength][codeWordLength] = {
        {1, 0, 0, 0, 1, 1, 1, 0,    1, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 1, 1, 1,    0, 1, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 0, 1, 1, 0, 1,    0, 0, 1, 0, 0, 0, 0, 0},
        {1, 1, 0, 1, 1, 0, 0, 0,    0, 0, 0, 1, 0, 0, 0, 0},
        {1, 1, 1, 0, 0, 0, 1, 0,    0, 0, 0, 0, 1, 0, 0, 0},
        {0, 1, 1, 1, 0, 0, 0, 1,    0, 0, 0, 0, 0, 1, 0, 0},
        {0, 1, 1, 1, 1, 0, 0, 0,    0, 0, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 1, 1, 1, 0, 1,    0, 0, 0, 0, 0, 0, 0, 1}
};

vector<bool> textToBinary(string text) {
    vector<bool> binary;
    for (int i = 0; i < text.size(); i++) {
        char character = text[i];
        for (int j = wordLength - 1; j >= 0; j--) {
            binary.push_back((character >> j) & 1);
        }
    }
    return binary;
}

string binaryToText(vector<bool> binary) {
    string text;
    for (int i = 0; i < binary.size(); i += wordLength) {
        char character = 0;
        for (int j = 0; j < wordLength; j++) {
            character |= binary[i + j] << (wordLength - 1 - j);
        }
        text += character;
    }
    return text;
}

vector<bool> addParityBits(vector<bool> binary) {
    vector<bool> binaryWithParityBits;
    for (int wordStartBit = 0; wordStartBit < binary.size(); wordStartBit += wordLength) {
        for (int currentWordBit = 0; currentWordBit < wordLength; currentWordBit++) {
            binaryWithParityBits.push_back(binary[wordStartBit + currentWordBit]);
        }
        for (int j = 0; j < wordLength; j++) {
            int parityBit = 0;
            for (int k = 0; k < wordLength; k++) {
                parityBit += (H[j][k] * binary[wordStartBit + k]);
            }
            parityBit %= 2;
            binaryWithParityBits.push_back(parityBit);
        }
    }
    return binaryWithParityBits;
}

vector<bool> removeParityBits(vector<bool> binaryWithParityBits) {
    vector<bool> binaryWithoutParityBits;
    for (int wordStartBit = 0; wordStartBit < binaryWithParityBits.size(); wordStartBit += codeWordLength) {
        for (int currentBit = 0; currentBit < wordLength; currentBit++) {
            binaryWithoutParityBits.push_back(binaryWithParityBits[wordStartBit + currentBit]);
        }
    }
    return binaryWithoutParityBits;
}

vector<bool> getErrorVector(vector<bool> T) {
    vector<bool> E;
    for (int wordStartBit = 0; wordStartBit < T.size(); wordStartBit += codeWordLength) {
        for (int currentWordBit = 0; currentWordBit < wordLength; currentWordBit++) {
            int result = 0;
            for (int i = 0; i < codeWordLength; i++) {
                result += T[wordStartBit + i] * H[currentWordBit][i];
            }
            result %= 2;
            E.push_back(result);
        }
    }
    return E;
}

vector<bool> findSingleErrors(vector<bool> E) {
    vector<bool> singleErrors;
    for (int wordStartBit = 0; wordStartBit < E.size(); wordStartBit += wordLength) {
        for (int HColumn = 0; HColumn < codeWordLength; HColumn++) {
            bool isColumnEqualToE = true;
            for (int HRow = 0; HRow < codeWordLength - wordLength; HRow++) {
                if (H[HRow][HColumn] != E[wordStartBit + HRow]) {
                    isColumnEqualToE = false;
                }
            }
            singleErrors.push_back(isColumnEqualToE);
        }
    }
    return singleErrors;
}

vector<int> getErrorColumns(vector<bool> word) {
    vector<int> columnNumbers;
    for (int firstHColumn = 0; firstHColumn < codeWordLength; firstHColumn++) {
        for (int secondHColumn = firstHColumn + 1; secondHColumn < codeWordLength; secondHColumn++) {
            bool areColumnsEqualToE = true;
            for (int HRow = 0; HRow < codeWordLength - wordLength; HRow++) {
                if (((H[HRow][firstHColumn] + H[HRow][secondHColumn]) % 2) != word[HRow]) {
                    areColumnsEqualToE = false;
                }
            }
            if (areColumnsEqualToE) {
                columnNumbers.push_back(firstHColumn);
                columnNumbers.push_back(secondHColumn);
            }
        }
    }
    return columnNumbers;
}

vector<bool> findDoubleErrors(vector<bool> E) {
    vector<bool> doubleErrors;
    for (int wordStartBit = 0; wordStartBit < E.size(); wordStartBit += wordLength) {
        vector<bool> word;
        for (int bit = 0; bit < wordLength; bit++) {
            word.push_back(E[wordStartBit + bit]);
        }
        vector<int> errorColumns = getErrorColumns(word);
        for (int HColumn = 0; HColumn < codeWordLength; HColumn++) {
            if (!errorColumns.empty() && (HColumn == errorColumns[0] || HColumn == errorColumns[1])) {
                doubleErrors.push_back(1);
            } else {
                doubleErrors.push_back(0);
            }
        }
    }
    return doubleErrors;
}

vector<bool> findAllErrors(vector<bool> E) {
    vector<bool> singleErrors = findSingleErrors(E);
    vector<bool> doubleErrors = findDoubleErrors(E);
    vector<bool> allErrors;
    for (int i = 0; i < singleErrors.size(); i++) {
        allErrors.push_back(singleErrors[i] + doubleErrors[i]);
    }
    return allErrors;
}

vector<bool> correctMessage(vector<bool> message) {
    vector<bool> errorVector = getErrorVector(message);
    vector<bool> errorPositions = findAllErrors(errorVector);
    vector<bool> correctedMessage;
    for (int i = 0; i < errorPositions.size(); i++) {
        correctedMessage.push_back((message[i] + errorPositions[i]) % 2);
    }
    return correctedMessage;
}

void prepareToTransmit() {
    string originalMessage;
    ifstream inputFile ("../input.txt");
    if (inputFile.is_open()) {
        while (!inputFile.eof()) {
            string line;
            getline(inputFile, line);
            originalMessage.append(line);
        }
    } else {
        cout << "Cannot open file.";
    }
    inputFile.close();
    vector<bool> binaryMessage = textToBinary(originalMessage);
    ofstream inputBinaryFile ("../inputBinary.txt");
    if (inputBinaryFile.is_open()) {
        for (int i = 0; i < binaryMessage.size(); i++) {
            inputBinaryFile << binaryMessage[i];
        }
    } else {
        cout << "Cannot open file.";
    }
    inputBinaryFile.close();
    vector<bool> encodedMessage = addParityBits(binaryMessage);
    ofstream inputEncodedFile ("../inputEncoded.txt");
    if (inputEncodedFile.is_open()) {
        for (int i = 0; i < encodedMessage.size(); i++) {
            inputEncodedFile << encodedMessage[i];
        }
    } else {
        cout << "Cannot open file.";
    }
    inputEncodedFile.close();
}

void receiveTransmission() {
    vector<bool> encodedMessage;
    ifstream outputEncodedFile ("../inputEncoded.txt");
    if (outputEncodedFile.is_open()) {
        string line;
        getline(outputEncodedFile, line);
        for (char c : line) {
            if (c == '0')
                encodedMessage.push_back(0);
            else if (c == '1')
                encodedMessage.push_back(1);
        }
    } else {
        cout << "Cannot open file.";
    }
    outputEncodedFile.close();
    string decodedMessage = binaryToText(removeParityBits(correctMessage(encodedMessage)));
    ofstream outputFile ("../output_text.txt");
    if (outputFile.is_open()) {
        outputFile << decodedMessage;
    } else {
        cout << "Cannot open file.";
    }
    outputFile.close();
}

int main() {
    int choice;
    while(true) {
        cout << "Choose an action:" << endl;
        cout << "-prepare to transmit (1)," << endl;
        cout << "-receive transmission (2)," << endl;
        cout << "-exit the program (3)" << endl;
        cin >> choice;
        if (choice == 1) {
            prepareToTransmit();
        } else if (choice == 2) {
            vector<bool> encodedMessage;
            ifstream outputEncodedFile ("../inputEncoded.txt");
            string originalMessage;
            if (outputEncodedFile.is_open()) {
                string line;
                getline(outputEncodedFile, line);
                for (char c : line) {
                    if (c == '0')
                        encodedMessage.push_back(false);
                    else if (c == '1')
                        encodedMessage.push_back(true);
                }
                cout << "Encoded message:           ";
                for (int i = 0; i < encodedMessage.size(); i++) {
                    cout << encodedMessage[i];
                }
                cout << endl;
                cout << "Error vector:              ";
                vector<bool> errorVector = getErrorVector(encodedMessage);
                for (int i = 0; i < errorVector.size(); i++) {
                    cout << errorVector[i];
                }
                cout << endl;
                cout << "Single error positions:    ";
                vector<bool> errors = findSingleErrors(errorVector);
                for (int i = 0; i < errors.size(); i++) {
                    cout << errors[i];
                }
                cout << endl;
                cout << "Double error positions:    ";
                vector<bool> doubleErrors = findDoubleErrors(errorVector);
                for (int i = 0; i < doubleErrors.size(); i++) {
                    cout << doubleErrors[i];
                }
                cout << endl;
                cout << "All error positions:       ";
                vector<bool> allErrors = findAllErrors(errorVector);
                for (int i = 0; i < allErrors.size(); i++) {
                    cout << allErrors[i];
                }
                cout << endl;
                cout << "Corrected encoded message: ";
                vector<bool> correctedMessage = correctMessage(encodedMessage);
                for (int i = 0; i < correctedMessage.size(); i++) {
                    cout << correctedMessage[i];
                }
                cout << endl;
                cout << "Decoded corrected message: ";
                cout << binaryToText(removeParityBits(correctedMessage));
                cout << endl << endl;
            } else {
                cout << "Cannot open file.";
            }
            receiveTransmission();
        } else if (choice == 3) {
            return 0;
        } else {
            cout << "Invalid choice.";
        }
    }
}