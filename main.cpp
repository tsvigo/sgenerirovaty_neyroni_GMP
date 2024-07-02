//#include <QCoreApplication>

// int main(int argc, char *argv[])
// {
//     QCoreApplication a(argc, argv);

//     return a.exec();
// }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////#include <QCoreApplication>

#include <QCoreApplication>
#include <QFile>
#include <QDataStream>
#include <QRandomGenerator>
#include <gmpxx.h>
#include <iostream>
#include <limits>

const int NUMBER_COUNT = 205;
const QString FILE_NAME = "/home/viktor/my_projects_qt_2/sgenerirovaty_neyroni_GMP/random_numbers.bin";

mpz_class generateRandomMPZ() {
    quint64 randomValue = QRandomGenerator::global()->generate64();
    qint64 signedRandomValue = static_cast<qint64>(randomValue);
    return mpz_class(std::to_string(signedRandomValue));
}

void writeNumbersToFile(const QList<mpz_class>& numbers) {
    QFile file(FILE_NAME);
    if (!file.open(QIODevice::WriteOnly)) {
        std::cerr << "Failed to open file for writing" << std::endl;
        return;
    }
    QDataStream out(&file);
    for (const auto& number : numbers) {
        QByteArray byteArray = number.get_str().c_str();
        out << byteArray;
    }
    file.close();
}

QList<mpz_class> readNumbersFromFile() {
    QList<mpz_class> numbers;
    QFile file(FILE_NAME);
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "Failed to open file for reading" << std::endl;
        return numbers;
    }
    QDataStream in(&file);
    while (!in.atEnd()) {
        QByteArray byteArray;
        in >> byteArray;
        numbers.append(mpz_class(byteArray.constData()));
    }
    file.close();
    return numbers;
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QList<mpz_class> numbers;
    for (int i = 0; i < NUMBER_COUNT; ++i) {
        numbers.append(generateRandomMPZ());
    }

    writeNumbersToFile(numbers);

    QList<mpz_class> readNumbers = readNumbersFromFile();
    bool verificationSuccessful = (readNumbers == numbers);

    if (verificationSuccessful) {
        std::cout << "Verification successful: numbers match" << std::endl;
    } else {
        std::cout << "Verification failed: numbers do not match" << std::endl;
    }

    return a.exec();
}
